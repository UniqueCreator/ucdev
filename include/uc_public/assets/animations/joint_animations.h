#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class JointAnimations : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API ~JointAnimations();

            UC_ENGINE_API JointAnimations(JointAnimations&&) = default;
            UC_ENGINE_API JointAnimations& operator=(JointAnimations&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            JointAnimations();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<JointAnimations::Impl>;
    }
}


