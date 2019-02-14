#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/math/matrix4x4.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Skeleton : private NonCopyable
        {
        public:

            struct Transforms final
            {
                Simd::Float4x4A* m_transforms;
                uint64_t         m_transforms_size;
            };


            class Impl;

            UC_ENGINE_API ~Skeleton();

            UC_ENGINE_API Skeleton(Skeleton&&) = default;
            UC_ENGINE_API Skeleton& operator=(Skeleton&&);

            UC_ENGINE_API Transforms GetInverseBindPose() const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            Skeleton();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Skeleton::Impl>;
    }
}


