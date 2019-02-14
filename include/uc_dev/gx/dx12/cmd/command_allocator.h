#pragma once

#include <queue>

#include <uc_dev/util/noncopyable.h>
#include <uc_dev/gx/dx12/api/helpers.h>
#include <uc_dev/gx/dx12/cmd/command_queue.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {

            //holds per thread data
            class gpu_direct_command_allocator : public util::noncopyable
            {
                public:

                gpu_direct_command_allocator(ID3D12Device* d, gpu_command_queue* q)
                    : m_device(d)
                    , m_queue(q)
                {

                }

                ID3D12CommandAllocator* allocate()
                {
                    std::lock_guard< std::mutex> g(m_allocators_mutex);
                    auto&& ready = m_ready_allocators_direct[m_frame_index];

                    if ( ready.empty() )
                    {
                        auto r0 = create_command_allocator(m_device.Get(), static_cast< D3D12_COMMAND_LIST_TYPE >(m_queue->type()));
                        auto r  = r0.Detach();
                        r->Reset();
                        return r;
                    }
                    else
                    {
                        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> r0 = ready.back();
                        
                        ready.pop_back();

                        auto r = r0.Detach();
                        r->Reset();

                        return r;
                    }
                }

                void free( gpu_fence_value, ID3D12CommandAllocator* r0 )
                {
                    std::lock_guard< std::mutex> g(m_allocators_mutex);
                    auto&& retired = m_retired_allocators_direct[m_frame_index];

                    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> r;
                    r.Attach(r0);

                    retired.push_back(r);
                }

                void sync()
                {
                    m_frame_index++;
                    m_frame_index %= 3;

                    std::lock_guard< std::mutex> g(m_allocators_mutex);

                    m_ready_allocators_direct[m_frame_index].swap(m_retired_allocators_direct[m_frame_index]);
                }

                private:

                Microsoft::WRL::ComPtr<ID3D12Device >                                                       m_device;
                gpu_command_queue*                                                                          m_queue;

                std::mutex                                                                                  m_allocators_mutex;

                std::vector< Microsoft::WRL::ComPtr<ID3D12CommandAllocator> >                               m_ready_allocators_direct[3];
                std::vector< Microsoft::WRL::ComPtr<ID3D12CommandAllocator> >                               m_retired_allocators_direct[3];
                
                uint64_t                                                                                    m_frame_index = 0;

            };

            class gpu_copy_command_allocator : public util::noncopyable
            {

                public:

                gpu_copy_command_allocator(ID3D12Device* d, gpu_command_queue* q)
                    : m_device(d)
                    , m_queue(q)
                {

                }

                ID3D12CommandAllocator* allocate()
                {
                    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> r;
                    {
                        std::lock_guard< std::mutex> g(m_allocators_mutex);
                        if (!m_allocators_command.empty())
                        {
                            const auto& d = m_allocators_command.front();
                            auto fence = std::get<0>(d);

                            if (m_queue->is_fence_complete(fence))
                            {
                                r = std::get<1>(d);
                                m_allocators_command.pop();
                            }

                            // Aim for number of recording threads * number of buffered frames + extra pool for bundles
                            if (!r && m_allocators_command.size() > 32)
                            {
                                m_queue->wait_for_fence(fence);
                                r = std::get<1>(d);
                                m_allocators_command.pop();
                            }
                        }
                    }

                    if (!r)
                    {
                        r = create_command_allocator(m_device.Get(), static_cast< D3D12_COMMAND_LIST_TYPE >(m_queue->type()));

                        auto type = m_queue->type();

                        const wchar_t* name = nullptr;

                        switch (type)
                        {
                            case D3D12_COMMAND_LIST_TYPE_DIRECT: 
                                name = L"Direct Allocator";
                                break;
                            case D3D12_COMMAND_LIST_TYPE_BUNDLE:
                                name = L"Bundle Allocator";
                                break;
                            case D3D12_COMMAND_LIST_TYPE_COMPUTE:
                                name = L"Compute Allocator";
                                break;
                            case D3D12_COMMAND_LIST_TYPE_COPY:
                                name = L"Copy Allocator";
                                break;
                        }

                        r->SetName(name);
                    }

                    r->Reset();
                    return r.Detach();
                }

                void free(gpu_fence_value v, ID3D12CommandAllocator* r)
                {
                    std::lock_guard< std::mutex> g(m_allocators_mutex);
                    m_allocators_command.push(std::make_tuple(v, r));
                }

                void sync()
                {

                }

            private:

                Microsoft::WRL::ComPtr<ID3D12Device >                                                       m_device;
                gpu_command_queue*                                                                          m_queue;

                std::mutex                                                                                  m_allocators_mutex;
                std::queue< std::tuple< gpu_fence_value, Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> >  m_allocators_command;
                uint64_t                                                                                    m_frame_index = 0;
            };


            //common interface for allocating command allocators
            class gpu_command_allocator : public util::noncopyable
            {

                public:

                gpu_command_allocator(ID3D12Device* d, gpu_command_queue* q)
                    :  m_queue(q)
                    ,  m_direct(d, q )
                    ,  m_copy(d, q )
                {

                }

                ID3D12CommandAllocator* allocate()
                {
                    return m_direct.allocate();
                    /*
                    if (m_queue->type() == D3D12_COMMAND_LIST_TYPE_DIRECT)
                    {
                        return m_direct.allocate();
                    }
                    else
                    {
                        return m_copy.allocate();
                    }
                    */
                }

                void free(gpu_fence_value v, ID3D12CommandAllocator* r)
                {
                    m_direct.free(v, r);

                    /*
                    if (m_queue->type() == D3D12_COMMAND_LIST_TYPE_DIRECT)
                    {
                        m_direct.free(v, r);
                    }

                    if (m_queue->type() == D3D12_COMMAND_LIST_TYPE_COPY)
                    {
                        m_copy.free(v, r);
                    }
                    */
                }

                void sync()
                {
                    //if (m_queue->type() == D3D12_COMMAND_LIST_TYPE_DIRECT)
                    {
                        m_direct.sync();
                    }
                }

            private:

                gpu_command_queue*              m_queue;
                gpu_direct_command_allocator    m_direct;
                gpu_copy_command_allocator      m_copy;

            };
        }
    }
}