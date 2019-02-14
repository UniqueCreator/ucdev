#pragma once

#include <uc_public/math/defines.h>
#include <uc_public/math/matrix4x4.h>
#include <uc_public/math/quaternion.h>

#include <intrin.h>

namespace UniqueCreator
{
    namespace Mathematics
    {
        namespace Graphics
        {
            struct ViewPort
            {
                float m_left;
                float m_top;
                float m_right;
                float m_bottom;
                float m_min_z;
                float m_max_z;
            };

            struct alignas(16) ViewFrustum
            {
                Simd::Float4A m_left;
                Simd::Float4A m_right;
                Simd::Float4A m_top;
                Simd::Float4A m_bottom;
                Simd::Float4A m_near;
                Simd::Float4A m_far;
            };

            inline Matrix4x4 UC_MATH_CALL Translation(Vector4::Param v)
            {
                Matrix4x4 m;

                m.r[0] = Simd::IdentityR0();
                m.r[1] = Simd::IdentityR1();
                m.r[2] = Simd::IdentityR2();
                m.r[3] = Simd::Select(v, Simd::IdentityR3(), Simd::MaskW());

                return m;
            }

            inline Matrix4x4 UC_MATH_CALL Translation(float x, float y, float z)
            {
                using namespace Simd;
                Vector4 v = Set(x, y, z, 1.0f);
                return Translation(v);
            }

            inline Vector4 UC_MATH_CALL Translation(Matrix4x4::Param m)
            {
                using namespace Simd;
                return Select(m.r[3], Zero(), MaskW());
            }

            inline Matrix4x4 UC_MATH_CALL TranslationX(float v)
            {
                using namespace Simd;
                return Translation(v, 0.0f, 0.0f);
            }

            inline Matrix4x4 UC_MATH_CALL TranslationY(float v)
            {
                using namespace Simd;
                return Translation(v, 0.0f, 0.0f);
            }

            inline Matrix4x4 UC_MATH_CALL TranslationZ(float v)
            {
                using namespace Simd;
                return Translation(v, 0.0f, 0.0f);
            }

            inline Matrix4x4 UC_MATH_CALL TranslationXY(float v)
            {
                using namespace Simd;
                return Translation(v, v, 0.0f);
            }

            inline Matrix4x4 UC_MATH_CALL TranslationXZ(float v)
            {
                using namespace Simd;
                return Translation(v, 0.0f, v);
            }

            inline Matrix4x4 UC_MATH_CALL TranslationYZ(float v)
            {
                using namespace Simd;
                return Translation(0.0f, v, v);
            }

            inline Matrix4x4 UC_MATH_CALL Scaling(Vector4::Param v)
            {
                using namespace Simd;
                Matrix4x4 m;

                m.r[0] = And (v, MaskX());
                m.r[1] = And (v, MaskY());
                m.r[2] = And (v, MaskZ());
                m.r[3] = IdentityR3();

                return m;
            }

            inline Matrix4x4 UC_MATH_CALL ScalingX(float x)
            {
                using namespace Simd;
                return Scaling(Set(x, 1.0f, 1.0f, 1.0f));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingY(float y)
            {
                using namespace Simd;
                return Scaling(Set(1.0f, y, 1.0f, 1.0f));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingZ(float z)
            {
                using namespace Simd;
                return Scaling(Set(1.0f, 1.0f, z, 1.0f));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingW(float w)
            {
                using namespace Simd;
                return Scaling(Set(1.0f, 1.0f, 1.0f, w));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingXY(float v)
            {
                using namespace Simd;
                return Scaling(Set(v, v, 1.0f, 1.0f));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingXZ(float v)
            {
                using namespace Simd;
                return Scaling(Set(v, 1.0f, v, 1.0f));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingXW(float v)
            {
                using namespace Simd;
                return Scaling(Set(v, 1.0f, 1.0f, v));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingYZ(float v)
            {
                using namespace Simd;
                return Scaling(Set(1.0f, v, v, 1.0f));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingYW(float v)
            {
                using namespace Simd;
                return Scaling(Set(1.0f, v, 1.0f, v));
            }

            inline Matrix4x4 UC_MATH_CALL ScalingZW(float v)
            {
                using namespace Simd;
                return Scaling(Set(1.0f, 1.0f, v, v));
            }

            inline Matrix4x4 UC_MATH_CALL Scaling(float x, float y, float z)
            {
                using namespace Simd;
                Vector4 v = Set(x, y, z, 1.0f);
                return Scaling(v);
            }

            inline Matrix4x4 UC_MATH_CALL RotationX(float sin_angle, float cos_angle)
            {
                using namespace Simd;
                Matrix4x4 m;

                Vector4 v1 = _mm_set_ss(cos_angle);
                Vector4 v2 = _mm_set_ss(sin_angle);

                Vector4 v3 = Shuffle<Y, X, X, Y>(v1, v2);
                Vector4 v4 = Swizzle<X, Z, Y, W>(v3);
                Vector4 v5 = Set(1.0f, -1.0f, 1.0f, 1.0f); //todo:construct this with instructions
                Vector4 v6 = Mul(v4, v5);

                m.r[0] = IdentityR0();
                m.r[1] = v3;
                m.r[2] = v6;
                m.r[3] = IdentityR3();

                return m;
            }

            inline Matrix4x4 UC_MATH_CALL RotationY(float sin_angle, float cos_angle)
            {
                using namespace Simd;
                Matrix4x4 m;

                Vector4 v1 = _mm_set_ss(cos_angle);
                Vector4 v2 = _mm_set_ss(sin_angle);

                Vector4 v3 = Shuffle<X, Y, X, Y>(v2, v1);
                Vector4 v4 = Swizzle<Z, Y, X, W>(v3);
                Vector4 v5 = Set(1.0f, 1.0f, -1.0f, 1.0f); //todo:construct this with instructions
                Vector4 v6 = Mul(v4, v5);

                m.r[0] = v6;
                m.r[1] = IdentityR1();
                m.r[2] = v3;
                m.r[3] = IdentityR3();

                return m;
            }

            inline Matrix4x4 UC_MATH_CALL RotationZ(float sin_angle, float cos_angle)
            {
                using namespace Simd;
                Matrix4x4 m;

                Vector4 v1 = _mm_set_ss(cos_angle);
                Vector4 v2 = _mm_set_ss(sin_angle);

                Vector4 v3 = _mm_unpacklo_ps(v1, v2);
                Vector4 v4 = Swizzle<Y, X, Z, W>(v3);
                Vector4 v5 = Set(-1.0f, 1.0f, 1.0f, 1.0f); //todo:construct this with instructions
                Vector4 v6 = Mul(v4, v5);

                m.r[0] = v3;
                m.r[1] = v6;
                m.r[2] = IdentityR2();
                m.r[3] = IdentityR3();

                return m;
            }

            inline Matrix4x4 UC_MATH_CALL RotationX(float angle)
            {
                float sin_angle = sinf(angle);
                float cos_angle = cosf(angle);
                return RotationX(sin_angle, cos_angle);
            }

            inline Matrix4x4 UC_MATH_CALL RotationY(float angle)
            {
                float sin_angle = sinf(angle);
                float cos_angle = cosf(angle);
                return RotationY(sin_angle, cos_angle);
            }

            inline Matrix4x4 UC_MATH_CALL RotationZ(float angle)
            {
                float sin_angle = sinf(angle);
                float cos_angle = cosf(angle);
                return RotationZ(sin_angle, cos_angle);
            }

            //creates left hAnded view matrix
            inline Matrix4x4 UC_MATH_CALL View(Vector4::Param eye_position, Vector4::Param eye_direction, Vector4::Param up_direction)
            {
                using namespace Simd;
                //eye_direction And up_direction should be normalized;
                //eye_direction And up_direction w components should be Zero

                Matrix4x4 m1;

                Vector4 v0 = Cross3(up_direction, eye_direction);
                v0 = Normalize3(v0);

                Vector4 v1 = Cross3(eye_direction, v0);
                Vector4 v2 = eye_direction;

                //form the translation
                Vector4 negative_eye_position = Negate(eye_position);

                Vector4 d0 = Dot3(v0, negative_eye_position);
                Vector4 d1 = Dot3(v1, negative_eye_position);
                Vector4 d2 = Dot3(v2, negative_eye_position);

                Vector4  mask = MaskW();
                m1.r[0] = Select(v0, d0, mask);
                m1.r[1] = Select(v1, d1, mask);
                m1.r[2] = Select(v2, d2, mask);
                m1.r[3] = IdentityR3();

                return Transpose(m1);
            }

            //creates left hAnded inverse view matrix
            inline Matrix4x4 UC_MATH_CALL InverseView(Vector4::Param eye_position, Vector4::Param eye_direction, Vector4::Param up_direction)
            {
                using namespace Simd;
                //eye_direction And up_direction should be normalized;
                //eye_direction And up_direction w components should be Zero

                static const uint32_t   __declspec(align(16)) MaskYw[4] = { 0, 0xFFFFFFFF, 0, 0xFFFFFFFF };
                static const uint32_t	__declspec(align(16)) MaskXyz[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0 };

                Matrix4x4 m1;
                Matrix4x4 m2;

                Vector4 v0 = Cross3(up_direction, eye_direction);
                v0 = Normalize3(v0);

                Vector4 v1 = Cross3(eye_direction, v0);
                Vector4 v2 = eye_direction;

                Vector4 position = eye_position;
                Vector4 d0 = Dot3(v0, position);
                Vector4 d1 = Dot3(v1, position);
                Vector4 d2 = Dot3(v2, position);

                Vector4  mask = Load(reinterpret_cast<const Float4A*>(&MaskYw[0]));
                m1.r[0] = v0;
                m1.r[1] = v1;
                m1.r[2] = v2;
                m1.r[3] = IdentityR3();

                Vector4 d0_d2 = Shuffle<X, X, X, X>(d0, d2);
                Vector4 d1_d3 = Shuffle<X, X, W, W>(d1, IdentityR3());

                m2.r[0] = IdentityR0();
                m2.r[1] = IdentityR1();
                m2.r[2] = IdentityR2();
                m2.r[3] = Select(d0_d2, d1_d3, mask);

                return Mul(m2, m1);
            }

            //creates left hAnded view matrix
            inline Matrix4x4 UC_MATH_CALL LookAtLH(Vector4::Param eye_position, Vector4::Param look_at_position, Vector4::Param up_direction)
            {
                using namespace Simd;

                Vector4 v1 = Sub(look_at_position, eye_position);
                Vector4 v2 = Normalize3(v1);
                Vector4 v3 = Normalize2(up_direction);
                return View(eye_position, v2, v3);
            }

            //creates left hAnded inverse view matrix
            inline Matrix4x4 UC_MATH_CALL InverseLookAtLH(Vector4::Param eye_position, Vector4::Param look_at_position, Vector4::Param up_direction)
            {
                using namespace Simd;

                Vector4 v1 = Sub(look_at_position, eye_position);
                Vector4 v2 = Normalize3(v1);
                Vector4 v3 = Normalize2(up_direction);
                return InverseView(eye_position, v2, v3);
            }

            //creates left hAnded perspective projection matrix
            inline Matrix4x4 UC_MATH_CALL PerspectiveLH(float view_width, float view_height, float z_near, float z_far)
            {
                using namespace Simd;

                //static const uint32_t	__declspec(align(16))	MaskYzw[4] = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

                float a = 2.0f * z_near;

                float r = z_far / (z_far - z_near);

                Vector4 v1 = Set(a / view_width, a / view_height, r, -r * z_near);
                Vector4 ze = Zero();

                Matrix4x4 m;

                Vector4 v2 = Shuffle<X, Y, X, Y>(v1, ze);
                m.r[0] = And (v2, MaskX());

                Vector4 v3 = And (v1, Set(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));

                v2 = Swizzle<X, Y, X, X>(v1);
                m.r[1] = And (v2, MaskY());

                Vector4 v4 = Shuffle<Z, W, Z, W>(v3, IdentityR3());

                m.r[2] = Swizzle<Z, Z, X, W>(v4);
                m.r[3] = Swizzle<Z, Z, Y, Z>(v4);

                return m;
            }

            //creates left hAnded inverse perspective projection matrix
            inline Matrix4x4 UC_MATH_CALL InversePerspectiveLH(float view_width, float view_height, float z_near, float z_far)
            {
                using namespace Simd;

                static const uint32_t	__declspec(align(16))	MaskYzw[4] = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
                static const Vector4	IdentityR3 = Set( 0.0f, 0.0f, 0.0f, 1.0f );

                float a = 2.0f / z_near;

                float r = (z_near - z_far) / (z_near * z_far);

                Vector4 v1 = Set(view_width / a, view_height / a, r, 1.0f / z_near);
                Vector4 ze = Zero();

                Matrix4x4 m;

                Vector4 v2 = Shuffle<X, Y, X, Y>(v1, ze);
                m.r[0] = And (v2, MaskX());

                Vector4 v3 = And (v1, reinterpret_cast<const Vector4*> (&MaskYzw)[0]);

                v2 = Swizzle<X, Y, X, X>(v1);
                m.r[1] = And (v2, MaskY());

                Vector4 v4 = Shuffle<Z, W, Z, W>(v3, IdentityR3);

                m.r[2] = Swizzle<Z, Z, Z, X>(v4);
                m.r[3] = Swizzle<Z, Z, W, Y>(v4);

                return m;
            }

            //creates left hAnded perspective projection matrix
            inline Matrix4x4 UC_MATH_CALL PerspectiveFOVLH(float fov, float aspect_ratio, float z_near, float z_far)
            {
                using namespace Simd;

                static const uint32_t	__declspec(align(16))	MaskYzw[4] = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

                float r = z_far / (z_far - z_near);

                float sin = sinf(fov * 0.5f);
                float cos = cosf(fov * 0.5f);
                float height = cos / sin;

                Vector4 v1 = Set(height / aspect_ratio, height, r, -r * z_near);
                Vector4 ze = Zero();

                Matrix4x4 m;

                Vector4 v2 = Shuffle<X, Y, X, Y>(v1, ze);
                m.r[0] = And (v2, MaskX());

                v2 = Swizzle<X, Y, X, X>(v1);
                m.r[1] = And (v2, MaskY());

                Vector4 v3 = And (v1, reinterpret_cast<const Vector4*> (&MaskYzw)[0]);
                Vector4 v4 = Shuffle<Z, W, Z, W>(v3, IdentityR3());

                m.r[2] = Swizzle<Z, Z, X, W>(v4);
                m.r[3] = Swizzle<Z, Z, Y, Z>(v4);

                return m;
            }

            //creates left hAnded inverse perspective projection matrix
            inline Matrix4x4 UC_MATH_CALL InversePerspectiveFOVLH(float fov, float aspect_ratio, float z_near, float z_far)
            {
                using namespace Simd;

                static const uint32_t	__declspec(align(16))	MaskYzw[4] = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

                float r = (z_near - z_far) / (z_near * z_far);

                float sin = sinf(fov * 0.5f);
                float cos = cosf(fov * 0.5f);
                float height = sin / cos;

                Vector4 v1 = Set(height * aspect_ratio, height, r, 1.0f / z_near);
                Vector4 ze = Zero();

                Matrix4x4 m;

                Vector4 v2 = Shuffle<X, Y, X, Y>(v1, ze);
                m.r[0] = And (v2, MaskX());

                Vector4 v3 = And (v1, reinterpret_cast<const Vector4*> (&MaskYzw)[0]);

                v2 = Swizzle<X, Y, X, X>(v1);
                m.r[1] = And (v2, MaskY());

                Vector4 v4 = Shuffle<Z, W, Z, W>(v3, IdentityR3());

                m.r[2] = Swizzle<Z, Z, Z, X>(v4);
                m.r[3] = Swizzle<Z, Z, W, Y>(v4);

                return m;
            }

            //creates left hAnded orthographic projection matrix
            inline Matrix4x4 UC_MATH_CALL OrthographicLH(float view_width, float view_height, float z_near, float z_far)
            {
                using namespace Simd;

                static const uint32_t	__declspec(align(16))	MaskYzw[4] = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

                float a = 2.0f;

                float r = 1.0f / (z_far - z_near);

                Vector4 v1 = Set(a / view_width, a / view_height, r, -r * z_near);
                Vector4 ze = Zero();

                Matrix4x4 m;

                Vector4 v2 = Shuffle<X, Y, X, Y>(v1, ze);
                m.r[0] = And (v2, MaskX());

                v2 = Swizzle<X, Y, X, X>(v1);
                m.r[1] = And (v2, MaskY());

                Vector4 v3 = And (v1, reinterpret_cast<const Vector4*> (&MaskYzw)[0]);
                Vector4 v4 = Shuffle<Z, W, Z, W>(v3, IdentityR3());

                m.r[2] = Swizzle<Z, Z, X, W>(v4);
                m.r[3] = Swizzle<Z, Z, Y, Z>(v4);

                return m;
            }

            //creates left hAnded orthographic projection matrix
            inline Matrix4x4 UC_MATH_CALL InverseOrthographicLH(float view_width, float view_height, float z_near, float z_far)
            {
                using namespace Simd;

                static const uint32_t	__declspec(align(16))	MaskYzw[4] = { 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

                float a = 2.0f;

                float r = (z_far - z_near);

                Vector4 v1 = Set(view_width / a, view_height / a, -r / z_near, 1.0f / z_near);
                Vector4 ze = Zero();

                Matrix4x4 m;

                Vector4 v2 = Shuffle<X, Y, X, Y>(v1, ze);
                m.r[0] = And (v2, MaskX());

                Vector4 v3 = And (v1, reinterpret_cast<const Vector4*> (&MaskYzw)[0]);

                v2 = Swizzle<X, Y, X, X>(v1);
                m.r[1] = And (v2, MaskY());

                Vector4 v4 = Shuffle<Z, W, Z, W>(v3, IdentityR3());

                m.r[2] = Swizzle<Z, Z, Z, X>(v4);
                m.r[3] = Swizzle<Z, Z, W, Y>(v4);

                return m;
            }

            //extracts view frusutm from world view projection matrix
            inline ViewFrustum UC_MATH_CALL ExtractViewFrustum(Matrix4x4::Param wvp)
            {
                using namespace Simd;

                Simd::Vector4 v1;
                Simd::Vector4 v2;
                Simd::Vector4 v3;
                Simd::Vector4 v4;
                Simd::Vector4 v5;
                Simd::Vector4 v6;

                Matrix4x4 m1;

                m1 = Transpose(wvp);

                v1 = Add(wvp.r[3], m1.r[0]);	//left
                v2 = Sub(wvp.r[3], m1.r[0]);	//right
                v3 = Sub(wvp.r[3], m1.r[1]);	//top
                v4 = Add(wvp.r[3], m1.r[0]);	//bottom

                v5 = m1.r[2];				    //near
                v6 = Sub(wvp.r[3], m1.r[2]);	//far

                v1 = NormalizePlane(v1);
                v2 = NormalizePlane(v2);
                v3 = NormalizePlane(v3);
                v4 = NormalizePlane(v4);
                v5 = NormalizePlane(v5);
                v6 = NormalizePlane(v6);

                v1 = Negate(v1);
                v2 = Negate(v2);
                v3 = Negate(v3);
                v4 = Negate(v4);
                v5 = Negate(v5);
                v6 = Negate(v6);

                ViewFrustum r;

                Store(&r.m_left, v1);       //left
                Store(&r.m_right, v2);      //right
                Store(&r.m_top, v3);        //top
                Store(&r.m_bottom, v4);     //bottom
                Store(&r.m_near, v5);       //near
                Store(&r.m_far, v6);        //far
            }

            //transforms a point with a transformation matrix (scale, rotation, translation) , assumes w = 1.0f
            inline Vector4 UC_MATH_CALL Transform3(Vector4::Param v, Matrix4x4::Param m)
            {
                using namespace Simd;
                // assume w component is 1 And Multiply vector * matrix
                Vector4  v1 = Swizzle<X, X, X, X>(v);
                Vector4  v2 = Swizzle<Y, Y, Y, Y>(v);
                Vector4  v3 = Swizzle<Z, Z, Z, Z>(v);

                Vector4  v5 = Mul(v1, m.r[0]);
                Vector4  v6 = Mul(v2, m.r[1]);
                Vector4  v7 = Mad(v3, m.r[2], m.r[3]);

                Vector4  v8 = Add(v5, v6);
                Vector4  v9 = Add(v7, v8);

                return v9;
            }

            //does a perspective transform of a point, what the vertex shader + the gpu does
            inline Vector4 UC_MATH_CALL PerspectiveTransform3(Vector4::Param v, Matrix4x4::Param m)
            {
                using namespace Simd;
                // assume w component of the input is 1 And Multiply vector * matrix
                Vector4  v1 = Swizzle<X, X, X, X>(v);
                Vector4  v2 = Swizzle<Y, Y, Y, Y>(v);
                Vector4  v3 = Swizzle<Z, Z, Z, Z>(v);
                Vector4  v4 = Swizzle<W, W, W, W>(v);

                Vector4  v5 = Mul(v1, m.r[0]);
                Vector4  v6 = Mul(v2, m.r[1]);
                Vector4  v7 = Mad(v3, m.r[2], m.r[3]);

                Vector4  v8 = Add(v5, v6);
                Vector4  v9 = Add(v7, v8);

                //do perspective divide
                Vector4  v10 = Div(v9, v4);

                return v10;
            }

            //projects a point on the screen And applies the view port transform. the result is in window coordinates
            //maps from 3d space -> view port
            inline Vector4 UC_MATH_CALL Project(Vector4::Param v, Matrix4x4::Param wvp, const ViewPort view_port)
            {
                using namespace Simd;
                Vector4  v1 = PerspectiveTransform3(v, wvp);

                float	half_height = (view_port.m_bottom - view_port.m_top) * 0.5f;
                float	half_width  = (view_port.m_right - view_port.m_left) * 0.5f;

                Vector4  scale      = Set(half_width, -half_height, view_port.m_max_z - view_port.m_min_z, 1.0f);
                Vector4  offSet     = Set(view_port.m_left + half_width, view_port.m_top + half_height, view_port.m_min_z, 0.0f);

                return	Mad(v1, scale, offSet);
            }

            //unprojects a point on the screen coordinates, removes the view port transform
            //maps from view port -> 3d space
            inline Vector4 UC_MATH_CALL UnProject(Vector4::Param v, Matrix4x4::Param inverse_wvp, const ViewPort& view_port)
            {
                using namespace Simd;
                float	half_height = (view_port.m_bottom - view_port.m_top) * 0.5f;
                float	half_width  = (view_port.m_right - view_port.m_left) * 0.5f;

                Vector4  scale      = Set(half_width, -half_height, view_port.m_max_z - view_port.m_min_z, 1.0f);
                Vector4  offSet     = Set(view_port.m_left + half_width, view_port.m_top + half_height, view_port.m_min_z, 0.0f);

                //undo the view port transform
                Vector4  v1         = Sub(v, offSet);
                Vector4	v2          = Div(v1, scale);

                Vector4  v3         = PerspectiveTransform3(v2, inverse_wvp);

                return	v3;
            }

            //returns near And far from a projection matrix
            //todo: 
            inline std::tuple<float, float> UC_MATH_CALL ExtractNearFar(Matrix4x4::Param p)
            {
                float a = GetY(p.r[3]);// [2];
                float b = GetY(p.r[2]);// [2];

                //float zf = a / ( 1.0f - b );
                //float zn = - a / b;
                return std::make_tuple<float, float>(-a / b, a / (1.0f - b));
            }

            //performs vector rotation with quaternion
            inline Vector4 UC_MATH_CALL RotateVector3(Vector4::Param v, Quaternion::Param quaternion)
            {
                Vector4 conjugate   = QuaternionConjugate(quaternion);
                Vector4 result_1    = QuaternionMul(conjugate, v);
                Vector4 result_2    = QuaternionMul(result_1, quaternion);
                return  result_2;
            }

            //reflects a vector around a quaternion
            inline Vector4 UC_MATH_CALL ReflectVector3(Vector4::Param v, Quaternion::Param quaternion)
            {
                Vector4 result_1 = QuaternionMul(quaternion, v);
                Vector4 result_2 = QuaternionMul(result_1, quaternion);
                return result_2;
            }

            //creates quaternion that aligns vector s with vector t
            //todo parallel s And t
            inline Vector4 UC_MATH_CALL QuaternionRotateVector3(Vector4::Param s, Quaternion::Param t)
            {
                using namespace Simd;
                Vector4  e = Dot3(s, t);
                Vector4  cross = Cross3(s, t);
                float   v_1 = sqrtf(2.0f * (1 + GetX(e)));

                Vector4 m = Splat(1.0f / v_1);

                Vector4 v = Mul(m, cross);
                Vector4 w = Splat(v_1 / 2.0f);

                return Select(v, w, MaskW());
            }

            //creates matrix that aligns vector s with vector t. usage :  v = Mul (v , m );
            //todo or parallel s And t
            inline Matrix4x4 UC_MATH_CALL MatrixRotateVector3(Vector4::Param s, Vector4::Param t)
            {
                using namespace Simd;
                //  e + hvx^2, hvxvy + vz, hvxvz - vy, 0
                //  hvxvy - vz, e + hvy^2, hvyvz + vx, 0
                //  hvxvz + vy, hvyvz - vx, e + hvz^2, 0
                //  0         , 0         , 0        , 1

                //  v = sxt
                //  e = s.t;
                //  h = 1 / ( 1 + e)


                const Vector4 v = Cross3(s, t);
                const Vector4 e = QuaternionDot(s, t);
                const Vector4 h = Splat(1.0f / (1.0f + GetX(e)));

                Vector4 vx = Mul(h, Mul(v, SplatX(v))); // hvxvx, hvxvy, hvxvz
                Vector4 vy = Mul(h, Mul(v, SplatY(v))); // hvxvy, hvyvy, hvyvz
                Vector4 vz = Mul(h, Mul(v, SplatZ(v))); // hvxvz, hvyvz, hvzvz

                const Vector4 masks = Set(1.0f, -1.0f, 0.0f, 0.0f );

                const Vector4  v_mask_w = MaskW();
                const Vector4  mask = masks;

                Vector4 v_p = Select(v, e, v_mask_w);
                Vector4 v_px = Swizzle<X, X, X, Z>(mask);

                Vector4 v_1 = Swizzle<W, Z, Y, W>(v_p);     //e, vz, vy, ?
                Vector4 v_2 = Swizzle<Z, W, X, W>(v_p);     //vz, e, vx, ?
                Vector4 v_3 = Swizzle<Y, X, W, W>(v_p);     //vy, vx, e, ? 

                v_1 = Mul(v_1, Swizzle<X, X, Y, Z>(mask));  //e, +vz, -vy, 0
                v_2 = Mul(v_2, Swizzle<Y, X, X, Z>(mask));  //-vz, e, +vx, 0
                v_3 = Mul(v_3, Swizzle<X, Y, X, Z>(mask));  //+vy, -vx, e, 0

                vx = Mul(vx, v_px); // hvxvx, hvxvy, hvxvz, 0
                vy = Mul(vy, v_px); // hvxvy, hvyvy, hvyvz, 0
                vz = Mul(vz, v_px); // hvxvz, hvyvz, hvzvz, 0

                Matrix4x4 m;

                m.r[0] = Add(vx, v_1);
                m.r[1] = Add(vy, v_2);
                m.r[2] = Add(vz, v_3);
                m.r[3] = IdentityR3();

                return m;
            }

            //creates a point in 3d
            inline Point3 UC_MATH_CALL MakePoint3(float x, float y, float z)
            {
                return Simd::Set(x, y, z, 1.0f);
            }

            //creates a vector in 3d
            inline Vector3 UC_MATH_CALL MakeVector3(float x, float y, float z)
            {
                return Simd::Set(x, y, z, 0.0f);
            }
        }
    }
}

