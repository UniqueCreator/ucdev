#include "pch.h"

#include "swap_chain_resources_impl.h"

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_public/graphics/direct_queue.h>
#include <uc_public/graphics/resource_create_context.h>
#include <uc_public/graphics/back_buffer.h>

#include "resource_create_context_impl.h"
#include "direct_command_context_internal.h"
#include "back_buffer_internal.h"
#include "direct_queue_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        SwapChainResources::Impl::Impl() 
        {

        }

        SwapChainResources::Impl::Impl( SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx ) : m_swap_chain(s), m_direct_queue(q), m_rc(ctx)
        {
            m_direct_command_manager    = std::make_unique<uc::gx::dx12::gpu_command_manager>(ctx->GetImpl()->Device(), q->GetImpl()->queue());
            m_direct_context_allocator  = std::make_unique<uc::gx::dx12::gpu_command_context_allocator>(ctx->GetImpl()->rc(), m_direct_command_manager.get(), q->GetImpl()->queue());

            //insert two queues
            auto queue = m_direct_queue->GetImpl()->queue();
            queue->increment_fence();
            queue->increment_fence();
        }

        void SwapChainResources::Impl::ResetBufferCount(uint32_t buffer_count)
        {
            m_back_buffer.resize(0);
            m_back_buffer.resize(buffer_count);
            m_buffer_count = buffer_count;
        }

        void SwapChainResources::Impl::WaitForGpu()
        {
            m_direct_queue->WaitForIdleGpu();
        }

        void SwapChainResources::Impl::WaitForFence(const FenceHandle*v)
        {
            m_direct_queue->WaitForFence(v);
        }

        void SwapChainResources::Impl::InsertWaitOn(const FenceHandle*v)
        {
            m_direct_queue->InsertWaitOn(v);
        }

        void SwapChainResources::Impl::MoveToNextFrame()
        {
            m_buffer_index = m_buffer_index + 1;
            m_buffer_index = m_buffer_index % m_buffer_count;

            auto queue = m_direct_queue->GetImpl()->queue();
            auto fence = queue->increment_fence();
            queue->wait_for_fence(fence - 2);
        }

        void SwapChainResources::Impl::Sync()
        {
            m_direct_context_allocator->sync();
        }

        void SwapChainResources::Impl::Present()
        {
            uc::gx::dx12::throw_if_failed(m_swap_chain->Present(0, 0));
        }

        void SwapChainResources::Impl::SetSourceSize(uint32_t width, uint32_t height)
        {
            uc::gx::dx12::throw_if_failed(m_swap_chain->SetSourceSize(width, height));
        }

        void SwapChainResources::Impl::Resize( uint32_t width, uint32_t height )
        {
            for (auto i = 0U; i < m_buffer_count; ++i)
            {
                m_back_buffer[i].reset();
            }

            uc::gx::dx12::throw_if_failed(m_swap_chain->ResizeBuffers(m_buffer_count, width, height, m_swap_buffer_format, m_resize_flags));

            uint32_t frame = m_buffer_index;

            auto resource_creator = m_rc->GetImpl()->rc();

            for (auto i = 0U; i < m_buffer_count; ++i)
            {
                using namespace uc;

                //Recreate the back buffers
                Microsoft::WRL::ComPtr<ID3D12Resource> back_buffer;
                gx::dx12::throw_if_failed(m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&back_buffer)));

                m_back_buffer[frame] = std::make_unique<BackBufferInternal>(std::unique_ptr<gx::dx12::gpu_back_buffer>(resource_creator->create_back_buffer(back_buffer.Get())));

                frame++;
                frame %= m_buffer_count;
            }
        }

        std::unique_ptr<DirectCommandContext> SwapChainResources::Impl::CreateCommandContext()
        {
            return std::make_unique<DirectCommandContextInternal>(uc::gx::dx12::create_graphics_command_context( m_direct_context_allocator.get()));
        }

        BackBuffer* SwapChainResources::Impl::GetBackBuffer() const
        {
            return m_back_buffer[m_buffer_index].get();
        }
    }
}


