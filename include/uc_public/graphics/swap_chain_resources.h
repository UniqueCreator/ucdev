#pragma once

#include <uc_public/uc_implementation.h>
#include <dxgi1_5.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class BackBuffer;
        class DirectCommandContext;
        class DirectQueue;
        class FenceHandle;
        class ResourceCreateContext;

        using SwapChainNative = IDXGISwapChain3;

        class SwapChainResources : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API ~SwapChainResources();

            UC_ENGINE_API SwapChainResources(SwapChainResources&&) = default;
            UC_ENGINE_API SwapChainResources& operator=(SwapChainResources&&);

            UC_ENGINE_API void Resize( uint32_t width, uint32_t height );

            UC_ENGINE_API void WaitForGpu();
            UC_ENGINE_API void WaitForFence(const FenceHandle*v);
            UC_ENGINE_API void InsertWaitOn(const FenceHandle*v);
            UC_ENGINE_API void Present();
            UC_ENGINE_API void Sync();
            UC_ENGINE_API void MoveToNextFrame();
            

            UC_ENGINE_API void SetSourceSize(uint32_t width, uint32_t height);

            UC_ENGINE_API SwapChainNative* GetSwapChain()  const;
            UC_ENGINE_API BackBuffer*      GetBackBuffer() const;

            UC_ENGINE_API std::unique_ptr<DirectCommandContext> CreateCommandContext();

            protected:

            SwapChainResources(SwapChainNative* swapChain, DirectQueue* queue, ResourceCreateContext* ctx );

            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SwapChainResources::Impl>;
    }
}


