#include "pch.h"

#include <uc_public/assets/animations/skeleton.h>
#include <uc_public/uc_pimpl_impl.h>

#include "skeleton_impl.h"

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<Skeleton::Impl>;

        Skeleton::Skeleton()
        {

        }

        Skeleton::~Skeleton() = default;
        Skeleton& Skeleton::operator=(Skeleton&&) = default;

        Skeleton::Impl*   Skeleton::GetImpl()
        {
            return m_impl.get();
        }

        const Skeleton::Impl*   Skeleton::GetImpl() const
        {
            return m_impl.get();
        }

        Skeleton::Transforms Skeleton::GetInverseBindPose() const
        {
            Skeleton::Transforms r = {};

            auto si = GetImpl();

            auto&& t = si->m_skeleton->m_joint_inverse_bind_pose2;
            r.m_transforms = const_cast<Simd::Float4x4A*> (reinterpret_cast<const Simd::Float4x4A*>(&t[0]));
            r.m_transforms_size = t.size();

            return r;
        }
    }
}
