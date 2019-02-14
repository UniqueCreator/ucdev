#include "pch.h"

#include "fence_handle_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        FenceHandle::Impl::Impl()
        {

        }

        FenceHandle::Impl::Impl(uc::gx::dx12::gpu_fence f) : m_fence(f)
        {

        }

        uc::gx::dx12::gpu_fence* FenceHandle::Impl::fence()
        {
            return &m_fence;
        }

        const uc::gx::dx12::gpu_fence* FenceHandle::Impl::fence() const
        {
            return &m_fence;
        }
    }
}


