#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/swap_chain_resources.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class BackgroundSwapChainResources : public SwapChainResources
        {
            using Base = SwapChainResources;
            public:

            UC_ENGINE_API ~BackgroundSwapChainResources();

            UC_ENGINE_API BackgroundSwapChainResources(BackgroundSwapChainResources&&) = default;
            UC_ENGINE_API BackgroundSwapChainResources& operator=(BackgroundSwapChainResources&&);

            
            protected:
            BackgroundSwapChainResources(SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx);
        };
    }
}


