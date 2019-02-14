#pragma once

#include <uc_dev/io/keyboard_state.h>
#include <uc_dev/io/error.h>
#include <uc_dev/io/platforms/pc/native_registration_token.h>

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.core.h>

namespace uc
{
    namespace io
    {
        struct native_keyboard
        {
            struct state
            {
                state()
                {
                    m_keys[0] = 0;
                    m_keys[1] = 0;
                    m_keys[2] = 0;
                    m_keys[3] = 0;
                }

                uint64_t m_keys[4];

                std::vector<uint32_t> m_key;
            };

            winrt::Windows::UI::Core::CoreWindow															m_window	= nullptr;
            state																							m_state;

			winrt::event_revoker < winrt::Windows::UI::Core::ICoreAcceleratorKeys>							m_key;
			winrt::Windows::UI::Core::ICoreWindow::CharacterReceived_revoker								m_character;

			native_keyboard() = default;

            keyboard_state update( const keyboard_state& o )
            {
                keyboard_state r(o);
                r.swap();
                r.set_state(m_state.m_keys);
                r.m_character_codes.swap(m_state.m_key);
                assert(m_state.m_key.empty());
                return r;
            }

            auto get_dispatcher(const winrt::Windows::UI::Core::CoreWindow& w)
			{
				return w.Dispatcher();
            }

            auto get_keys(const winrt::Windows::UI::Core::CoreDispatcher& d)
            {
				return d.as<winrt::Windows::UI::Core::CoreAcceleratorKeys>();
            }

            void set_window(const winrt::Windows::UI::Core::CoreWindow& window)
            {
                if (m_window != window )
                {
                    m_window = window;

                    auto keys = get_keys(get_dispatcher(window));
					
					m_key = keys.AcceleratorKeyActivated( winrt::auto_revoke, [this](auto&&, const winrt::Windows::UI::Core::AcceleratorKeyEventArgs& args)
					{
						using namespace winrt::Windows::System;
						using namespace winrt::Windows::UI::Core;

						CoreAcceleratorKeyEventType type	= args.EventType();
						CorePhysicalKeyStatus status		= args.KeyStatus();
						VirtualKey key						= args.VirtualKey();
						
						uint32_t key_int = static_cast<uint32_t>(key);

						auto byte = key_int / 64;
						auto bit = key_int % 64;

						if (type == CoreAcceleratorKeyEventType::KeyDown || type == CoreAcceleratorKeyEventType::SystemKeyDown)
						{
							m_state.m_keys[byte] = util::bit_set(m_state.m_keys[byte], bit);
						}

						if (type == CoreAcceleratorKeyEventType::KeyUp || type == CoreAcceleratorKeyEventType::SystemKeyUp)
						{
							m_state.m_keys[byte] = util::bit_reset(m_state.m_keys[byte], bit);
						}
					});

					m_character = window.CharacterReceived(winrt::auto_revoke, [this](auto&&, const winrt::Windows::UI::Core::CharacterReceivedEventArgs& a)
					{
						m_state.m_key.push_back(a.KeyCode());
					});
                }
            }

			void release()
            {
				m_key.revoke();
				m_character.revoke();
            }
        };
    }
}
