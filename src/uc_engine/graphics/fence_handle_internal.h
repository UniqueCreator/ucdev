#include "pch.h"

#include <uc_public/graphics/fence_handle.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandleInternal : public FenceHandle
        {
            public:

            FenceHandleInternal(uc::gx::dx12::gpu_fence f);
        };
    }
}


