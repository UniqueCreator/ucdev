#include "pch.h"

#include "texture_2d_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "texture_2d_impl.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        Texture2DInternal::Texture2DInternal
        (
            uc::gx::dx12::managed_gpu_texture_2d&& b
        )
        {
            m_impl = details::pimpl<Impl>(std::move(b));
            GpuVirtualResource::m_impl = details::pimpl<GpuVirtualResource::Impl>(m_impl->texture());
        }
    }
}


