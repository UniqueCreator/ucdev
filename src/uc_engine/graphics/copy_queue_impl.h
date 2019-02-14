#pragma once

#include <uc_public/graphics/copy_queue.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class CopyQueue::Impl
        {
            std::unique_ptr<uc::gx::dx12::gpu_command_queue> m_copy_queue;

            public:

            CopyQueue::Impl() {}
            
            CopyQueue::Impl(std::unique_ptr<uc::gx::dx12::gpu_command_queue>&& p) : m_copy_queue(std::move(p))
            {

            }

            uc::gx::dx12::gpu_command_queue* queue() const
            {
                return m_copy_queue.get();
            }

            void WaitForFrence(const FenceHandle* v);
            void InsertWaitOn(const FenceHandle* h);

            void WaitForIdleGpu();
        };
    }
}


