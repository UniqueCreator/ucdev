#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/gpu_virtual_resource.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class PixelBuffer : public GpuVirtualResource
        {
            using Base = GpuVirtualResource;

            public:

            class Impl;

            UC_ENGINE_API PixelBuffer(PixelBuffer&&) = default;
            UC_ENGINE_API PixelBuffer& operator=(PixelBuffer&&);

            UC_ENGINE_API Size2D GetSize2D() const;
            UC_ENGINE_API Size3D GetSize3D() const;

            protected:
            PixelBuffer();
            ~PixelBuffer();
            details::pimpl<Impl> m_impl;

        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<PixelBuffer::Impl>;
    }
}


