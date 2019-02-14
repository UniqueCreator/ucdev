#include "pch.h"

#include "fence_handle_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "fence_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        FenceHandleInternal::FenceHandleInternal(uc::gx::dx12::gpu_fence f)
        {
            m_impl = details::pimpl<Impl>(f);
        }
    }
}


