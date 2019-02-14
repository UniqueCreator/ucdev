#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class JointAnimation : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API ~JointAnimation();

            UC_ENGINE_API JointAnimation(JointAnimation&&) = default;
            UC_ENGINE_API JointAnimation& operator=(JointAnimation&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            JointAnimation();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<JointAnimation::Impl>;
    }
}


