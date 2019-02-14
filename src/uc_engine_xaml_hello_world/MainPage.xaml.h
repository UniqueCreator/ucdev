//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"


#include <uc_public/io/io.h>
#include <uc_public/graphics/graphics.h>

#include <windows.ui.core.h>
#include <windows.ui.xaml.h>
#include <windows.graphics.display.h>
#include <windows.ui.xaml.controls.h>

#include <mutex>


namespace uc_engine_xaml_hello_world
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public ref class MainPage sealed
    {
        std::unique_ptr<UniqueCreator::Graphics::ResourceCreateContext>                 m_resource_create_context;
        std::unique_ptr<UniqueCreator::Graphics::CompositionSwapChainResources>         m_background_swap_chain;

        std::unique_ptr<UniqueCreator::Graphics::DeviceResources>                       m_device_resources;

        std::unique_ptr<UniqueCreator::InputOutput::Mouse>                              m_mouse;
        std::unique_ptr<UniqueCreator::InputOutput::GamePad>                            m_game_pad;
        std::unique_ptr<UniqueCreator::InputOutput::Keyboard>                           m_keyboard;

        UniqueCreator::InputOutput::MouseState                                          m_mouse_state;
        UniqueCreator::InputOutput::KeyboardState                                       m_keyboard_state;
        UniqueCreator::InputOutput::GamePadState                                        m_game_pad_state;

        //view dependent buffers
        std::unique_ptr<UniqueCreator::Graphics::ViewDepthBuffer>                       m_depth_buffer;

        //Pipeline state objects
        std::unique_ptr<UniqueCreator::Graphics::GraphicsPipelineState>                 m_full_screen_main;
        std::unique_ptr<std::thread> m_renderer_thread;

        void Render();
        
        bool _windowClosed;

        Platform::Agile<Windows::UI::Core::CoreWindow> _win;

        std::mutex                                                                                         m_render_lock;

        winrt::Windows::UI::Core::CoreWindow::SizeChanged_revoker                                          m_size_changed;
        winrt::Windows::Graphics::Display::DisplayInformation::DpiChanged_revoker                          m_dpi_changed;
        winrt::Windows::Graphics::Display::DisplayInformation::OrientationChanged_revoker                  m_orientation_changed;
        winrt::Windows::Graphics::Display::DisplayInformation::DisplayContentsInvalidated_revoker          m_display_contents_invalidated;

        void OnSwapChainPanelSizeChanged(Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ args);
        void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Object^ args);
        void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Object^ args);
        void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Object^ args);
        void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
        
    public:
        MainPage(Windows::UI::Core::CoreWindow^ win);

        void BeginRendering();
        void EndRendering();

    };
}
