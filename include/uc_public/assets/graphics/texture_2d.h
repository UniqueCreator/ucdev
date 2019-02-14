#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Assets
    {
        enum class StorageFormat : uint32_t
        {
            R32G32B32A32_FLOAT  = static_cast<uint32_t>(Graphics::GraphicsFormat::R32G32B32A32_FLOAT),
            R32G32B32_FLOAT     = static_cast<uint32_t>(Graphics::GraphicsFormat::R32G32B32_FLOAT),
            R16G16B16A16_FLOAT  = static_cast<uint32_t>(Graphics::GraphicsFormat::R16G16B16A16_FLOAT),
            R16G16B16A16_UNORM  = static_cast<uint32_t>(Graphics::GraphicsFormat::R16G16B16A16_UNORM),
            R10G10B10A2_UNORM   = static_cast<uint32_t>(Graphics::GraphicsFormat::R10G10B10A2_UNORM),
            R8G8B8A8_UNORM      = static_cast<uint32_t>(Graphics::GraphicsFormat::R8G8B8A8_UNORM),
            R32_FLOAT           = static_cast<uint32_t>(Graphics::GraphicsFormat::R32_FLOAT),
            R16_FLOAT           = static_cast<uint32_t>(Graphics::GraphicsFormat::R16_FLOAT),
            R16_UNORM           = static_cast<uint32_t>(Graphics::GraphicsFormat::R16_UNORM),
            R8_UNORM            = static_cast<uint32_t>(Graphics::GraphicsFormat::R8_UNORM),
            A8_UNORM            = static_cast<uint32_t>(Graphics::GraphicsFormat::A8_UNORM),

            BC1_TYPELESS        = static_cast<uint32_t>(Graphics::GraphicsFormat::BC1_TYPELESS),
            BC2_TYPELESS        = static_cast<uint32_t>(Graphics::GraphicsFormat::BC2_TYPELESS),
            BC3_TYPELESS        = static_cast<uint32_t>(Graphics::GraphicsFormat::BC3_TYPELESS),
            BC4_TYPELESS        = static_cast<uint32_t>(Graphics::GraphicsFormat::BC4_TYPELESS),
            BC5_TYPELESS        = static_cast<uint32_t>(Graphics::GraphicsFormat::BC5_TYPELESS),

            B5G6R5_UNORM        = static_cast<uint32_t>(Graphics::GraphicsFormat::B5G6R5_UNORM),
            B5G5R5A1_UNORM      = static_cast<uint32_t>(Graphics::GraphicsFormat::B5G5R5A1_UNORM),
            B8G8R8A8_UNORM      = static_cast<uint32_t>(Graphics::GraphicsFormat::B8G8R8A8_UNORM),
            B8G8R8X8_UNORM      = static_cast<uint32_t>(Graphics::GraphicsFormat::B8G8R8X8_UNORM),
            R10G10B10_XR_BIAS_A2_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R10G10B10_XR_BIAS_A2_UNORM)
        };

        enum class ViewFormat : uint32_t
        {
            R32G32B32A32_FLOAT = static_cast<uint32_t>(Graphics::GraphicsFormat::R32G32B32A32_FLOAT),
            R32G32B32_FLOAT = static_cast<uint32_t>(Graphics::GraphicsFormat::R32G32B32_FLOAT),
            R16G16B16A16_FLOAT = static_cast<uint32_t>(Graphics::GraphicsFormat::R16G16B16A16_FLOAT),
            R16G16B16A16_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R16G16B16A16_UNORM),
            R10G10B10A2_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R10G10B10A2_UNORM),
            R8G8B8A8_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R8G8B8A8_UNORM),
            R32_FLOAT = static_cast<uint32_t>(Graphics::GraphicsFormat::R32_FLOAT),
            R16_FLOAT = static_cast<uint32_t>(Graphics::GraphicsFormat::R16_FLOAT),
            R16_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R16_UNORM),
            R8_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R8_UNORM),
            A8_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::A8_UNORM),

            BC1_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC1_UNORM),
            BC1_UNORM_SRGB = static_cast<uint32_t>(Graphics::GraphicsFormat::BC1_UNORM_SRGB),

            BC2_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC2_UNORM),
            BC2_UNORM_SRGB = static_cast<uint32_t>(Graphics::GraphicsFormat::BC2_UNORM_SRGB),

            BC3_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC3_UNORM),
            BC3_UNORM_SRGB = static_cast<uint32_t>(Graphics::GraphicsFormat::BC3_UNORM_SRGB),

            BC4_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC4_UNORM),
            BC4_SNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC4_SNORM),

            BC5_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC5_UNORM),
            BC5_SNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::BC5_SNORM),

            B5G6R5_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::B5G6R5_UNORM),
            B5G5R5A1_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::B5G5R5A1_UNORM),
            B8G8R8A8_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::B8G8R8A8_UNORM),
            B8G8R8X8_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::B8G8R8X8_UNORM),
            R10G10B10_XR_BIAS_A2_UNORM = static_cast<uint32_t>(Graphics::GraphicsFormat::R10G10B10_XR_BIAS_A2_UNORM)
        };

        class Texture2D : private NonCopyable
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

            UC_ENGINE_API ~Texture2D();

            UC_ENGINE_API Texture2D(Texture2D&&) = default;
            UC_ENGINE_API Texture2D& operator=(Texture2D&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API const void* GetData() const;
            UC_ENGINE_API uint64_t    GetDataSize() const;
            UC_ENGINE_API Description GetDescription() const;

        protected:
            Texture2D();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Texture2D::Impl>;
    }
}


