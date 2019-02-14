#include "pch.h"

#include <uc_public/graphics/compute_queue.h>
#include <uc_public/uc_pimpl_impl.h>

#include "compute_queue_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<ComputeQueue::Impl>;

        ComputeQueue::ComputeQueue()
        {

        }

        ComputeQueue::~ComputeQueue() = default;
        ComputeQueue& ComputeQueue::operator=(ComputeQueue&&) = default;

        ComputeQueue::Impl*   ComputeQueue::GetImpl()
        {
            return m_impl.get();
        }

        const ComputeQueue::Impl*   ComputeQueue::GetImpl() const
        {
            return m_impl.get();
        }

        void ComputeQueue::WaitForIdleGpu()
        {
            m_impl->WaitForIdleGpu();
        }

        void ComputeQueue::InsertWaitOn(const FenceHandle* f)
        {
            m_impl->InsertWaitOn(f);
        }

        void ComputeQueue::WaitForFence(const FenceHandle* f)
        {
            m_impl->WaitForFrence(f);
        }
    }
}


