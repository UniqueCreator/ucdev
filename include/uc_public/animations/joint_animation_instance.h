#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Animations
    {
        class SkeletonInstance; 

        class JointAnimationInstance : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API ~JointAnimationInstance();

            UC_ENGINE_API JointAnimationInstance(JointAnimationInstance&&) = default;
            UC_ENGINE_API JointAnimationInstance& operator=(JointAnimationInstance&&);

            UC_ENGINE_API void Accumulate(SkeletonInstance* s, double time );

            UC_ENGINE_API void Reset();

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            JointAnimationInstance();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<JointAnimationInstance::Impl>;
    }
}


