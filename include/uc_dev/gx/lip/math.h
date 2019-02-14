#pragma once

#include <uc_dev/lip/lip.h>

namespace uc
{
    namespace lip
    {
        struct vector2
        {
            float m_x;
            float m_y;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::vector2)

        struct vector3
        {
            float m_x;
            float m_y;
            float m_z;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::vector3)

        struct alignas(16) vector4
        {
            float m_x;
            float m_y;
            float m_z;
            float m_w;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::vector4)

        struct point2
        {
            float m_x;
            float m_y;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::point2)

        struct point3
        {
            float m_x;
            float m_y;
            float m_z;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::point3)

        struct point4
        {
            float m_x;
            float m_y;
            float m_z;
            float m_w;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::point4)

        struct alignas(16) quaternion
        {
            float m_x;
            float m_y;
            float m_z;
            float m_w;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::quaternion)

        struct alignas(16) matrix4x4
        {
            float m_a0;
            float m_a1;
            float m_a2;
            float m_a3;
            
            float m_b0;
            float m_b1;
            float m_b2;
            float m_b3;

            float m_c0;
            float m_c1;
            float m_c2;
            float m_c3;

            float m_d0;
            float m_d1;
            float m_d2;
            float m_d3;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::matrix4x4)
        //------------------------------------------------------
        inline bool operator == (const vector2& a, const vector2& b)
        {
            return a.m_x == b.m_x && a.m_y == b.m_y;
        }
        inline bool operator !=(const vector2& a, const vector2& b)
        {
            return a.m_x != b.m_x || a.m_y != b.m_y;
        }
        //------------------------------------------------------
        inline bool operator == (const vector3& a, const vector3& b)
        {
            return a.m_x == b.m_x && a.m_y == b.m_y && a.m_z == b.m_z;
        }

        inline bool operator !=(const vector3& a, const vector3& b)
        {
            return a.m_x != b.m_x || a.m_y != b.m_y || a.m_z != b.m_z;
        }
        //------------------------------------------------------
        inline bool operator == (const vector4& a, const vector4& b)
        {
            return a.m_x == b.m_x && a.m_y == b.m_y && a.m_z == b.m_z && a.m_w == b.m_w;
        }
        inline bool operator !=(const vector4& a, const vector4& b)
        {
            return a.m_x != b.m_x || a.m_y != b.m_y || a.m_z != b.m_z || a.m_w != b.m_w;
        }
        //------------------------------------------------------
        inline bool operator == (const point2& a, const point2& b)
        {
            return a.m_x == b.m_x && a.m_y == b.m_y;
        }
        inline bool operator !=(const point2& a, const point2& b)
        {
            return a.m_x != b.m_x || a.m_y != b.m_y;
        }
        //------------------------------------------------------
        inline bool operator == (const point3& a, const point3& b)
        {
            return a.m_x == b.m_x && a.m_y == b.m_y && a.m_z == b.m_z;
        }
        inline bool operator !=(const point3& a, const point3& b)
        {
            return a.m_x != b.m_x || a.m_y != b.m_y || a.m_z != b.m_z;
        }
        //------------------------------------------------------
        inline bool operator == (const quaternion& a, const quaternion& b)
        {
            return a.m_x == b.m_x && a.m_y == b.m_y && a.m_z == b.m_z && a.m_w == b.m_w;
        }
        inline bool operator !=(const quaternion& a, const quaternion& b)
        {
            return a.m_x != b.m_x || a.m_y != b.m_y || a.m_z != b.m_z || a.m_w != b.m_w;
        }
        //------------------------------------------------------
        inline bool operator == (const matrix4x4& a, const matrix4x4& b)
        {
            return std::memcmp(&a.m_a0, &b.m_a0, sizeof(matrix4x4)) == 0;
        }
        inline bool operator !=(const matrix4x4& a, const matrix4x4& b)
        {
            return std::memcmp(&a.m_a0, &b.m_a0, sizeof(matrix4x4)) != 0;
        }
    }
}
