#pragma once

#include <uc_public/graphics/copy_queue.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class CopyQueueInternal : public CopyQueue
        {
            public:
            CopyQueueInternal(std::unique_ptr<uc::gx::dx12::gpu_command_queue>&& p );
        };
    }
}


