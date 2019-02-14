#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/overlay_swap_chain_resources.h>

#include <uc_public/graphics/graphics_types.h>

#include <winrt/windows.ui.core.h>
#include <winrt/windows.graphics.display.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class WindowOverlaySwapChainResources : public OverlaySwapChainResources
        {
            using Base = OverlaySwapChainResources;

            public:

            class Impl;

            UC_ENGINE_API WindowOverlaySwapChainResources( ResourceCreateContext* ctx, const winrt::Windows::UI::Core::CoreWindow& window, const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation );
            UC_ENGINE_API ~WindowOverlaySwapChainResources();

            UC_ENGINE_API WindowOverlaySwapChainResources(WindowOverlaySwapChainResources&&) = default;
            UC_ENGINE_API WindowOverlaySwapChainResources& operator=(WindowOverlaySwapChainResources&&);

            UC_ENGINE_API void SetLogicalSize(Size2D size);
            UC_ENGINE_API void SetDisplayInformation(const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation);

        protected:
            details::pimpl<Impl> m_impl;
        };
    }
}


