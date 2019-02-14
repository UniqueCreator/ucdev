#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Texture2D;

        class Texture2DFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API Texture2DFactory();
            UC_ENGINE_API ~Texture2DFactory();

            UC_ENGINE_API Texture2DFactory(Texture2DFactory&&) = default;
            UC_ENGINE_API Texture2DFactory& operator=(Texture2DFactory&&);

            UC_ENGINE_API std::unique_ptr<Texture2D> CreateFromFile(const wchar_t* fileName);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Texture2DFactory::Impl>;
    }
}


