#include "pch.h"

#include "buffer_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "buffer_impl.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        BufferInternal::BufferInternal
        (
            uc::gx::dx12::managed_gpu_buffer&& b
        ) : m_buffer( std::move(b) )
        {
            m_impl = details::pimpl<Impl>(m_buffer.get());
            GpuVirtualResource::m_impl = details::pimpl<GpuVirtualResource::Impl>(m_buffer.get());
        }

        BufferInternal2::BufferInternal2
        (
            uc::gx::dx12::gpu_buffer* b
        ) 
        {
            m_impl = details::pimpl<Impl>(b);
            GpuVirtualResource::m_impl = details::pimpl<GpuVirtualResource::Impl>(b);
        }
    }
}


