#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Skeleton;

        class SkeletonFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API SkeletonFactory();
            UC_ENGINE_API ~SkeletonFactory();

            UC_ENGINE_API SkeletonFactory(SkeletonFactory&&) = default;
            UC_ENGINE_API SkeletonFactory& operator=(SkeletonFactory&&);

            UC_ENGINE_API std::unique_ptr<Skeleton> CreateFromFile(const wchar_t* fileName);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkeletonFactory::Impl>;
    }
}


