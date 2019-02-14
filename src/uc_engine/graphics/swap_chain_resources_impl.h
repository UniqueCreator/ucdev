#pragma once

#include <memory>
#include <vector>
#include <wrl/client.h>

#include <uc_public/graphics/swap_chain_resources.h>
#include <uc_public/graphics/back_buffer.h>
#include <uc_public/graphics/direct_command_context.h>

#include <uc_dev/gx/dx12/dx12.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class BackBuffer;
        class FenceHandle;
        class DirectQueue;
        class ResourceCreateContext;
        
        class SwapChainResources::Impl
        {
            public:

            uint32_t                                    m_buffer_count = 0;
            uint32_t                                    m_buffer_index = 0;

            Microsoft::WRL::ComPtr<SwapChainNative>     m_swap_chain;
            DirectQueue*                                m_direct_queue = nullptr;
            ResourceCreateContext*                      m_rc = nullptr;
            std::vector< std::unique_ptr<BackBuffer> >  m_back_buffer;
            uint32_t                                    m_resize_flags = 0;
            DXGI_FORMAT                                 m_swap_buffer_format = DXGI_FORMAT_UNKNOWN;

            std::unique_ptr<uc::gx::dx12::gpu_command_manager>                  m_direct_command_manager;
            std::unique_ptr<uc::gx::dx12::gpu_command_context_allocator>        m_direct_context_allocator;

            SwapChainResources::Impl();
            SwapChainResources::Impl(SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx);

            void ResetBufferCount(uint32_t buffer_count);
            void WaitForGpu();
            void WaitForFence(const FenceHandle*v);
            void InsertWaitOn(const FenceHandle*v);
            void Present();
            void Sync();
            void MoveToNextFrame();
            void SetSourceSize(uint32_t width, uint32_t height);
            void Resize(uint32_t width, uint32_t height);

            std::unique_ptr<DirectCommandContext> CreateCommandContext();

            BackBuffer* GetBackBuffer() const;
         
            SwapChainNative* GetSwapChain() const
            {
                return m_swap_chain.Get();
            }
       };
    }
}


