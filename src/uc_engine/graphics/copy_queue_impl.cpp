#include "pch.h"

#include "copy_queue_impl.h"
#include "fence_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        void CopyQueue::Impl::InsertWaitOn(const FenceHandle* h)
        {
            auto v = h->GetImpl()->fence();
            assert(h->GetImpl()->fence()->m_fence == this->m_copy_queue->fence());
            m_copy_queue->insert_wait_on(*v);
        }

        void CopyQueue::Impl::WaitForFrence( const FenceHandle* h )
        {
            auto v = h->GetImpl()->fence()->m_value;
            assert(h->GetImpl()->fence()->m_fence == this->m_copy_queue->fence());
            m_copy_queue->wait_for_fence(v);
        }

        void CopyQueue::Impl::WaitForIdleGpu()
        {
            m_copy_queue->wait_for_idle_gpu();
        }

      
    }
}


