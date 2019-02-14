#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class BackgroundSwapChainResources;
        class ComputeCommandContext;
        class CopyCommandContext;
        class OverlaySwapChainResources;
        class ResourceCreateContext;
        class SwapChainResources;
        class UploadQueue;

        class DeviceResources : private NonCopyable
        {
            public:

            class Impl;

            //todo: restrict to specific swap chain types
            UC_ENGINE_API DeviceResources(ResourceCreateContext*, BackgroundSwapChainResources* r);
            UC_ENGINE_API DeviceResources(ResourceCreateContext*, BackgroundSwapChainResources* background, OverlaySwapChainResources* overlay);
            
            UC_ENGINE_API ~DeviceResources();

            UC_ENGINE_API DeviceResources(DeviceResources&&) = default;
            UC_ENGINE_API DeviceResources& operator=(DeviceResources&&);

            UC_ENGINE_API const UploadQueue* GetUploadQueue() const;
            UC_ENGINE_API UploadQueue* GetUploadQueue();

            UC_ENGINE_API const ResourceCreateContext* GetResourceCreateContext() const;
            UC_ENGINE_API ResourceCreateContext* GetResourceCreateContext();

            UC_ENGINE_API const BackgroundSwapChainResources* GetBackgroundSwapChainResources() const;
            UC_ENGINE_API BackgroundSwapChainResources* GetBackgroundSwapChainResources();
            
            UC_ENGINE_API const OverlaySwapChainResources* GetOverlaySwapChainResources() const;
            UC_ENGINE_API OverlaySwapChainResources* GetOverlaySwapChainResources();

            UC_ENGINE_API std::unique_ptr<CopyCommandContext>    CreateCopyCommandContext();
            UC_ENGINE_API std::unique_ptr<ComputeCommandContext> CreateComputeCommandContext();
            
            UC_ENGINE_API void ResetViewDependentResources();
            UC_ENGINE_API void WaitForGpu();
            UC_ENGINE_API void Present();

            UC_ENGINE_API void Sync();
            
            protected:

            details::pimpl<Impl> m_impl;

        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<DeviceResources::Impl>;
    }
}


