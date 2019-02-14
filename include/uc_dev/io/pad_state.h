#pragma once

#include <cstdint>
#include <windows.h>

namespace uc
{
    namespace io
    {
        struct pad_state
        {
            struct state
            {
                float    m_left_trigger = 0.0f;
                float    m_right_trigger = 0.0f;

                float    m_thumb_left_x = 0.0f;
                float    m_thumb_left_y = 0.0f;

                float    m_thumb_right_x = 0.0f;
                float    m_thumb_right_y = 0.0f;

                uint32_t  m_buttons = 0;
            };

        public:

            enum mask : uint32_t
            {
                button_0  = (1 << 0),
                button_1  = (1 << 1),
                button_2  = (1 << 2),
                button_3  = (1 << 3),
                button_4  = (1 << 4),
                button_5  = (1 << 5),
                button_6  = (1 << 6),
                button_7  = (1 << 7),
                button_8  = (1 << 8),
                button_9  = (1 << 9),
                button_10 = (1 << 10),
                button_11 = (1 << 11),
                button_12 = (1 << 12),
                button_13 = (1 << 13),
                button_14 = (1 << 14),
                button_15 = (1 << 15),
                button_16 = (1 << 16),
                button_17 = (1 << 17),
                button_18 = (1 << 18),
                button_19 = (1 << 19),
                button_20 = (1 << 20),
                button_21 = (1 << 21),
                button_22 = (1 << 22),
                button_23 = (1 << 23),
                button_24 = (1 << 24),
                button_25 = (1 << 25),
                button_26 = (1 << 26),
                button_27 = (1 << 27),
                button_28 = (1 << 28),
                button_29 = (1 << 29),
                button_30 = (1 << 30),
            };

            enum gamepad_mask : uint32_t
            {
                menu                        = button_0,
                view                        = button_1,
                button_a                    = button_2,
                button_b                    = button_3,
                button_x                    = button_4,
                button_y                    = button_5,
                button_dpad_up              = button_6,
                button_dpad_down            = button_7,
                button_dpad_left            = button_8,
                button_dpad_right           = button_9,
                button_left_shoulder        = button_10,
                button_right_shoulder       = button_11,
                button_left_thumbstick      = button_12,
                button_right_thumbstick    = button_13
            };

            struct difference
            {
                uint32_t m_v;

                difference(uint32_t v) : m_v(v) {}

                operator uint32_t() const
                {
                    return m_v;
                }
            };

            void swap()
            {
                m_shadow_state = m_state;
            }

            difference get_difference() const
            {
                return m_state.m_buttons ^ m_shadow_state.m_buttons;
            }

            void set_state(uint32_t state)
            {
                m_state.m_buttons = state;
            }

            void set_mask(uint32_t mask)
            {
                m_state.m_buttons = m_state.m_buttons | mask;
            }

            void reset_mask(uint32_t mask)
            {
                m_state.m_buttons = m_state.m_buttons & (~(mask));
            }

            void button_down(uint32_t button)
            {
                m_state.m_buttons = m_state.m_buttons | button;
            }

            void button_up(uint32_t button)
            {
                m_state.m_buttons = m_state.m_buttons & (~(button));
            }

            bool is_button_down(uint32_t button) const
            {
                return (m_state.m_buttons & button) != 0;
            }

            bool is_button_up(uint32_t button) const
            {
                return (m_state.m_buttons & button) == 0;
            }


            state   m_state;
            state   m_shadow_state;

        };

        inline bool has_difference(pad_state::difference difference, uint32_t button)
        {
            return (difference & button) != 0;
        }

        inline bool button_was_pressed(const pad_state & pad, uint32_t bit)
        {
            const io::pad_state::difference    differences = pad.get_difference();
            return (pad.is_button_down(bit) && io::has_difference(differences, bit));
        }

        inline bool button_was_released(const pad_state & pad, uint32_t bit)
        {
            const io::pad_state::difference    differences = pad.get_difference();
            return (pad.is_button_up(bit) && io::has_difference(differences, bit));
        }

    }
}
