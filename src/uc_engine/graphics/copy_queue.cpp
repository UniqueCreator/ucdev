#include "pch.h"

#include <uc_public/graphics/copy_queue.h>
#include <uc_public/uc_pimpl_impl.h>

#include "copy_queue_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<CopyQueue::Impl>;

        CopyQueue::CopyQueue()
        {

        }

        CopyQueue::~CopyQueue() = default;
        CopyQueue& CopyQueue::operator=(CopyQueue&&) = default;

        CopyQueue::Impl*   CopyQueue::GetImpl()
        {
            return m_impl.get();
        }

        const CopyQueue::Impl*   CopyQueue::GetImpl() const
        {
            return m_impl.get();
        }

        void CopyQueue::WaitForIdleGpu()
        {
            m_impl->WaitForIdleGpu();
        }

        void CopyQueue::InsertWaitOn(const FenceHandle* f)
        {
            m_impl->InsertWaitOn(f);
        }

        void CopyQueue::WaitForFence( const FenceHandle* f )
        {
            m_impl->WaitForFrence(f);
        }
    }
}


