#pragma once

#include <d3d12.h>

#include <atomic>
#include <mutex>
#include <thread>

#include <uc_dev/util/noncopyable.h>

#include <uc_dev/gx/dx12/api/dxgi_helpers.h>
#include <uc_dev/gx/dx12/api/helpers.h>
#include <uc_dev/gx/dx12/gpu/fence_value.h>
#include <pix3.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_command_queue : public util::noncopyable
            {
                void signal_fence(ID3D12Fence* f, uint64_t value);
                void signal_fence(uint64_t value);

                public:

                gpu_command_queue(  ID3D12Device* d, ID3D12CommandQueue*  q ) :
                      m_queue(q)
                    , m_fence( create_fence(d) )
                    , m_fence_value( 1 )
                {
                    //todo: throw exception on error
                    m_wait_fence_handle = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
                }

                ~gpu_command_queue()
                {
                    CloseHandle(m_wait_fence_handle);
                }

                bool is_fence_complete( gpu_fence_value value )
                {
                    std::unique_lock< std::mutex > g(m_queue_mutex);
                    auto completed_value = m_fence->GetCompletedValue();
                    return value <= completed_value;
                }

                void insert_wait_on( gpu_fence v )
                {
                    std::unique_lock< std::mutex > g(m_queue_mutex);
                    signal_fence(v.m_fence, v.m_value);
                }

                gpu_fence_value increment_fence()
                {
                    std::unique_lock< std::mutex > g(m_queue_mutex);
                    signal_fence(m_fence_value);
                    return m_fence_value++;
                }

                gpu_fence signal_fence()
                {
                    std::unique_lock< std::mutex > g(m_queue_mutex);
                    signal_fence(m_fence_value++);
                    
                    gpu_fence r;
                    r.m_fence = m_fence.Get();
                    r.m_value = m_fence_value;

                    return r;
                }
                
                gpu_fence execute_command_list( ID3D12CommandList* list )
                {
                    std::unique_lock< std::mutex > g(m_queue_mutex);

                    ID3D12CommandList* l[] = { list };
                    m_queue->ExecuteCommandLists(1, l);

                    signal_fence(m_fence_value);

                    gpu_fence r;
                    r.m_fence = m_fence.Get();
                    r.m_value = m_fence_value++;
                    
                    return r;
                }

                void wait_for_idle_gpu()
                {
                    wait_for_fence(increment_fence());
                }

                void wait_for_fence(gpu_fence_value value)
                {
                    if ( is_fence_complete( value ) )
                    {
                        return;
                    }

                    {
                        std::lock_guard<std::mutex> g(m_wait_fence_mutex);
                        throw_if_failed(m_fence->SetEventOnCompletion(value, m_wait_fence_handle));
                        WaitForSingleObject(m_wait_fence_handle, INFINITE);
                    }
                }

                void present( IDXGISwapChain1* swap_chain )
                {
                    std::unique_lock< std::mutex > g(m_queue_mutex);
                    auto hr = swap_chain->Present(0, 0); // we do not block on vsync now, this wastes energy and reduces latency, should work only on power unconstrained modes

                    if (hr == DXGI_STATUS_OCCLUDED)
                    {
                        //todo: how this status is obtained?
                    }

                    throw_if_failed(hr);
                }

                D3D12_COMMAND_LIST_TYPE type() const
                {
                    auto d = m_queue->GetDesc();
                    return d.Type;
                }

                ID3D12CommandQueue* queue() const
                {
                    return m_queue.Get();
                }

                ID3D12Fence* fence() const
                {
                    return m_fence.Get();
                }

                void pix_begin_event(const wchar_t* label)
                {
                    PIXBeginEvent(m_queue.Get(), 0, label);
                }

                void pix_end_event(void)
                {
                    PIXEndEvent(m_queue.Get());
                }

                private:

                Microsoft::WRL::ComPtr<ID3D12CommandQueue>  m_queue;
                Microsoft::WRL::ComPtr<ID3D12Fence>         m_fence;
                std::mutex                                  m_queue_mutex;
                uint64_t                                    m_fence_value;

                std::mutex                                  m_wait_fence_mutex;
                HANDLE                                      m_wait_fence_handle;
            };
        }
    }
}