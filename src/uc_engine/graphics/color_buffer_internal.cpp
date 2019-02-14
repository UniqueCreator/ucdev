#include "pch.h"

#include "color_buffer_internal.h"

#include <uc_public/graphics/back_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "color_buffer_impl.h"
#include "pixel_buffer_impl.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        ViewColorBufferInternal::ViewColorBufferInternal(std::unique_ptr<uc::gx::dx12::gpu_view_color_buffer>&& b )
        {
            m_impl = details::pimpl<Impl>(std::move(b));
            ColorBuffer::m_impl = details::pimpl<ColorBuffer::Impl>(m_impl->buffer());
            PixelBuffer::m_impl = details::pimpl<PixelBuffer::Impl>(m_impl->buffer());
            GpuVirtualResource::m_impl = details::pimpl<GpuVirtualResource::Impl>(m_impl->buffer());
        }
    }
}


