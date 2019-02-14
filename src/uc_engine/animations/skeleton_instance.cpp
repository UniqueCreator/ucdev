#include "pch.h"

#include <uc_public/animations/skeleton_instance.h>
#include <uc_public/uc_pimpl_impl.h>

#include "skeleton_instance_impl.h"

namespace UniqueCreator
{
    namespace Animations
    {
        template details::pimpl<SkeletonInstance::Impl>;

        SkeletonInstance::SkeletonInstance()
        {

        }

        SkeletonInstance::~SkeletonInstance() = default;
        SkeletonInstance& SkeletonInstance::operator=(SkeletonInstance&&) = default;

        SkeletonInstance::Impl*   SkeletonInstance::GetImpl()
        {
            return m_impl.get();
        }

        const SkeletonInstance::Impl*   SkeletonInstance::GetImpl() const
        {
            return m_impl.get();
        }

        void SkeletonInstance::Reset()
        {
            GetImpl()->m_skeleton_instance->reset();
        }

        SkeletonInstance::Transforms SkeletonInstance::GetLocalTransforms() const
        {
            SkeletonInstance::Transforms r = {};

            auto si = GetImpl()->m_skeleton_instance.get();

            auto&& t = si->local_transforms();
            r.m_transforms = reinterpret_cast<Simd::Float4x4A*>(&t[0]);
            r.m_transforms_size = t.size();

            return r;
        }

        UC_ENGINE_API SkeletonInstance::Transforms SkeletonInstance::GetConcatenatedTransforms() const
        {
            SkeletonInstance::Transforms r = {};

            auto si = GetImpl();

            auto&& t            = si->m_concatenated_transforms;
            r.m_transforms      = const_cast<Simd::Float4x4A*> (reinterpret_cast<const Simd::Float4x4A*>(&t[0]));
            r.m_transforms_size = t.size();

            return r;
        }

        void SkeletonInstance::ConcatenateLocalTransforms(Simd::Matrix4x4::Param locomotionTransform)
        {
            //todo: remove memory allocations
            uc::math::float4x4 m = uc::math::load44(reinterpret_cast<const float*>(&locomotionTransform));
            GetImpl()->m_concatenated_transforms = GetImpl()->m_skeleton_instance->concatenate_transforms(m);
        }
    }
}
