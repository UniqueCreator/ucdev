#pragma once

#include "upload_queue.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_compute_upload_queue : public gpu_upload_queue
            {
                using base = gpu_upload_queue;

                public:

                gpu_compute_upload_queue( ID3D12Device* d, gpu_resource_create_context* rc, gpu_command_context_allocator* allocator, gpu_command_queue* queue);
                ~gpu_compute_upload_queue();

                protected:
            };
        }
    }
}