#pragma once

#include <uc_public/graphics/depth_buffer.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ViewDepthBufferInternal : public ViewDepthBuffer
        {
            public:
            ViewDepthBufferInternal( std::unique_ptr<uc::gx::dx12::gpu_view_depth_buffer>&& b );
        };

    }
}


