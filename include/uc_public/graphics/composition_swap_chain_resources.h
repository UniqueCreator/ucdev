#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/background_swap_chain_resources.h>

#include <uc_public/graphics/graphics_types.h>

#include <winrt/windows.ui.core.h>
#include <winrt/windows.graphics.display.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class CompositionSwapChainResources : public BackgroundSwapChainResources
        {
            using Base = BackgroundSwapChainResources;
            public:

            class Impl;

            UC_ENGINE_API CompositionSwapChainResources( ResourceCreateContext* ctx, Size2D logicalSize, const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation, float compositionScaleX = 1.0f, float compositionScaleY = 1.0f);

            UC_ENGINE_API ~CompositionSwapChainResources();

            UC_ENGINE_API CompositionSwapChainResources(CompositionSwapChainResources&&) = default;
            UC_ENGINE_API CompositionSwapChainResources& operator=(CompositionSwapChainResources&&);

            UC_ENGINE_API void SetCompositionScale( float scaleX, float scaleY );
            UC_ENGINE_API void SetLogicalSize( Size2D size );
            UC_ENGINE_API void SetDisplayInformation( const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation );

            protected:
            details::pimpl<Impl> m_impl;
        };

    }
}


