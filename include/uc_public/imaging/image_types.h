#pragma once

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Imaging
    {
        enum class ImageType : uint32_t
        {
            //map to dxgi_formats
            R32G32B32A32_FLOAT              = static_cast<uint32_t>(Graphics::GraphicsFormat::R32G32B32A32_FLOAT),
            R16G16B16A16_FLOAT              = static_cast<uint32_t>(Graphics::GraphicsFormat::R16G16B16A16_FLOAT),
            R16G16B16A16_UNORM              = static_cast<uint32_t>(Graphics::GraphicsFormat::R16G16B16A16_UNORM),
            R8G8B8A8_UNORM                  = static_cast<uint32_t>(Graphics::GraphicsFormat::R8G8B8A8_UNORM),
            B8G8R8A8_UNORM                  = static_cast<uint32_t>(Graphics::GraphicsFormat::B8G8R8A8_UNORM),
            B8G8R8X8_UNORM                  = static_cast<uint32_t>(Graphics::GraphicsFormat::B8G8R8X8_UNORM),


            R10G10B10_XR_BIAS_A2_UNORM      = static_cast<uint32_t>(Graphics::GraphicsFormat::R10G10B10_XR_BIAS_A2_UNORM),
            R10G10B10A2_UNORM               = static_cast<uint32_t>(Graphics::GraphicsFormat::R10G10B10A2_UNORM),

            B5G5R5A1_UNORM                  = static_cast<uint32_t>(Graphics::GraphicsFormat::B5G5R5A1_UNORM),
            B5G6R5_UNORM                    = static_cast<uint32_t>(Graphics::GraphicsFormat::B5G6R5_UNORM),

            R32_FLOAT                       = static_cast<uint32_t>(Graphics::GraphicsFormat::R32_FLOAT),
            R16_FLOAT                       = static_cast<uint32_t>(Graphics::GraphicsFormat::R16_FLOAT),
            R16_UNORM                       = static_cast<uint32_t>(Graphics::GraphicsFormat::R16_UNORM),
            R8_UNORM                        = static_cast<uint32_t>(Graphics::GraphicsFormat::R8_UNORM),

            A8_UNORM                        = static_cast<uint32_t>(Graphics::GraphicsFormat::A8_UNORM),

            R32G32B32_FLOAT                 = static_cast<uint32_t>(Graphics::GraphicsFormat::R32G32B32_FLOAT)
        };
    }
}


