#pragma once

#include <cstdint>

#include <uc_dev/math/math_matrix.h>
#include <uc_dev/gx/gx_view_port.h>


namespace uc
{
    namespace gx
    {
        struct view
        {

            math::float4x4 view_matrix() const
            {
                return m_view_matrix;
            }

            math::float4x4 projection_matrix() const
            {
                return m_projection_matrix;
            }

            view_port view_port() const
            {
                return m_view_port;
            }

            float zn() const
            {
                return m_zn;
            }

            float zf() const
            {
                return m_zf;
            }

        private:

            math::float4x4  m_view_matrix = math::identity_matrix();
            math::float4x4  m_projection_matrix = math::identity_matrix();
            view_port       m_view_port;
            float           m_zn = 0.0f;
            float           m_zf = 1.0f;
        };
    }
}

