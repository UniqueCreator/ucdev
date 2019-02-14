#pragma once

#include <memory>
#include <uc_dev/gx/dx12/dx12.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class UploadQueue::Impl
        {
            public:

            UploadQueue::Impl()
            {
                
            }

            UploadQueue::Impl(std::unique_ptr< uc::gx::dx12::gpu_upload_queue >&& queue) : m_queue(std::move(queue))
            {

            }

            std::unique_ptr< uc::gx::dx12::gpu_upload_queue > m_queue;

            uc::gx::dx12::gpu_upload_queue* queue() const
            {
                return m_queue.get();
            }
        };
    }
}


