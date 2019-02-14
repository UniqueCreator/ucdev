#include "pch.h"

#include <uc_dev/gx/lip/math.h>

namespace uc
{
        namespace lip
        {
            LIP_BEGIN_DEFINE_RTTI(vector2)
            LIP_RTTI_MEMBER(vector2, m_x)
            LIP_RTTI_MEMBER(vector2, m_y)
            LIP_END_DEFINE_RTTI(vector2)

            LIP_BEGIN_DEFINE_RTTI(vector3)
            LIP_RTTI_MEMBER(vector3, m_x)
            LIP_RTTI_MEMBER(vector3, m_y)
            LIP_RTTI_MEMBER(vector3, m_z)
            LIP_END_DEFINE_RTTI(vector3)

            LIP_BEGIN_DEFINE_RTTI(vector4)
            LIP_RTTI_MEMBER(vector4, m_x)
            LIP_RTTI_MEMBER(vector4, m_y)
            LIP_RTTI_MEMBER(vector4, m_z)
            LIP_RTTI_MEMBER(vector4, m_w)
            LIP_END_DEFINE_RTTI(vector4)

            LIP_BEGIN_DEFINE_RTTI(point3)
                LIP_RTTI_MEMBER(point3, m_x)
                LIP_RTTI_MEMBER(point3, m_y)
                LIP_RTTI_MEMBER(point3, m_z)
            LIP_END_DEFINE_RTTI(point3)

            LIP_BEGIN_DEFINE_RTTI(point2)
                LIP_RTTI_MEMBER(point2, m_x)
                LIP_RTTI_MEMBER(point2, m_y)
            LIP_END_DEFINE_RTTI(point2)

            LIP_BEGIN_DEFINE_RTTI(quaternion)
            LIP_RTTI_MEMBER(quaternion, m_x)
            LIP_RTTI_MEMBER(quaternion, m_y)
            LIP_RTTI_MEMBER(quaternion, m_z)
            LIP_RTTI_MEMBER(quaternion, m_w)
            LIP_END_DEFINE_RTTI(quaternion)


            LIP_BEGIN_DEFINE_RTTI(matrix4x4)

                LIP_RTTI_MEMBER(matrix4x4, m_a0)
                LIP_RTTI_MEMBER(matrix4x4, m_a1)
                LIP_RTTI_MEMBER(matrix4x4, m_a2)
                LIP_RTTI_MEMBER(matrix4x4, m_a3)

                LIP_RTTI_MEMBER(matrix4x4, m_b0)
                LIP_RTTI_MEMBER(matrix4x4, m_b1)
                LIP_RTTI_MEMBER(matrix4x4, m_b2)
                LIP_RTTI_MEMBER(matrix4x4, m_b3)

                LIP_RTTI_MEMBER(matrix4x4, m_c0)
                LIP_RTTI_MEMBER(matrix4x4, m_c1)
                LIP_RTTI_MEMBER(matrix4x4, m_c2)
                LIP_RTTI_MEMBER(matrix4x4, m_c3)

                LIP_RTTI_MEMBER(matrix4x4, m_d0)
                LIP_RTTI_MEMBER(matrix4x4, m_d1)
                LIP_RTTI_MEMBER(matrix4x4, m_d2)
                LIP_RTTI_MEMBER(matrix4x4, m_d3)

            LIP_END_DEFINE_RTTI(matrix4x4)
        }
}
