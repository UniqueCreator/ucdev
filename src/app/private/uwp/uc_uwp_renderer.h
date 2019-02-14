#pragma once

#include <ppltasks.h>
#include <memory>


#include "winrt/base.h"
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

struct ISwapChainPanelNative;

namespace uc
{
    namespace uwp
    {
        class renderer
        {
            public:

            renderer(bool* window_close, const winrt::Windows::UI::Core::CoreWindow& window, const winrt::Windows::Graphics::Display::DisplayInformation& display_information, ISwapChainPanelNative*  swapchainpanel = nullptr);

            ~renderer();

            concurrency::task<void> initialize_async();
            concurrency::task<void> shutdown_async();

            void update();
            void pre_render();
            void render();
            void present();
            void resize();

            void set_window(const winrt::Windows::UI::Core::CoreWindow& window);
            void set_display_info(const winrt::Windows::Graphics::Display::DisplayInformation& display_information);
            void set_swapchainpanel(const winrt::Windows::UI::Xaml::Controls::SwapChainPanel&  swapchainpanel);

            void refresh_display_layout();

            private:

            class renderer_impl* m_impl;
        };
    }
}
