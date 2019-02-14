#include "pch.h"

#include <uc_public/assets/graphics/texture_2d.h>
#include <uc_public/uc_pimpl_impl.h>

#include "texture_2d_impl.h"

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<Texture2D::Impl>;

        Texture2D::Texture2D()
        {

        }

        Texture2D::~Texture2D() = default;
        Texture2D& Texture2D::operator=(Texture2D&&) = default;

        Texture2D::Impl*   Texture2D::GetImpl()
        {
            return m_impl.get();
        }

        const Texture2D::Impl*   Texture2D::GetImpl() const
        {
            return m_impl.get();
        }

        UC_ENGINE_API const void* Texture2D::GetData() const
        {
            return m_impl->m_texture->data();
        }

        UC_ENGINE_API uint64_t Texture2D::GetDataSize() const
        {
            return m_impl->m_texture->size();
        }

        Texture2D::Description Texture2D::GetDescription() const
        {
            Texture2D::Description r = {};

            auto texture = m_impl->m_texture;
            
            r.m_width = texture->m_width;
            r.m_height = texture->m_height;
            r.m_mip_levels = 1;
            r.m_storage_format = static_cast<StorageFormat>(texture->storage());
            r.m_view_format = static_cast<ViewFormat>(texture->view());

            return r;
        }
    }
}


