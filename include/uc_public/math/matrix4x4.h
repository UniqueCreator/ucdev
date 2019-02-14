#pragma once

#include <uc_public/math/vector4.h>

namespace UniqueCreator
{
    namespace Simd
    {
        struct Float4x4
        {
            union
            {
                struct
                {
                    float m_00, m_01, m_02, m_03;
                    float m_10, m_11, m_12, m_13;
                    float m_20, m_21, m_22, m_23;
                    float m_30, m_31, m_32, m_33;
                } m_values;

                float m[4][4];
            };
        };

        struct alignas(16) Float4x4A : public Float4x4
        {

        };

        struct alignas(16) Matrix4x4
        {
            Simd::Vector4 r[4];

            #if defined(_X64) || defined(_ARM) || defined(_ARM64)
            using Param = Matrix4x4;
            #else
            using Param = const Matrix4x4&;
            #endif
        };

        inline Matrix4x4 Identity()
        {
            Matrix4x4 r;

            r.r[0] = Simd::IdentityR0();
            r.r[1] = Simd::IdentityR1();
            r.r[2] = Simd::IdentityR2();
            r.r[3] = Simd::IdentityR3();

            return r;
        }

        inline Matrix4x4 Load(const Float4x4* address)
        {
            Matrix4x4 r;
            r.r[0] = _mm_loadu_ps(&address->m_values.m_00);
            r.r[1] = _mm_loadu_ps(&address->m_values.m_10);
            r.r[2] = _mm_loadu_ps(&address->m_values.m_20);
            r.r[3] = _mm_loadu_ps(&address->m_values.m_30);
            return r;
        }

        inline Matrix4x4 Load(const Float4x4A* address)
        {
            Matrix4x4 r;
            r.r[0] = _mm_load_ps(&address->m_values.m_00);
            r.r[1] = _mm_load_ps(&address->m_values.m_10);
            r.r[2] = _mm_load_ps(&address->m_values.m_20);
            r.r[3] = _mm_load_ps(&address->m_values.m_30);
            return r;
        }

        inline void Store(Float4x4* address, Matrix4x4::Param p)
        {
            _mm_storeu_ps(&address->m_values.m_00, p.r[0]);
            _mm_storeu_ps(&address->m_values.m_10, p.r[1]);
            _mm_storeu_ps(&address->m_values.m_20, p.r[2]);
            _mm_storeu_ps(&address->m_values.m_30, p.r[3]);
        }

        inline Matrix4x4 Load(Float4x4A* address, Matrix4x4::Param p)
        {
            _mm_store_ps(&address->m_values.m_00, p.r[0]);
            _mm_store_ps(&address->m_values.m_10, p.r[1]);
            _mm_store_ps(&address->m_values.m_20, p.r[2]);
            _mm_store_ps(&address->m_values.m_30, p.r[3]);
        }

        inline Vector4 Mul(Vector4::Param v, Matrix4x4::Param m)
        {
            using namespace Simd;
            Simd::Vector4  v1 = Swizzle<X, X, X, X>(v);
            Simd::Vector4  v2 = Swizzle<Y, Y, Y, Y>(v);
            Simd::Vector4  v3 = Swizzle<Z, Z, Z, Z>(v);
            Simd::Vector4  v4 = Swizzle<W, W, W, W>(v);

            Simd::Vector4  v5 = Simd::Mul(v1, m.r[0]);
            Simd::Vector4  v6 = Simd::Mul(v2, m.r[1]);
            Simd::Vector4  v7 = Simd::Mul(v3, m.r[2]);
            Simd::Vector4  v8 = Simd::Mul(v4, m.r[3]);

            Simd::Vector4  v9 = Simd::Add(v5, v6);
            Simd::Vector4  v10 = Simd::Add(v7, v8);
            Simd::Vector4  v11 = Simd::Add(v9, v10);

            return v11;
        }

        inline Matrix4x4 Transpose(Matrix4x4::Param m)
        {
            Matrix4x4       m1;

            Simd::Vector4 v1 = Simd::Shuffle<Simd::X, Simd::Y, Simd::X, Simd::Y>(m.r[0], m.r[1]);
            Simd::Vector4 v2 = Simd::Shuffle<Simd::X, Simd::Y, Simd::X, Simd::Y>(m.r[2], m.r[3]);
            Simd::Vector4 v3 = Simd::Shuffle<Simd::Z, Simd::W, Simd::Z, Simd::W>(m.r[0], m.r[1]);
            Simd::Vector4 v4 = Simd::Shuffle<Simd::Z, Simd::W, Simd::Z, Simd::W>(m.r[2], m.r[3]);

            m1.r[0] = Simd::Shuffle<Simd::X, Simd::Z, Simd::X, Simd::Z>(v1, v2);
            m1.r[1] = Simd::Shuffle<Simd::Y, Simd::W, Simd::Y, Simd::W>(v1, v2);
            m1.r[2] = Simd::Shuffle<Simd::X, Simd::Z, Simd::X, Simd::Z>(v3, v4);
            m1.r[3] = Simd::Shuffle<Simd::Y, Simd::W, Simd::Y, Simd::W>(v3, v4);

            return m1;
        }

        inline Vector4 Mul(Matrix4x4::Param m, Vector4::Param v)
        {
            Matrix4x4 t = Transpose(m);
            return Mul( v, t);
        }

        inline Matrix4x4 Mul(Matrix4x4::Param m0, Matrix4x4::Param m1)
        {
            Matrix4x4 m;

            m.r[0] = Mul(m0.r[0], m1);
            m.r[1] = Mul(m0.r[1], m1);
            m.r[2] = Mul(m0.r[2], m1);
            m.r[3] = Mul(m0.r[3], m1);

            return m;
        }
    }

    namespace Mathematics
    {
        using Matrix4x4 = Simd::Matrix4x4;
    }
}

