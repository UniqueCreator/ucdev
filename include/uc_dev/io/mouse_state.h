#pragma once

#include <cstdint>
#include <windows.h>

namespace uc
{
    namespace io
    {
        struct mouse_state
        {
            struct state
            {
                uint32_t  m_buttons = 0;
                int16_t   m_x		= 0;
                int16_t   m_y		= 0;
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

            enum mouse_mask : uint32_t
            {
                left                        = button_0,
                right                       = button_1,
                middle                      = button_2
                
            };

            struct difference
            {
                uint32_t m_v;

                difference( uint32_t v) : m_v( v ){}

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

            void set_position(uint16_t x, uint16_t y)
            {
                m_state.m_x = x;
                m_state.m_y = y;
            }

            int16_t x() const
            {
                return m_state.m_x;
            }

            int16_t y() const
            {
                return m_state.m_y;
            }

            int16_t dx() const
            {
                return m_state.m_x - m_shadow_state.m_x;
            }

            int16_t dy() const
            {
                return m_state.m_y - m_shadow_state.m_y;
            }

            state   m_state;
            state   m_shadow_state;
        };

        inline bool has_difference(mouse_state::difference difference, uint32_t button)
        {
            return (difference & button) != 0;
        }

        inline bool button_was_pressed(const mouse_state & mouse, uint32_t bit)
        {
            const io::mouse_state::difference    differences = mouse.get_difference();
            return (mouse.is_button_down(bit) && io::has_difference(differences, bit));
        }

        inline bool button_was_released(const mouse_state & mouse, uint32_t bit)
        {
            const io::mouse_state::difference    differences = mouse.get_difference();
            return (mouse.is_button_up(bit) && io::has_difference(differences, bit));
        }

    }
}
