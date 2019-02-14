#include "pch.h"

#include <uc_public/graphics/gpu_virtual_resource.h>
#include <uc_public/uc_pimpl_impl.h>

#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<GpuVirtualResource::Impl>;

        GpuVirtualResource::GpuVirtualResource()
        {

        }

        GpuVirtualResource::~GpuVirtualResource() = default;
        GpuVirtualResource& GpuVirtualResource::operator=(GpuVirtualResource&&) = default;

        GpuVirtualResource::Impl*   GpuVirtualResource::GetImpl()
        {
            return m_impl.get();
        }

        const GpuVirtualResource::Impl*   GpuVirtualResource::GetImpl() const
        {
            return m_impl.get();
        }

        GpuVirtualAddress GpuVirtualResource::GetVirtualAddress() const
        {
            GpuVirtualAddress r;
            r.m_value = m_impl->resource()->virtual_address();
            return r;
        }
    }
}


