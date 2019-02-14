#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/math/matrix4x4.h>

namespace UniqueCreator
{
    namespace Animations
    {
        class SkeletonInstance : private NonCopyable
        {
        public:

            struct Transforms final
            {
                Simd::Float4x4A*        m_transforms;
                uint64_t                m_transforms_size;
            };

            class Impl;

            UC_ENGINE_API ~SkeletonInstance();

            UC_ENGINE_API SkeletonInstance(SkeletonInstance&&) = default;
            UC_ENGINE_API SkeletonInstance& operator=(SkeletonInstance&&);

            UC_ENGINE_API void Reset();

            UC_ENGINE_API void ConcatenateLocalTransforms(Simd::Matrix4x4::Param locomotionTransform);

            UC_ENGINE_API Transforms GetLocalTransforms() const;
            UC_ENGINE_API Transforms GetConcatenatedTransforms() const;


            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            SkeletonInstance();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkeletonInstance::Impl>;
    }
}


#pragma once

