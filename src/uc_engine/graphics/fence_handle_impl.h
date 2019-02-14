#include "pch.h"

#include <uc_public/graphics/fence_handle.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle::Impl
        {
            public:

            FenceHandle::Impl();

            FenceHandle::Impl(uc::gx::dx12::gpu_fence f);

            const uc::gx::dx12::gpu_fence* fence() const;
            uc::gx::dx12::gpu_fence* fence();

            private:
            uc::gx::dx12::gpu_fence m_fence;
        };
    }
}


