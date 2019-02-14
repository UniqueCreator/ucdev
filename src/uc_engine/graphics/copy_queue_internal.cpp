#include "pch.h"

#include "copy_queue_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "copy_queue_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        CopyQueueInternal::CopyQueueInternal(std::unique_ptr<uc::gx::dx12::gpu_command_queue>&& p)
        {
            m_impl = details::pimpl<Impl>(std::move(p));
        }
    }
}


