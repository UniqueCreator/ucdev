#pragma once

#include <uc_public/graphics/direct_queue.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DirectQueue::Impl
        {
            std::unique_ptr<uc::gx::dx12::gpu_command_queue> m_direct_queue;

            public:

            DirectQueue::Impl() {}
            
            DirectQueue::Impl(std::unique_ptr<uc::gx::dx12::gpu_command_queue>&& p) : m_direct_queue(std::move(p))
            {

            }

            uc::gx::dx12::gpu_command_queue* queue() const
            {
                return m_direct_queue.get();
            }

            void WaitForFrence(const FenceHandle* v);
            void InsertWaitOn(const FenceHandle* h);

            void WaitForIdleGpu();
        };
    }
}


