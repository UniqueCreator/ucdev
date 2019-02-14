#include "pch.h"

#include <uc_dev/gx/dx12/cmd/compute_upload_queue.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            gpu_compute_upload_queue::gpu_compute_upload_queue(ID3D12Device* d, gpu_resource_create_context* rc, gpu_command_context_allocator* allocator, gpu_command_queue* q)
                : base(d,rc,allocator,q)
            {

            }

            gpu_compute_upload_queue::~gpu_compute_upload_queue()
            {
                flush();
            }
        }
    }
}

