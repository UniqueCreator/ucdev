#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/assets/graphics/texture_2d.h>


namespace UniqueCreator
{
    namespace Assets
    {
        class Texture2DMipChain : private NonCopyable
        {
            public:

            struct Description final
            {
                uint16_t      m_width;
                uint16_t      m_height;
                uint16_t      m_mip_levels;
                StorageFormat m_storage_format;
                ViewFormat    m_view_format;
            };

            class Impl;

            UC_ENGINE_API ~Texture2DMipChain();

            UC_ENGINE_API Texture2DMipChain(Texture2DMipChain&&) = default;
            UC_ENGINE_API Texture2DMipChain& operator=(Texture2DMipChain&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API const void* GetData() const;
            UC_ENGINE_API uint64_t    GetDataSize() const;
            UC_ENGINE_API Description GetDescription() const;

        protected:
            Texture2DMipChain();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Texture2DMipChain::Impl>;
    }
}


