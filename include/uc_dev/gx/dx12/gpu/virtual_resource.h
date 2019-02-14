#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include "resource.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_virtual_resource : public gpu_resource
            {
                using base = gpu_resource;

                public: 

                explicit gpu_virtual_resource( ID3D12Resource* resource ) : base(resource)
                {

                }

                D3D12_GPU_VIRTUAL_ADDRESS virtual_address() const
                {
                    return m_resource->GetGPUVirtualAddress();
                }
            };
        }
    }
}
