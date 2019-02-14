#include "pch.h"
#include "uc_uwp_renderer.h"
#include "uc_uwp_renderer_impl.h"

namespace uc
{
    namespace uwp
    {
        renderer::renderer(bool* window_close, const winrt::Windows::UI::Core::CoreWindow& window, const winrt::Windows::Graphics::Display::DisplayInformation& display_information, ISwapChainPanelNative*  swapchainpanel) :
            m_impl(new uwp::renderer_impl(window_close, window, display_information, swapchainpanel))
        {

        }

        renderer::~renderer()
        {
            delete m_impl;
        }

        void renderer::update()
        {
            m_impl->update();
        }

        void renderer::pre_render()
        {
            m_impl->pre_render();
        }

        void renderer::render()
        {
            m_impl->render();
        }

        void renderer::present()
        {
            m_impl->present();
        }

        void renderer::resize()
        {

        }

        void renderer::set_window(const winrt::Windows::UI::Core::CoreWindow& window)
        {
            m_impl->set_window(window);
        }

        void renderer::set_display_info(const winrt::Windows::Graphics::Display::DisplayInformation& display_information)
        {
            m_impl->set_display_info(display_information);
        }

        void renderer::set_swapchainpanel(const winrt::Windows::UI::Xaml::Controls::SwapChainPanel& swapchainpanel)
        {
            m_impl->set_swapchainpanel(swapchainpanel);
        }

        void renderer::refresh_display_layout()
        {
            m_impl->refresh_display_layout();
        }

        concurrency::task<void> renderer::initialize_async()
        {
            return concurrency::create_task([this]
            {
                m_impl->initialize_resources();
            });
        }

        concurrency::task<void> renderer::shutdown_async()
        {
            return concurrency::create_task([this]()
            {
                delete m_impl;
                m_impl = nullptr;
            });
        }
    }
}

