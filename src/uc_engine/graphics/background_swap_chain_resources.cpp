#include "pch.h"

#include <uc_public/graphics/background_swap_chain_resources.h>
#include <uc_public/uc_pimpl_impl.h>

#include "swap_chain_resources_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        BackgroundSwapChainResources::BackgroundSwapChainResources(SwapChainNative* s, DirectQueue* q, ResourceCreateContext* ctx) : Base(s,q,ctx)
        {
            
        }

        BackgroundSwapChainResources::~BackgroundSwapChainResources() = default;
        BackgroundSwapChainResources& BackgroundSwapChainResources::operator=(BackgroundSwapChainResources&&) = default;

    }
}


