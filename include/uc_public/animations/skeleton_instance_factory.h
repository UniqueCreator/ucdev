#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Skeleton;
    }

    namespace Animations
    {
        class SkeletonInstance;

        class SkeletonInstanceFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API SkeletonInstanceFactory();
            UC_ENGINE_API ~SkeletonInstanceFactory();

            UC_ENGINE_API SkeletonInstanceFactory(SkeletonInstanceFactory&&) = default;
            UC_ENGINE_API SkeletonInstanceFactory& operator=(SkeletonInstanceFactory&&);

            UC_ENGINE_API std::unique_ptr<SkeletonInstance> Create(const Assets::Skeleton* skeleton);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkeletonInstanceFactory::Impl>;
    }
}


