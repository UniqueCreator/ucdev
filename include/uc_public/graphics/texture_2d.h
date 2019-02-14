#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>

#include <uc_public/graphics/gpu_virtual_resource.h>
#include <uc_public/graphics/shader_resource_view.h>
#include <uc_public/graphics/unordered_access_view.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class Texture2D : public GpuVirtualResource
        {
            public:

            class Impl;

            UC_ENGINE_API ~Texture2D();

            UC_ENGINE_API Texture2D(Texture2D&&) = default;
            UC_ENGINE_API Texture2D& operator=(Texture2D&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API const ShaderResourceView*   GetShaderResourceView() const;
            UC_ENGINE_API const UnorderedAccessView*  GetUnorderedAccessView() const;

            protected:
            Texture2D();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Texture2D::Impl>;

        struct RowSlicePitch final
        {
            uint32_t m_RowPitch;
            uint32_t m_SlicePitch;
        };

        class TextureFormatUtils
        {
            public:

            UC_ENGINE_API static RowSlicePitch GetRowSlicePitch( GraphicsFormat f, uint32_t width, uint32_t height );
        };
    }
}


