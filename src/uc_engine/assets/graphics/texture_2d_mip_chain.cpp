#include "pch.h"

#include <uc_public/assets/graphics/texture_2d_mip_chain.h>
#include <uc_public/uc_pimpl_impl.h>

#include "texture_2d_mip_chain_impl.h"

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<Texture2DMipChain::Impl>;

        Texture2DMipChain::Texture2DMipChain()
        {

        }

        Texture2DMipChain::~Texture2DMipChain() = default;
        Texture2DMipChain& Texture2DMipChain::operator=(Texture2DMipChain&&) = default;

        Texture2DMipChain::Impl*   Texture2DMipChain::GetImpl()
        {
            return m_impl.get();
        }

        const Texture2DMipChain::Impl*   Texture2DMipChain::GetImpl() const
        {
            return m_impl.get();
        }

        UC_ENGINE_API const void* Texture2DMipChain::GetData() const
        {
            return m_impl->m_texture->m_levels[0].data();
        }

        UC_ENGINE_API uint64_t Texture2DMipChain::GetDataSize() const
        {
            return m_impl->m_texture->m_levels[0].size();
        }

        Texture2DMipChain::Description Texture2DMipChain::GetDescription() const
        {
            Texture2DMipChain::Description r = {};

            auto texture = m_impl->m_texture;
            
            r.m_width = texture->m_levels[0].m_width;
            r.m_height = texture->m_levels[0].m_height;
            r.m_mip_levels = texture->m_levels[0].m_mip_levels;
            r.m_storage_format = static_cast<StorageFormat>(texture->m_levels[0].storage());
            r.m_view_format = static_cast<ViewFormat>(texture->m_levels[0].view());

            return r;
        }
    }
}


