#pragma once

#include <uc_public/graphics/buffer.h>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/dx12/gpu/buffer.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class Buffer::Impl
        {

            public:

            Impl() {}

            Impl(uc::gx::dx12::gpu_buffer* buffer) :
                m_buffer(buffer)
            {

            }

            uc::gx::dx12::gpu_buffer* buffer() const
            {
                return m_buffer;
            }


            uc::gx::dx12::gpu_buffer* m_buffer = nullptr;
        };
    }
}


