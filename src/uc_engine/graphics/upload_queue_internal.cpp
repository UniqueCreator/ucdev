#include "pch.h"

#include "upload_queue_internal.h"

#include <uc_public/graphics/upload_queue.h>
#include <uc_public/uc_pimpl_impl.h>

#include "upload_queue_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        UploadQueueInternal::UploadQueueInternal( std::unique_ptr< uc::gx::dx12::gpu_upload_queue > && q)
        {
            m_impl = details::pimpl<Impl>(std::move(q));
        }
    }
}


