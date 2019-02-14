#include "pch.h"

#include <uc_public/graphics/depth_stencil_buffer.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DepthStencilBuffer::Impl
        {
            public:

            DepthStencilBuffer::Impl()
            {

            }

            DepthStencilBuffer::Impl(std::unique_ptr<uc::gx::dx12::gpu_depth_stencil_buffer>&& b ) : m_buffer(std::move(b))
            {

            }

            uc::gx::dx12::gpu_depth_stencil_buffer* buffer() const
            {
                return m_buffer.get();
            }

            private:
            std::unique_ptr< uc::gx::dx12::gpu_depth_stencil_buffer > m_buffer;
        };
    }
}


