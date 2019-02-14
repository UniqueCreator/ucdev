#include "pch.h"

#include "direct_queue_impl.h"
#include "fence_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        void DirectQueue::Impl::InsertWaitOn(const FenceHandle* h)
        {
            auto v = h->GetImpl()->fence();
            m_direct_queue->insert_wait_on(*v);
        }

        void DirectQueue::Impl::WaitForFrence( const FenceHandle* h )
        {
            auto v = h->GetImpl()->fence()->m_value;
            assert(h->GetImpl()->fence()->m_fence == this->m_direct_queue->fence());
            m_direct_queue->wait_for_fence(v);
        }

        void DirectQueue::Impl::WaitForIdleGpu()
        {
            m_direct_queue->wait_for_idle_gpu();
        }

      
    }
}


