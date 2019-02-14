#pragma once

#include <uc_public/math/vector4.h>
#include <uc_public/math/quaternion.h>
#include <math.h>


namespace UniqueCreator
{
    namespace Mathematics
    {
        struct alignas(16) Quaternion
        {
            Simd::Vector4 m_value;

            Quaternion(Simd::Vector4::Param v) : m_value(v.m_value) {}
            Quaternion(Mathematics::Vector4::Param v) : m_value(v.m_value) {}
            Quaternion(__m128 value) : m_value(value) {}

            operator __m128() const
            {
                return m_value;
            }

            operator Simd::Vector4() const
            {
                return m_value;
            }

            operator Mathematics::Vector4() const
            {
                return m_value;
            }

#if defined(_X64) || defined(_ARM) || defined(_ARM64)
            using Param = Quaternion;
#else
            using Param = const Quaternion&;
#endif
        };

        inline Quaternion   UC_MATH_CALL QuaternionConjugate(Quaternion::Param q1)
        {
            using namespace Simd;
            Quaternion v = Set(-1.0f, -1.0f, -1.0f, 1.0f);
            return Mul(q1, v);
        }

        inline Quaternion UC_MATH_CALL QuaternionNormalize(Quaternion::Param q1)
        {
            return Normalize4(q1);
        }

        inline Quaternion    UC_MATH_CALL QuaternionDot(Quaternion::Param q1, Quaternion::Param q2)
        {
            return Dot4(q1, q2);
        }

        inline Quaternion   UC_MATH_CALL QuaternionInverse(Quaternion::Param q1)
        {
            using namespace Simd;
            Quaternion length = QuaternionDot(q1, q1);
            Quaternion conjugate = QuaternionConjugate(q1);
            Quaternion result = Div(conjugate, length);

            return result;
        }

        inline Quaternion   UC_MATH_CALL QuaternionLength(Quaternion::Param q1)
        {
            return Length4(q1);
        }

        inline Quaternion   UC_MATH_CALL QuaternionMul(Quaternion::Param q, Quaternion::Param r)
        {
            using namespace Simd;
            /*
            t0 = (  + r0q3 + r1q2 - r2q1 + r3q0)
            t1 = (  - r0q2 + r1q3 + r2q0 + r3q1)
            t2 = (  + r0q1 - r1q0 + r2q3 + r3q2)
            t3 = (  - r0q0 - r1q1 - r2q2 + r3q3)
            */

            Simd::Vector4 t;
            Simd::Vector4 q_1;
            Simd::Vector4 q_2;
            Simd::Vector4 q_3;

            const Simd::Vector4 mask_0 = Simd::Set(1.0f, -1.0f,  1.0f, -1.0f);
            const Simd::Vector4 mask_1 = Swizzle<X, X, Y, Y>(mask_0);
            const Simd::Vector4 mask_2 = Swizzle<Y, X, X, Y>(mask_0);

            t = Mul(SplatW(r), q);

            q_1 = Swizzle<W, Z, Y, X>(q);
            q_1 = Mul(mask_0, q_1);
            q_1 = Mul(q_1, SplatX(r));

            q_2 = Swizzle<Z, W, X, Y>(q);
            q_2 = Mul(mask_1, q_2);
            q_2 = Mul(q_2, SplatY(r));

            q_3 = Swizzle<Y, X, W, Z>(q);
            q_3 = Mul(mask_2, q_3);
            q_3 = Mul(q_3, SplatZ(r));

            t = Add(Add(t, q_2), Add(q_1, q_3));

            return t;
        }

        inline Quaternion UC_MATH_CALL QuaternionNormalAngle(Quaternion::Param normal, float angle)
        {
            using namespace Simd;

            float sin_angle = sinf(0.5f * angle);
            float cos_angle = cosf(0.5f * angle);

            Simd::Vector4 n = Select(normal, IdentityR3(), MaskW());
            Simd::Vector4 v = Set(sin_angle, sin_angle, sin_angle, cos_angle);

            return Mul(v, n);
        }

        inline Quaternion UC_MATH_CALL QuaternionAxisAngle(Vector4::Param axis, float angle)
        {
            return QuaternionNormalAngle(Normalize3(axis), angle);
        }
    }
}

