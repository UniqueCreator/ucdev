#pragma once

#include <uc_public/graphics/depth_stencil_buffer.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DepthStencilBufferInternal : public DepthStencilBuffer
        {
            public:

            DepthStencilBufferInternal( std::unique_ptr<uc::gx::dx12::gpu_depth_stencil_buffer>&& b );
        };
    }
}


