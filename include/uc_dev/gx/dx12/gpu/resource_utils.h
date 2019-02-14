#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <uc_dev/gx/dx12/api/dx12_api_helpers.h>
#include "dx12_gpu_virtual_resource.h"
#include "dx12_gpu_texture_2d.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline size_t size( gpu_virtual_resource* r )
            {
                auto res = r->resource();
                auto d   = get_device(res);
                auto desc = r->desc();

                auto info = d->GetResourceAllocationInfo(1, 1, &desc);
                return info.SizeInBytes;
            }

            inline D3D12_SHADER_RESOURCE_VIEW_DESC raw_srv_view( gpu_virtual_resource* r )
            {
                D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
                srv_desc.ViewDimension                   = D3D12_SRV_DIMENSION_BUFFER;
                srv_desc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv_desc.Buffer.FirstElement             = 0;
                srv_desc.Format                          = DXGI_FORMAT_R32_TYPELESS;
                srv_desc.Buffer.NumElements              = static_cast<uint32_t>( size(r) ) / 4;
                srv_desc.Buffer.StructureByteStride      = 0;
                srv_desc.Buffer.Flags                    = D3D12_BUFFER_SRV_FLAG_RAW;
                return srv_desc;
            }

            
            //todo: fix
            inline D3D12_UNORDERED_ACCESS_VIEW_DESC uav_view( gpu_texture_2d*  )
            {
                D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};

                uav_desc.ViewDimension          = D3D12_UAV_DIMENSION_TEXTURE2D;
                uav_desc.Format                 = DXGI_FORMAT_UNKNOWN;
                uav_desc.Texture2D.MipSlice     = 0;
                uav_desc.Texture2D.PlaneSlice   = 0;

                return uav_desc;
            }
        }
    }
}
