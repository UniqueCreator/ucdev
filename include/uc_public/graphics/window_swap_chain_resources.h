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
        class WindowSwapChainResources : public BackgroundSwapChainResources
        {
            using Base = BackgroundSwapChainResources;

            public:

            class Impl;

            UC_ENGINE_API WindowSwapChainResources( ResourceCreateContext* ctx, const winrt::Windows::UI::Core::CoreWindow& window, const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation);
            
            UC_ENGINE_API ~WindowSwapChainResources();

            UC_ENGINE_API WindowSwapChainResources(WindowSwapChainResources&&) = default;
            UC_ENGINE_API WindowSwapChainResources& operator=(WindowSwapChainResources&&);

            UC_ENGINE_API void SetLogicalSize(Size2D size);
            UC_ENGINE_API void SetDisplayInformation(const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation);

            protected:
            details::pimpl<Impl> m_impl;
        };

    }
}


