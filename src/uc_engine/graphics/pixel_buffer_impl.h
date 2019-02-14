#pragma once

#include <uc_public/graphics/gpu_virtual_resource.h>

#include "gpu_virtual_resource_impl.h"
namespace UniqueCreator
{
    namespace Graphics
    {
        class PixelBuffer::Impl
        {

            public:
            Impl()
            {

            }

            PixelBuffer::Impl(uc::gx::dx12::gpu_pixel_buffer* b) : m_buffer(b)
            {

            }

            uc::gx::dx12::gpu_pixel_buffer* buffer() const
            {
                return m_buffer;
            }

            private:
            uc::gx::dx12::gpu_pixel_buffer* m_buffer;
        };
    }
}


