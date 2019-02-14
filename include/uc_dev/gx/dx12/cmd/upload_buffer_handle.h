#pragma once

#include <d3d12.h>

#include <uc_dev/mem/ref_counter.h>
#include <uc_dev/mem/intrusive_ptr.h>
#include <uc_dev/gx/dx12/gpu/upload_buffer.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class ref_counted_upload_buffer : public gpu_upload_buffer, public mem::ref_counter< ref_counted_upload_buffer>
            {
                using base = gpu_upload_buffer;

                public:
                ref_counted_upload_buffer() : base(nullptr)
                {

                }

                ref_counted_upload_buffer(ID3D12Resource* resource) : base(resource)
                {

                }
            };

            using upload_buffer_handle = mem::intrusive_ptr< ref_counted_upload_buffer>;
        }
    }
}


