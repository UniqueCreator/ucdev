#pragma once

#include <uc_public/graphics/color_buffer.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
	class ViewColorBufferInternal : public ViewColorBuffer
        {
            public:

                ViewColorBufferInternal( std::unique_ptr<uc::gx::dx12::gpu_view_color_buffer>&& b );
        };
    }
}


