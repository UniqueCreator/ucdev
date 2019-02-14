#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class SkinnedModel;

        class SkinnedModelFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API SkinnedModelFactory();
            UC_ENGINE_API ~SkinnedModelFactory();

            UC_ENGINE_API SkinnedModelFactory(SkinnedModelFactory&&) = default;
            UC_ENGINE_API SkinnedModelFactory& operator=(SkinnedModelFactory&&);

            UC_ENGINE_API std::unique_ptr<SkinnedModel> CreateFromFile(const wchar_t* fileName);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedModelFactory::Impl>;
    }
}


