#pragma once


#include "uc_uwp_device_resources.h"

#include <uc_dev/io/pad.h>
#include <uc_dev/io/mouse.h>
#include <uc_dev/io/keyboard.h>


namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            struct size
            {
                uint16_t m_width;
                uint16_t m_height;
            };

            struct render_context
            {
                device_resources* m_resources;

                size              m_back_buffer_size;
                size              m_front_buffer_size;
            };

            struct update_context
            {
                double            m_frame_time;
                device_resources* m_resources;

                io::pad_state       m_pad_state;
                io::mouse_state     m_mouse_state;
                io::keyboard_state  m_keyboard_state;

                size                m_back_buffer_size;
                size                m_front_buffer_size;


                bool*               m_window_close;
            };

            struct initialize_context
            {
                device_resources* m_resources;
            };
        }
    }

}