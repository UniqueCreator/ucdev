#pragma once

#include <uc_dev/gx/dx12/api/error.h>

#include "virtual_resource.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_upload_buffer : public gpu_virtual_resource
            {

            private:

                using base = gpu_virtual_resource;

            public:

                gpu_upload_buffer( ID3D12Resource* resource ) : base(resource)
                {
                    resource->SetName(L"GPU Upload Buffer");
                }

            public:

                void* map( uint32_t offset = 0 )
                {
                    D3D12_RESOURCE_DESC d;
                    d = m_resource->GetDesc();

                    D3D12_RANGE range = {};

                    range.Begin = offset;
                    range.End   = static_cast<SIZE_T>(d.Width - offset);

                    void* r;
                    throw_if_failed(m_resource->Map(0, &range, &r));
                    return r;
                }

                void unmap( uint32_t offset = 0 )
                {
                    D3D12_RESOURCE_DESC d;
                    d = m_resource->GetDesc();

                    D3D12_RANGE range = {};

                    range.Begin = offset;
                    range.End = static_cast<SIZE_T>(d.Width - offset);

                    m_resource->Unmap(0, &range);
                }
            };
        }
    }
}
