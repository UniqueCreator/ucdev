#include "pch.h"

#include <uc_public/graphics/overlay_swap_chain_resources.h>
#include <uc_public/uc_pimpl_impl.h>

#include "swap_chain_resources_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        OverlaySwapChainResources::OverlaySwapChainResources(SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx) : SwapChainResources(s, q, ctx)
        {

        }

        OverlaySwapChainResources::~OverlaySwapChainResources() = default;
        OverlaySwapChainResources& OverlaySwapChainResources::operator=(OverlaySwapChainResources&&) = default;

        void OverlaySwapChainResources::Present( const Rectangle2D * rectangle, uint32_t rectangle_count)
        {
            rectangle;
            rectangle_count;
        }
    }
}


