#pragma once

#include <uc_public/graphics/back_buffer.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class BackBufferInternal : public BackBuffer
        {
            public:

            BackBufferInternal( std::unique_ptr<uc::gx::dx12::gpu_back_buffer>&& b );



        };
    }
}


