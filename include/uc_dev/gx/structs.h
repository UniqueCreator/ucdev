#pragma once

#include <uc_dev/math/math.h>

namespace uc
{
    namespace gx
    {
        struct position_3d
        {
            float m_x;
            float m_y;
            float m_z;
        };

        struct vector_3d
        {
            float m_x;
            float m_y;
            float m_z;
        };

        inline math::float4 position( position_3d a )
        {
            return math::set(a.m_x, a.m_y, a.m_z, 1.0f);
        }

        inline math::float4 vector( vector_3d a )
        {
            return math::set(a.m_x, a.m_y, a.m_z, 0.0f);
        }
		
		inline math::float4 up()
		{
			return math::set(0.0, 1.0f, 0.0f, 0.0f);
		}

		inline math::float4 left()
		{
			return math::set(1.0, 0.0f, 0.0f, 0.0f);
		}

		inline math::float4 front()
		{
			return math::set(0.0, 0.0f, 1.0f, 0.0f);
		}


		struct rigid_transform
		{
			math::float4 m_rotation_quaternion;
			math::float4 m_translation;
		};



    }
}
