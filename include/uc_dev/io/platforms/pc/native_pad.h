#pragma once

#include <uc_dev/io/pad_state.h>
#include <uc_dev/io/error.h>

#include <winrt/windows.foundation.h>
#include <winrt/windows.gaming.input.h>

#include <exception>

#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include <windows.gaming.input.h>
#include <Windows.Foundation.Collections.h>

namespace uc
{
    namespace io
    {
        using namespace winrt::Windows::Gaming::Input;

        namespace details
        {
            namespace naitve_pad
            {
                static const uint32_t invalid_user_index = 0xFFFFFFFF;

                static inline Gamepad get_pad(const IGamepadStatics& statics )
                {
                    using namespace winrt::Windows::Foundation::Collections;
                    
                    auto pads   = statics.Gamepads();

                    if (pads.Size() > 0)
                    {
                        return pads.GetAt(0);
                    }
                    else
                    {
                        return winrt::Windows::Gaming::Input::Gamepad(nullptr);
                    }
                }

                static inline winrt::Windows::Gaming::Input::IGamepadStatics create_statics()
                {
                    return winrt::get_activation_factory<Gamepad, IGamepadStatics>();
                }

                static inline winrt::Windows::Gaming::Input::IGamepadStatics get_statics()
                {
                    static winrt::Windows::Gaming::Input::IGamepadStatics statics = create_statics();
                    return statics;
                }
            }
        }

        static inline float filter_value(float dead_zone, float value)
        {
            if (fabsf(value) < dead_zone)
            {
                return 0.0f;
            }
            else
            {
                return value;
            }
        }

        struct native_pad
        {
            Gamepad::GamepadAdded_revoker   m_pad_added;
            Gamepad::GamepadRemoved_revoker m_pad_removed;

            IGamepadStatics m_statics;
            Gamepad         m_pad;

            private:

            void on_gamepad_added(const winrt::Windows::Foundation::IInspectable&, const winrt::Windows::Gaming::Input::Gamepad& pad)
            {
                m_pad = pad;
            }

            void on_gamepad_removed(const winrt::Windows::Foundation::IInspectable&, const winrt::Windows::Gaming::Input::Gamepad&)
            {
                m_pad = nullptr;
            }

            public:

            native_pad() : 
                m_statics ( details::naitve_pad::get_statics() )
            ,   m_pad(details::naitve_pad::get_pad(m_statics))
            {
                m_pad_added     = Gamepad::GamepadAdded(winrt::auto_revoke, { this,&native_pad::on_gamepad_added });
                m_pad_removed   = Gamepad::GamepadRemoved(winrt::auto_revoke, { this,&native_pad::on_gamepad_removed });
            }

            pad_state update(const pad_state& o) const
            {
                if (m_pad)
                {
                    GamepadReading s = m_pad.GetCurrentReading();

                    pad_state r(o);

                    r.swap();
                    r.set_state(static_cast<uint32_t>(s.Buttons));

                    r.m_state.m_left_trigger = filter_value(0.06666667f, static_cast<float>(s.LeftTrigger));
                    r.m_state.m_right_trigger = filter_value(0.06666667f, static_cast<float>(s.RightTrigger));

                    r.m_state.m_thumb_left_x = filter_value(0.23f, static_cast<float>(s.LeftThumbstickX));
                    r.m_state.m_thumb_left_y = filter_value(0.23f, static_cast<float>(s.LeftThumbstickY));

                    r.m_state.m_thumb_right_x = filter_value(0.23f, static_cast<float>(s.RightThumbstickX));
                    r.m_state.m_thumb_right_y = filter_value(0.23f, static_cast<float>(s.RightThumbstickY));
                    return r;
                }
                else
                {
                    return o;
                }
            }
        };
    }
}
