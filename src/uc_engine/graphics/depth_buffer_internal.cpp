#include "pch.h"

#include "depth_buffer_internal.h"

#include <uc_public/graphics/depth_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "depth_buffer_impl.h"
#include "pixel_buffer_impl.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {

        ViewDepthBufferInternal::ViewDepthBufferInternal(std::unique_ptr<uc::gx::dx12::gpu_view_depth_buffer>&& b )
        {
            m_impl = details::pimpl<Impl>(std::move(b));
            PixelBuffer::m_impl = details::pimpl<PixelBuffer::Impl>(m_impl->buffer());
            DepthBuffer::m_impl = details::pimpl<DepthBuffer::Impl>(m_impl->buffer());
            GpuVirtualResource::m_impl = details::pimpl<GpuVirtualResource::Impl>(m_impl->buffer());
        }

    }
}


