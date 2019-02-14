#pragma once

#include <uc_dev/gx/dx12/api/helpers.h>

#include "command_allocator.h"
#include "command_queue.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_command_manager : public util::noncopyable
            {
                public:

                enum class list_type
                {
                    direct  = D3D12_COMMAND_LIST_TYPE_DIRECT,
                    bundle  = D3D12_COMMAND_LIST_TYPE_BUNDLE,
                    compute = D3D12_COMMAND_LIST_TYPE_COMPUTE,
                    copy    = D3D12_COMMAND_LIST_TYPE_COPY
                };

                struct command_list
                {
                    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_list;
                    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    m_allocator;

                    operator ID3D12GraphicsCommandList*()
                    {
                        return m_list.Get();;
                    }

                    void reset()
                    {
                        m_list.Reset();
                        m_allocator.Reset();
                    }
                };

                gpu_command_manager( ID3D12Device* d, gpu_command_queue* q ) :
                    m_device(d )
                    , m_allocator(d, q)
                {

                }

                ~gpu_command_manager()
                {

                }

                command_list create_command_list( list_type type )
                {

                    auto a = m_allocator.allocate();
                    auto l = create_graphics_command_list(m_device.Get(), 0, static_cast<D3D12_COMMAND_LIST_TYPE>(type), a, nullptr);
                    l->Close();
                    l->Reset(a, nullptr);

                    return{ l, a };
                }

                void free_command_list( gpu_fence_value v, command_list & list)
                {
                    m_allocator.free( v, list.m_allocator.Get() );
                }

                ID3D12Device* device() const
                {
                    return m_device.Get();
                }

                void sync()
                {
                    m_allocator.sync();
                }

            private:

                Microsoft::WRL::ComPtr<ID3D12Device> m_device;
                gpu_command_allocator                m_allocator;
            };
        }
    }
}