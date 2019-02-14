#include "pch.h"

#include <uc_public/graphics/texture_2d.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/gx/dx12/gpu/pixel_format.h>

#include "texture_2d_impl.h"

namespace UniqueCreator
{
    namespace Graphics
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

        const ShaderResourceView* Texture2D::GetShaderResourceView() const
        {
            return m_impl->GetShaderResourceView();
        }

        const UnorderedAccessView* Texture2D::GetUnorderedAccessView() const
        {
            return m_impl->GetUnorderedAccessView();
        }

        RowSlicePitch TextureFormatUtils::GetRowSlicePitch( GraphicsFormat f, uint32_t width, uint32_t height )
        {
            uint32_t row_pitch   = 0;
            uint32_t slice_pitch = 0;

            uc::gx::dx12::row_slice_pitch(static_cast<DXGI_FORMAT>(f), width, height, row_pitch, slice_pitch);
            return {row_pitch, slice_pitch};
        }
    }
}


