#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/math/math.h>
#include <uc_public/render/perspective_transform_3d.h>
#include <uc_public/render/view_transform_3d.h>

namespace UniqueCreator
{
    namespace Render
    {
        class PinholeCamera : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API PinholeCamera();
            UC_ENGINE_API ~PinholeCamera();

            UC_ENGINE_API PinholeCamera(PinholeCamera&&) = default;
            UC_ENGINE_API PinholeCamera& operator=(PinholeCamera&&);

            UC_ENGINE_API Simd::Vector4 GetPosition() const;
            UC_ENGINE_API Simd::Vector4 GetUp() const;
            UC_ENGINE_API Simd::Vector4 GetForward() const;

            UC_ENGINE_API float GetFieldOfView() const;
            UC_ENGINE_API float GetAspectRatio() const;
            UC_ENGINE_API float GetNearPlane() const;
            UC_ENGINE_API float GetFarPlane() const;

            UC_ENGINE_API void SetPosition(Simd::Vector4::Param v);
            UC_ENGINE_API void SetUp(Simd::Vector4::Param v);
            UC_ENGINE_API void SetForward(Simd::Vector4::Param v);

            UC_ENGINE_API void SetFieldOfView(float v);
            UC_ENGINE_API void SetAspectRatio(float v);
            UC_ENGINE_API void SetNearPlane(float v);
            UC_ENGINE_API void SetFarPlane(float v);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<PinholeCamera::Impl>;

        class PinholeCameraUtils
        {

            public:

            UC_ENGINE_API static ViewTransform3D        ViewTransform(const PinholeCamera* c);
            UC_ENGINE_API static PerspectiveTransform3D PerspectiveTransform(const PinholeCamera* c);

        };
    }
}


