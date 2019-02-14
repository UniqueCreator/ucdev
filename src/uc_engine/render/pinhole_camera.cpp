#include "pch.h"

#include <uc_public/render/pinhole_camera.h>
#include <uc_public/uc_pimpl_impl.h>


#include <uc_public/math/graphics.h>
#include "pinhole_camera_impl.h"

namespace UniqueCreator
{
    namespace Render
    {
        template details::pimpl<PinholeCamera::Impl>;

        PinholeCamera::PinholeCamera()
        {

        }

        PinholeCamera::~PinholeCamera() = default;
        PinholeCamera& PinholeCamera::operator=(PinholeCamera&&) = default;

        PinholeCamera::Impl*   PinholeCamera::GetImpl()
        {
            return m_impl.get();
        }

        const PinholeCamera::Impl*   PinholeCamera::GetImpl() const
        {
            return m_impl.get();
        }

        Simd::Vector4 PinholeCamera::GetPosition() const
        {
            return Simd::Load(&m_impl->m_view_position_ws);
        }

        Simd::Vector4 PinholeCamera::GetUp() const
        {
            return Simd::Load(&m_impl->m_up_ws);
        }

        Simd::Vector4 PinholeCamera::GetForward() const
        {
            return Simd::Load(&m_impl->m_forward_ws);
        }

        float PinholeCamera::GetFieldOfView() const
        {
            return m_impl->m_fov;
        }

        float PinholeCamera::GetAspectRatio() const
        {
            return m_impl->m_aspect_ratio;
        }

        float PinholeCamera::GetNearPlane() const
        {
            return m_impl->m_near;
        }

        float PinholeCamera::GetFarPlane() const
        {
            return m_impl->m_far;
        }

        void PinholeCamera::SetPosition(Simd::Vector4::Param v)
        {
            Simd::Store(&m_impl->m_view_position_ws, v);
        }

        void PinholeCamera::SetUp(Simd::Vector4::Param v)
        {
            Simd::Store(&m_impl->m_up_ws, v);
        }

        void PinholeCamera::SetForward(Simd::Vector4::Param v)
        {
            Simd::Store(&m_impl->m_forward_ws, v);
        }

        void PinholeCamera::SetFieldOfView(float v)
        {
            m_impl->m_fov = v;
        }

        void PinholeCamera::SetAspectRatio(float v)
        {
            m_impl->m_aspect_ratio = v;
        }

        void PinholeCamera::SetNearPlane(float v)
        {
            m_impl->m_near = v;
        }
    
        void PinholeCamera::SetFarPlane(float v)
        {
            m_impl->m_far = v;
        }

        ViewTransform3D PinholeCameraUtils::ViewTransform(const PinholeCamera* c)
        {
            ViewTransform3D r ;

            auto impl = c->GetImpl();

            Simd::Vector4 pos     = Simd::Load(&impl->m_view_position_ws);
            Simd::Vector4 up      = Simd::Load(&impl->m_up_ws);
            Simd::Vector4 forward = Simd::Load(&impl->m_forward_ws);

            return Mathematics::Graphics::View(pos, forward, up);
        }

        PerspectiveTransform3D PinholeCameraUtils::PerspectiveTransform(const PinholeCamera* c)
        {
            PerspectiveTransform3D r;

            auto impl = c->GetImpl();

            return Mathematics::Graphics::PerspectiveFOVLH(impl->m_fov, impl->m_aspect_ratio, impl->m_near, impl->m_far);
        }

    }
}
