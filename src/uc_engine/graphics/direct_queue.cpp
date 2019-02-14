#include "pch.h"

#include <uc_public/graphics/direct_queue.h>
#include <uc_public/uc_pimpl_impl.h>

#include "direct_queue_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<DirectQueue::Impl>;

        DirectQueue::DirectQueue()
        {

        }

        DirectQueue::~DirectQueue() = default;
        DirectQueue& DirectQueue::operator=(DirectQueue&&) = default;

        DirectQueue::Impl*   DirectQueue::GetImpl()
        {
            return m_impl.get();
        }

        const DirectQueue::Impl*   DirectQueue::GetImpl() const
        {
            return m_impl.get();
        }

        void DirectQueue::WaitForIdleGpu()
        {
            m_impl->WaitForIdleGpu();
        }

        void DirectQueue::InsertWaitOn(const FenceHandle* f)
        {
            m_impl->InsertWaitOn(f);
        }

        void DirectQueue::WaitForFence( const FenceHandle* f )
        {
            m_impl->WaitForFrence(f);
        }
    }
}


