#pragma once


#include <uc_dev/io/pad.h>
#include <uc_dev/io/mouse.h>
#include <uc_dev/io/keyboard.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_depth_buffer;
            class gpu_msaa_depth_buffer;
            class gpu_color_buffer;
        }

        namespace geo
        {
            class geometry_allocator;
        }
    }

    namespace uwp
    {
        class device_resources;

        namespace gxu
        {
            struct size
            {
                uint16_t m_width;
                uint16_t m_height;
            };

            struct render_context
            {
                device_resources*                   m_resources;
                gx::geo::geometry_allocator*        m_geometry;
                gx::dx12::gpu_depth_buffer*         m_view_depth_buffer;

                size                                m_back_buffer_size;
                size                                m_back_buffer_scaled_size;

                size                                m_front_buffer_size;
                double                              m_frame_time;
                float                               m_scale_render;


                gx::dx12::gpu_msaa_depth_buffer*    m_shadow_depth_buffer;
                gx::dx12::gpu_color_buffer*         m_shadow_map;
                size                                m_shadow_buffer_size;
            };

            struct shadow_render_context : public render_context
            {
            
            };

            struct update_context
            {
                double            m_frame_time;
                
                device_resources*   m_resources;

                io::pad_state       m_pad_state;
                io::mouse_state     m_mouse_state;
                io::keyboard_state  m_keyboard_state;

                size                m_back_buffer_size;
                size                m_front_buffer_size;

                bool*               m_window_close;
            };

            struct initialize_context
            {
                device_resources*               m_resources;
                gx::geo::geometry_allocator*    m_geometry;
            };
        }
    }

}