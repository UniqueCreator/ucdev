#pragma once

#include <uc_public/graphics/direct_queue.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DirectQueueInternal : public DirectQueue
        {
            public:
            DirectQueueInternal(std::unique_ptr<uc::gx::dx12::gpu_command_queue>&& p );
        };
    }
}


