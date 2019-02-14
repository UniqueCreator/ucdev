#include "pch.h"

#include <uc_public/graphics/upload_queue.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class UploadQueueInternal : public UploadQueue
        {

            public:
            UploadQueueInternal( std::unique_ptr< uc::gx::dx12::gpu_upload_queue > && q );
        };
    }
}


