#pragma once

#include <cstdint>

#include <uc_dev/math/math_graphics.h>

#include <d3d12.h>

namespace uc
{
    namespace gx
    {
        struct view_port
        {
            inline uint32_t height() const
            {
                return m_height;
            }

            inline uint32_t width() const
            {
                return m_width;
            }

            inline uint32_t left() const
            {
                return m_left;
            }

            inline uint32_t top() const
            {
                return m_top;
            }

            inline float min_z() const
            {
                return m_min_z;
            }

            inline float max_z() const
            {
                return m_max_z;
            }

            inline void set_dimensions(uint32_t width, uint32_t height)
            {
                m_width = static_cast<uint16_t>(width);
                m_height = static_cast<uint16_t>(height);
            }

            inline void set_offset(uint32_t left, uint32_t top)
            {
                m_left = static_cast<uint16_t>(left);
                m_top = static_cast<uint16_t>(top);
            }

            inline void set_depth(float min_z, float max_z)
            {
                m_min_z = min_z;
                m_max_z = max_z;
            }

            operator math::view_port()  const
            {
                const math::view_port v =
                {
                        static_cast<float> (m_left),
                        static_cast<float> (m_top),
                        static_cast<float> (m_left + m_width),
                        static_cast<float> (m_top + m_height),
                                             m_min_z,
                                             m_max_z
                };
                return v;
            }

            operator D3D12_VIEWPORT() const
            {
                D3D12_VIEWPORT view_port;

                view_port.Height = static_cast<float> (height());
                view_port.MaxDepth = max_z();
                view_port.MinDepth = min_z();
                view_port.TopLeftX = static_cast<float> (left());
                view_port.TopLeftY = static_cast<float> (top());
                view_port.Width = static_cast<float> (width());

                return view_port;
            }

            float   m_min_z     = 0.0f;
            float   m_max_z     = 1.0f;

            uint16_t m_left     = 0.0f;
            uint16_t m_top      = 0.0f;

            uint16_t m_width    = 240;
            uint16_t m_height   = 320;
        };
    }
}

