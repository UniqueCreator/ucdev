#include "pch.h"

#include "back_buffer_internal.h"

#include <uc_public/graphics/back_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "back_buffer_impl.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        BackBufferInternal::BackBufferInternal(std::unique_ptr<uc::gx::dx12::gpu_back_buffer>&& b )
        {
            m_impl                     = details::pimpl<Impl>(std::move(b));
            GpuVirtualResource::m_impl = details::pimpl<GpuVirtualResource::Impl>(m_impl->buffer());
        }
    }
}


