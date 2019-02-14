#pragma once

#include "upload_buffer.h"
#include "resource_create_context.h"
#include "map_guard.h"


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            gpu_upload_buffer* create_upload_buffer(gpu_resource_create_context* c, size_t size, const void* initial_data)
            {
                //todo: fix unique ptr
                std::unique_ptr<gpu_upload_buffer> b(c->create_upload_buffer(size) );
                gpu_map_guard<gpu_upload_buffer> guard(b.get());
                memcpy(guard.cpu_address(), initial_data, size);
                return b.release();
            }
        }
    }
}
