#pragma once

#include <concurrent_queue.h>

#include "winrt/base.h"
#include <winrt/windows.ui.core.h>
#include <winrt/windows.graphics.display.h>
#include <winrt/windows.ui.xaml.controls.h>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/sys/profile_timer.h>

#include <uc_dev/gx/geo/indexed_geometry.h>
#include <uc_dev/gxu/pinhole_camera_dispatcher.h>

#include <uc_dev/io/pad.h>
#include <uc_dev/io/mouse.h>
#include <uc_dev/io/keyboard.h>

#include "uc_uwp_device_resources.h"

#include "uc_uwp_renderer_overlay_page_manager.h"
#include "uc_uwp_gx_render_world_manager.h"

#include "uc_uwp_gx_render_object.h"
#include "uc_uwp_renderer_impl_command.h"

#include <uc_dev/gx/geo/geometry_allocator.h>

struct ISwapChainPanelNative;

namespace uc
{
    namespace uwp
    {
        class renderer_impl : public util::noncopyable
        {
        public:

            renderer_impl(bool* window_close, const winrt::Windows::UI::Core::CoreWindow& window, const winrt::Windows::Graphics::Display::DisplayInformation& display_information, ISwapChainPanelNative*  swapchainpanel = nullptr);
            ~renderer_impl();

            void update();
            void pre_render();
            void render();
            void process_user_input();

            void present();
            void resize();

            void set_display_info(const winrt::Windows::Graphics::Display::DisplayInformation& display_information);
            void set_window(const winrt::Windows::UI::Core::CoreWindow& window);
            void set_swapchainpanel(const winrt::Windows::UI::Xaml::Controls::SwapChainPanel&  swapchainpanel);

            void refresh_display_layout();
            
            void resize_buffers(const window_environment* environment);

            void initialize_resources();

        private:

            device_resources                                                                    m_resources;

            uint32_t                                                                            m_frame_index = 2;
            std::unique_ptr<gx::dx12::gpu_frame_depth_buffer>                                   m_frame_depth_buffer[3];
            std::unique_ptr<gx::dx12::gpu_frame_msaa_depth_buffer>                              m_frame_shadow_buffer[3];
            std::unique_ptr<gx::dx12::gpu_frame_color_buffer>                                   m_frame_shadow_map[3];

            std::unique_ptr<gx::geo::geometry_allocator>                                        m_geometry_allocator;

            std::unique_ptr<gxu::render_world_manager>                                          m_render_world_manager;
            std::unique_ptr<overlay::page_manager>                                              m_overlay_page_manager;

            window_environment                                                                  m_window_enviroment;
            winrt::Windows::UI::Core::CoreWindow                                                m_window = nullptr;
            winrt::Windows::Graphics::Display::DisplayInformation                               m_display_information = nullptr;

            io::pad                                                                             m_pad;
            io::pad_state                                                                       m_pad_state;


            io::mouse                                                                           m_mouse;
            io::mouse_state                                                                     m_mouse_state;
            io::keyboard                                                                        m_keyboard;
            io::keyboard_state                                                                  m_keyboard_state;


            float                                                                               m_scale_render = 1.0f;

            sys::profile_timer                                                                  m_frame_timer;
            double                                                                              m_frame_time;

            bool*                                                                               m_main_window;
            Concurrency::concurrent_queue < resize_window_command >                             m_prerender_queue;
            void                                                                                flush_prerender_queue();
        };
    }

}