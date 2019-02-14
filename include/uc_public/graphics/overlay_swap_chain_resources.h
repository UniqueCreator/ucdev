#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/swap_chain_resources.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class OverlaySwapChainResources : public SwapChainResources
        {
            using Base = SwapChainResources;

            public:

            UC_ENGINE_API ~OverlaySwapChainResources();

            UC_ENGINE_API OverlaySwapChainResources(OverlaySwapChainResources&&) = default;
            UC_ENGINE_API OverlaySwapChainResources& operator=(OverlaySwapChainResources&&);

            //todo: partial presentation
            UC_ENGINE_API void Present( const Rectangle2D * rectangle, uint32_t rectangle_count );

            using Base::Present;
            

            protected:
            OverlaySwapChainResources(SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx);
            
        };
    }
}


