#pragma once

#include <uc_public/graphics/compute_queue.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ComputeQueue::Impl
        {
            std::unique_ptr<uc::gx::dx12::gpu_command_queue> m_compute_queue;

            public:

            ComputeQueue::Impl() {}
            
            ComputeQueue::Impl(std::unique_ptr<uc::gx::dx12::gpu_command_queue>&& p) : m_compute_queue(std::move(p))
            {

            }

            uc::gx::dx12::gpu_command_queue* queue() const
            {
                return m_compute_queue.get();
            }

            void WaitForFrence(const FenceHandle* v);
            void InsertWaitOn(const FenceHandle* h);

            void WaitForIdleGpu();
        };
    }
}


