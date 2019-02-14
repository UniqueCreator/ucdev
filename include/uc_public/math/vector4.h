#pragma once

#include <uc_public/math/defines.h>

#include <intrin.h>

namespace UniqueCreator
{
    namespace Simd
    {
        enum Component
        {
            X = 0,
            Y = 1,
            Z = 2,
            W = 3
        };

        struct Float4
        {
            float m_x;
            float m_y;
            float m_z;
            float m_w;
        };

        struct alignas(16) Float4A : public Float4
        {

        };

        struct alignas(16) Vector4
        {
            __m128 m_value;

            Vector4() = default;
            ~Vector4() = default;

            Vector4(__m128 value) : m_value(value) {}

            operator __m128() const
            {
                return m_value;
            }

            #if defined(_X64) || defined(_ARM) || defined(_ARM64)
            using Param = Vector4;
            #else
            using Param = const Vector4&;
            #endif
        };

        inline Vector4 UC_MATH_CALL Set(float v1, float v2, float v3, float v4)
        {
            return _mm_set_ps(v4, v3, v2, v1);
        }

        inline Vector4 UC_MATH_CALL Set(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
        {
            __m128i v = _mm_set_epi32(v4, v3, v2, v1);
            return reinterpret_cast<__m128*> (&v)[0];
        }

        inline Vector4 UC_MATH_CALL Zero()
        {
            return _mm_setzero_ps();
        }

        inline Vector4 UC_MATH_CALL One()
        {
            return _mm_set_ps1(1.0f);
        }

        inline Vector4 UC_MATH_CALL MinusOne()
        {
            return _mm_set_ps1(-1.0f);
        }

        inline Vector4 UC_MATH_CALL IdentityR0()
        {
            return Set(1.0f, 0.0f, 0.0f, 0.0f);
        }

        inline Vector4  UC_MATH_CALL IdentityR1()
        {
            return Set(0.0f, 1.0f, 0.0f, 0.0f);
        }

        inline Vector4  UC_MATH_CALL IdentityR2()
        {
            return Set(0.0f, 0.0f, 1.0f, 0.0f);
        }

        inline Vector4  UC_MATH_CALL IdentityR3()
        {
            return Set(0.0f, 0.0f, 0.0f, 1.0f);
        }

        inline Vector4 UC_MATH_CALL Load(const Float4* v)
        {
            return _mm_loadu_ps(&v->m_x);
        }

        inline Vector4 UC_MATH_CALL Load(const Float4A* v)
        {
            return _mm_load_ps(&v->m_x);
        }

        inline void UC_MATH_CALL Store(Float4* v, Vector4::Param p)
        {
            _mm_storeu_ps(&v->m_x, p);
        }

        inline void UC_MATH_CALL Store(Float4A* v, Vector4::Param p)
        {
            _mm_store_ps(&v->m_x, p);
        }

        namespace details
        {
            template <uint32_t c> inline float UC_MATH_CALL get_component(Vector4::Param v)
            {
                Vector4 v1 = Swizzle<c, c, c, c>(v);
                float __declspec( align(16) ) f;
                _mm_store_ss(&f, v1);
                return f;
            }
        }

        inline float UC_MATH_CALL GetX(Vector4::Param v)
        {
            return details::get_component<X>(v);
        }

        inline float UC_MATH_CALL GetY(Vector4::Param v)
        {
            return details::get_component<Y>(v);
        }

        inline float UC_MATH_CALL GetZ(Vector4::Param v)
        {
            return details::get_component<Z>(v);
        }

        inline float UC_MATH_CALL GetW(Vector4::Param v)
        {
            return details::get_component<W>(v);
        }

        template <uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4> inline Vector4 UC_MATH_CALL Shuffle(Vector4::Param value1, Vector4::Param value2)
        {
            const uint32_t shuffle_k = _MM_SHUFFLE(v4, v3, v2, v1);
            return _mm_shuffle_ps(value1, value2, shuffle_k);
        }

        template <uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4> inline Vector4 UC_MATH_CALL Swizzle(Vector4::Param value)
        {
            return Shuffle<v1, v2, v3, v4>(value, value);
        }

        inline Vector4 UC_MATH_CALL Splat(float value)
        {
            return _mm_set1_ps(value);
        }

        inline Vector4 UC_MATH_CALL SplatX(Vector4::Param value)
        {
            return Swizzle<X, X, X, X>(value);
        }

        inline Vector4 UC_MATH_CALL SplatY(Vector4::Param value)
        {
            return Swizzle<Y, Y, Y, Y>(value);
        }

        inline Vector4 UC_MATH_CALL SplatZ(Vector4::Param value)
        {
            return Swizzle<Z, Z, Z, Z>(value);
        }

        inline Vector4 UC_MATH_CALL SplatW(Vector4::Param value)
        {
            return Swizzle<W, W, W, W>(value);
        }

        inline Vector4 UC_MATH_CALL Mul(Vector4::Param value0, Vector4::Param value1)
        {
            return _mm_mul_ps(value0, value1);
        }

        //simple math operations
        inline Vector4 UC_MATH_CALL HorizontalAdd(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_hadd_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Sub(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_sub_ps(v1, v2);
        }

        //simple math operations
        inline Vector4 UC_MATH_CALL HorizontalSub(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_hsub_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Add(Vector4::Param value0, Vector4::Param value1)
        {
            return _mm_add_ps(value0, value1);
        }

        inline Vector4 UC_MATH_CALL Mad(Vector4::Param v1, Vector4::Param v2, Vector4::Param v3)
        {
            return Add(Mul(v1, v2), v3);
        }

        inline Vector4 UC_MATH_CALL Div(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_div_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Sqrt(Vector4::Param v)
        {
            return _mm_sqrt_ps(v);
        }

        inline Vector4 UC_MATH_CALL Rcp(Vector4::Param v)
        {
            return _mm_rcp_ps(v);
        }

        inline Vector4 UC_MATH_CALL Rsqrt(Vector4::Param v)
        {
            return _mm_rsqrt_ps(v);
        }

        inline Vector4 UC_MATH_CALL Min(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_min_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Max(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_max_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Negate(Vector4::Param v)
        {
            return Mul(v, MinusOne());
        }

        //simple logical operations
        inline Vector4 UC_MATH_CALL And (Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_and_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL AndNot(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_andnot_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Or(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_or_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Xor(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_xor_ps(v1, v2);
        }

        inline int32_t UC_MATH_CALL MoveMask(Vector4::Param v)
        {
            return _mm_movemask_ps(v);
        }

        inline Vector4 UC_MATH_CALL CompareEqual(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpeq_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareLessThan(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmplt_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareLessEqual(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmple_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareGreaterThan(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpgt_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareGreaterEqual(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpge_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareNotEqual(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpneq_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareNotLessThan(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpnlt_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareNotLessEqual(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpnle_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareNotGreaterThan(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpngt_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL CompareNotGreaterEqual(Vector4::Param v1, Vector4::Param v2)
        {
            return _mm_cmpnge_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Select(Vector4::Param value1, Vector4::Param value2, Vector4::Param control)
        {
            Vector4 v1 = _mm_andnot_ps(control, value1);
            Vector4 v2 = _mm_and_ps(value2, control);
            return _mm_or_ps(v1, v2);
        }

        inline Vector4 UC_MATH_CALL SelectControl(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
        {
            Vector4 v = Set(v1, v2, v3, v4);
            Vector4 z = Zero();
            return CompareGreaterThan(v, z);
        }

        inline Vector4 UC_MATH_CALL MaskX()
        {
            static const uint32_t __declspec(align(16)) mask_x[4] = { 0xFFFFFFFF, 0x0, 0x0, 0x0 };
            return Load(reinterpret_cast<const Float4A*>(&mask_x[0]));
        }

        inline Vector4 UC_MATH_CALL MaskY()
        {
            static const uint32_t __declspec(align(16)) mask_y[4] = { 0x0, 0xFFFFFFFF, 0x0, 0x0 };
            return Load(reinterpret_cast<const Float4A*>(&mask_y[0]));
        }

        inline Vector4 UC_MATH_CALL MaskZ()
        {
            static const uint32_t __declspec(align(16)) mask_z[4] = { 0x0, 0x0, 0xFFFFFFFF, 0x0 };
            return Load(reinterpret_cast<const Float4A*>(&mask_z[0]));
        }

        inline Vector4 UC_MATH_CALL MaskW()
        {
            static const uint32_t __declspec(align(16)) mask_w[4] = { 0x0, 0x0, 0x0, 0xFFFFFFFF };
            return Load(reinterpret_cast<const Float4A*>(&mask_w[0]));
        }
    }

    namespace Mathematics
    {
        struct alignas(16) Vector4
        {
            Simd::Vector4 m_value;

            Vector4() = default;
            Vector4(Simd::Vector4::Param v) : m_value(v.m_value) {}
            Vector4(__m128 value) : m_value(value) {}

            operator __m128() const
            {
                return m_value;
            }

            operator Simd::Vector4() const
            {
                return m_value;
            }

            #if defined(_X64) || defined(_ARM) || defined(_ARM64)
            using Param = Vector4;
            #else
            using Param = const Vector4&;
            #endif
        };

        struct alignas(16) Vector3
        {
            Simd::Vector4 m_value;

            Vector3() = default;
            Vector3(Simd::Vector4::Param v) : m_value(v.m_value) {}
            Vector3(__m128 value) : m_value(value) {}

            operator __m128() const
            {
                return m_value;
            }

            operator Simd::Vector4() const
            {
                return m_value;
            }

#if defined(_X64) || defined(_ARM) || defined(_ARM64)
            using Param = Vector4;
#else
            using Param = const Vector4&;
#endif
        };

        struct alignas(16) Point3
        {
            Simd::Vector4 m_value;

            Point3() = default;
            Point3(Simd::Vector4::Param v) : m_value(v.m_value) {}
            Point3(__m128 value) : m_value(value) {}

            operator __m128() const
            {
                return m_value;
            }

            operator Simd::Vector4() const
            {
                return m_value;
            }

#if defined(_X64) || defined(_ARM) || defined(_ARM64)
            using Param = Vector4;
#else
            using Param = const Vector4&;
#endif
        };

        inline Vector4 UC_MATH_CALL Add(Vector4::Param v1, Vector4::Param v2)
        {
            return Simd::Add(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Sub(Vector4::Param v1, Vector4::Param v2)
        {
            return Simd::Sub(v1, v2);
        }

        inline Vector4 UC_MATH_CALL Abs(Vector4::Param v)
        {
            Vector4 v3 = Simd::Sub(Simd::Zero(), v);
            Vector4 v4 = Simd::Max(v, v3);
            return v4;
        }

        inline Vector4 UC_MATH_CALL Lerp(Vector4::Param v1, Vector4::Param v2, Vector4::Param l)
        {
            using namespace Simd;
            Vector4 a = Sub(v2, v1);
            return Mad(l, a, v2);
        }

        inline Vector4 UC_MATH_CALL Lerp(Vector4::Param v1, Vector4::Param v2, float l)
        {
            using namespace Simd;
            Vector4 s = Splat(l);
            return Lerp(v1, v2, s);
        }

        inline Vector4 UC_MATH_CALL Clamp(Vector4::Param v, Vector4::Param a, Vector4::Param b)
        {
            using namespace Simd;
            Vector4 v1 = Min(v, b);
            Vector4 v2 = Max(v1, a);
            return v2;
        }

        inline Vector4 UC_MATH_CALL Saturate(Vector4::Param v)
        {
            using namespace Simd;
            return Clamp(v, Zero(), One());
        }

        //math functions
        inline Vector4 UC_MATH_CALL Dot2(Vector4::Param v1, Vector4::Param v2)
        {
            using namespace Simd;
            Vector4 v3 = Mul(v1, v2);
            Vector4 v4 = Swizzle<X, X, X, X>(v3);
            Vector4 v5 = Swizzle<Y, Y, Y, Y>(v3);
            return Add(v4, v5);
        }

        inline Vector4 UC_MATH_CALL Dot3(Vector4::Param v1, Vector4::Param v2)
        {
            using namespace Simd;
            Vector4 v3 = Mul(v1, v2);
            Vector4 v4 = Swizzle<X, X, X, X>(v3);
            Vector4 v5 = Swizzle<Y, Y, Y, Y>(v3);
            Vector4 v6 = Swizzle<Z, Z, Z, Z>(v3);
            Vector4 v7 = Add(v4, v5);
            return Add(v6, v7);
        }

        inline Vector4 UC_MATH_CALL Dot4(Vector4::Param v1, Vector4::Param v2)
        {
            using namespace Simd;
            Vector4 v3 = Mul(v1, v2);
            Vector4 v4 = HorizontalAdd(v3, v3);
            Vector4 v5 = HorizontalAdd(v4, v4);
            return v5;
        }

        inline Vector4 UC_MATH_CALL Length2(Vector4::Param v)
        {
            Vector4 d = Dot2(v, v);
            Vector4 l = Simd::Sqrt(d);
            return l;
        }

        inline Vector4 UC_MATH_CALL Length3(Vector4::Param v)
        {
            Vector4 d = Dot3(v, v);
            Vector4 l = Simd::Sqrt(d);
            return l;
        }

        inline Vector4 UC_MATH_CALL Length4(Vector4::Param v)
        {
            Vector4 d = Dot4(v, v);
            Vector4 l = Simd::Sqrt(d);
            return l;
        }

        inline Vector4 UC_MATH_CALL Normalize2(Vector4::Param v)
        {
            Vector4 l = Length2(v);
            Vector4 n = Simd::Div(v, l);
            return n;
        }

        inline Vector4 UC_MATH_CALL Normalize3(Vector4::Param v)
        {
            Vector4 l = Length3(v);
            Vector4 n = Simd::Div(v, l);
            return n;
        }

        inline Vector4 UC_MATH_CALL Normalize4(Vector4::Param v)
        {
            Vector4 l = Length4(v);
            Vector4 n = Simd::Div(v, l);
            return n;
        }

        inline Vector4 UC_MATH_CALL NormalizePlane(Vector4::Param v)
        {
            Vector4 l = Length3(v);
            Vector4 n = Simd::Div(v, l);
            return n;
        }

        inline Vector4 UC_MATH_CALL Cross2(Vector4::Param v1, Vector4::Param v2)
        {
            using namespace Simd;
            Vector4 v3 = Swizzle<X, Y, X, Y>(v2);
            Vector4 v4 = Mul(v1, v3);
            Vector4 v5 = Swizzle<Y, Y, Y, Y>(v4);
            Vector4 v6 = Sub(v4, v5);
            Vector4 v7 = Swizzle<X, X, X, X>(v6);
            return v7;
        }

        inline Vector4 UC_MATH_CALL Cross3(Vector4::Param v1, Vector4::Param v2)
        {
            using namespace Simd;
            Vector4 v3 = Swizzle<Y, Z, X, W>(v1);
            Vector4 v4 = Swizzle<Z, X, Y, W>(v2);
            Vector4 v5 = Mul(v3, v4);

            Vector4 v6 = Swizzle<Z, X, Y, W>(v1);
            Vector4 v7 = Swizzle<Y, Z, X, W>(v2);

            Vector4 v8 = Mul(v6, v7);
            Vector4 v9 = Sub(v5, v8);

            return v9;
        }

        inline Vector4 UC_MATH_CALL Ortho2(Vector4::Param v)
        {
            using namespace Simd;
            Vector4 v3 = Swizzle<Y, X, Z, W>(v);
            Vector4 v4 = Negate(v3);
            return v4;
        }

        inline Vector4 UC_MATH_CALL Ortho4(Vector4::Param v)
        {
            using namespace Simd;
            Vector4 v3 = Swizzle<Y, X, W, Z>(v);
            Vector4 v4 = Negate(v3);
            return v4;
        }
    }
}

