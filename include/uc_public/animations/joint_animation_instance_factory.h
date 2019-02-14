#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Skeleton;
        class JointAnimations;
    }

    namespace Animations
    {
        class JointAnimationInstance;
        class SkeletonInstance;

        class JointAnimationInstanceFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API JointAnimationInstanceFactory();
            UC_ENGINE_API ~JointAnimationInstanceFactory();

            UC_ENGINE_API JointAnimationInstanceFactory(JointAnimationInstanceFactory&&) = default;
            UC_ENGINE_API JointAnimationInstanceFactory& operator=(JointAnimationInstanceFactory&&);

            UC_ENGINE_API std::unique_ptr<JointAnimationInstance> Create(const Assets::JointAnimations* jointAnimations, const Assets::Skeleton* skeleton, double startTime);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<JointAnimationInstanceFactory::Impl>;
    }
}


