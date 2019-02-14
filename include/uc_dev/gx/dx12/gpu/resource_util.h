#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <uc_dev/gx/dx12/api/helpers.h>
#include "virtual_resource.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline uint64_t size( const gpu_virtual_resource* r )
            {
                auto res = r->resource();
                auto d   = get_device(res);
                auto desc = r->desc();
                auto info = d->GetResourceAllocationInfo(1, 1, &desc );
                return info.SizeInBytes;
            }

            inline D3D12_SHADER_RESOURCE_VIEW_DESC raw_view( gpu_virtual_resource* r )
            {
                D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
                srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv_desc.Buffer.FirstElement = 0;
                srv_desc.Format = DXGI_FORMAT_UNKNOWN;
                srv_desc.Buffer.NumElements = static_cast<UINT>(size(r));
                srv_desc.Buffer.StructureByteStride = 1;
                srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
                return srv_desc;
            }
        }
    }
}
