#pragma once

#include <uc_public/graphics/buffer.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class BufferInternal : public Buffer
        {
            public:
            BufferInternal() {}
            BufferInternal
            (
                uc::gx::dx12::managed_gpu_buffer && b
            );

            private:
            uc::gx::dx12::managed_gpu_buffer                        m_buffer;
        };

        class BufferInternal2 : public Buffer
        {
        public:
            BufferInternal2() {}
            BufferInternal2
            (
                uc::gx::dx12::gpu_buffer*
            );
        };
    }
}


