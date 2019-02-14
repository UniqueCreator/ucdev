#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class JointAnimations;

        class JointAnimationsFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API JointAnimationsFactory();
            UC_ENGINE_API ~JointAnimationsFactory();

            UC_ENGINE_API JointAnimationsFactory(JointAnimationsFactory&&) = default;
            UC_ENGINE_API JointAnimationsFactory& operator=(JointAnimationsFactory&&);

            UC_ENGINE_API std::unique_ptr<JointAnimations> CreateFromFile(const wchar_t* fileName);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<JointAnimationsFactory::Impl>;
    }
}


