#pragma once

#include <uc_dev/io/mouse_state.h>
#include <uc_dev/io/error.h>
#include <uc_dev/io/platforms/pc/native_registration_token.h>

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.core.h>
#include <winrt/windows.devices.input.h>
#include <winrt/windows.ui.input.h>


namespace uc
{
    namespace io
    {
        struct native_mouse 
        {
            struct state
            {
                bool        m_left_button	= false;
                bool        m_middle_button = false;
                bool        m_right_button = false;
                bool        m_xButton1 = false;
                bool        m_xButton2 = false;
                int16_t     m_x = 0;
                int16_t     m_y = 0;
                int32_t     scrollWheelValue = 0;

            };

            winrt::Windows::UI::Core::CoreWindow		         m_window = nullptr;
            state                                                m_state;
            float												 m_dpi = 96.0f;

            
            winrt::Windows::UI::Core::ICoreWindow::PointerPressed_revoker       m_pointer_pressed;
            winrt::Windows::UI::Core::ICoreWindow::PointerReleased_revoker      m_pointer_released;
            winrt::Windows::UI::Core::ICoreWindow::PointerMoved_revoker         m_pointer_moved;
            winrt::Windows::UI::Core::ICoreWindow::PointerWheelChanged_revoker  m_pointer_wheel_changed;

            native_mouse()
            {

            }

            mouse_state update( const mouse_state& o )
            {
                mouse_state r(o);

                r.swap();

                uint32_t buttons = 0;

                if (m_state.m_left_button)
                {
                    buttons |= mouse_state::button_0;
                }

                if (m_state.m_right_button)
                {
                    buttons |= mouse_state::button_1;
                }

                if (m_state.m_middle_button)
                {
                    buttons |= mouse_state::button_2;
                }

                if (m_state.m_xButton1)
                {
                    buttons |= mouse_state::button_3;
                }

                if (m_state.m_xButton2)
                {
                    buttons |= mouse_state::button_4;
                }

                r.set_position(m_state.m_x, m_state.m_y);
                r.set_state(buttons);

                return r;
            }

            //can be called only on the ui thread
            void set_window(const winrt::Windows::UI::Core::CoreWindow& window, float dpi)
            {
                m_dpi = dpi;
                if (m_window != window)
                {
                    m_window = window;

                    auto handler = [this](auto&&, const winrt::Windows::UI::Core::PointerEventArgs& args)
                    {
                        using namespace winrt::Windows::Foundation;
                        using namespace winrt::Windows::UI::Input;
                        using namespace winrt::Windows::Devices::Input;

                        auto currentPoint = args.CurrentPoint();
                        auto pointerDevice = currentPoint.PointerDevice();
                        auto devType = pointerDevice.PointerDeviceType();
                    
                        if (devType == PointerDeviceType::Mouse)
                        {
                            auto props = currentPoint.Properties();

                            m_state.m_left_button		= props.IsLeftButtonPressed();
                            m_state.m_right_button		= props.IsRightButtonPressed();
                            m_state.m_middle_button	= props.IsMiddleButtonPressed();
                            m_state.m_xButton1			= props.IsXButton1Pressed();
                            m_state.m_xButton2			= props.IsXButton2Pressed();
                        }

                        if (true)
                        {
                            Point pos = currentPoint.Position();
                            float dpi = m_dpi;

                            m_state.m_x = static_cast<int16_t>(pos.X * dpi / 96.f + 0.5f);
                            m_state.m_y = static_cast<int16_t>(pos.Y * dpi / 96.f + 0.5f);
                        }

                        return S_OK;

                    };

                    if (m_window)
                    {
                        m_pointer_pressed = window.PointerPressed(winrt::auto_revoke, handler);
                        m_pointer_released = window.PointerReleased(winrt::auto_revoke, handler);
                        m_pointer_moved = window.PointerMoved(winrt::auto_revoke, handler);
                        m_pointer_wheel_changed = window.PointerWheelChanged(winrt::auto_revoke, handler);
                    }
                    else
                    {
                        m_pointer_wheel_changed.revoke();
                        m_pointer_moved.revoke();
                        m_pointer_released.revoke();
                        m_pointer_pressed.revoke();
                    }
                }
            }

            //can be called only on the ui thread
            void release()
            {
                m_pointer_moved.revoke();
                m_pointer_wheel_changed.revoke();
                m_pointer_released.revoke();
                m_pointer_pressed.revoke();
            }

        };
    }
}
