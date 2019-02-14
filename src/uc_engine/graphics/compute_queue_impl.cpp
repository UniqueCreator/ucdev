#include "pch.h"

#include "compute_queue_impl.h"
#include "fence_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        void ComputeQueue::Impl::InsertWaitOn(const FenceHandle* h)
        {
            auto v = h->GetImpl()->fence();
            assert(h->GetImpl()->fence()->m_fence == this->m_compute_queue->fence());
            m_compute_queue->insert_wait_on(*v);
        }

        void ComputeQueue::Impl::WaitForFrence( const FenceHandle* h )
        {
            auto v = h->GetImpl()->fence()->m_value;
            assert(h->GetImpl()->fence()->m_fence == this->m_compute_queue->fence());
            m_compute_queue->wait_for_fence(v);
        }

        void ComputeQueue::Impl::WaitForIdleGpu()
        {
            m_compute_queue->wait_for_idle_gpu();
        }

      
    }
}


