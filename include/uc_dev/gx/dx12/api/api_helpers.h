#pragma once

#include "dxgi_helpers.h"
#include "helpers.h"


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline  Microsoft::WRL::ComPtr<ID3D12Device> create_device_always(IDXGIAdapter* adapter = nullptr, D3D_FEATURE_LEVEL minimumFeatureLevel = D3D_FEATURE_LEVEL_11_0)
            {
                Microsoft::WRL::ComPtr<ID3D12Device> result;

                auto r = D3D12CreateDevice(adapter, minimumFeatureLevel, IID_PPV_ARGS(&result));

                if (r != S_OK) //fall back to the warp device
                {
                    auto factory = create_dxgi_factory4();
                    Microsoft::WRL::ComPtr<IDXGIAdapter1> warp_adapter;
                    throw_if_failed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warp_adapter)));
                    return create_device(warp_adapter.Get(), minimumFeatureLevel);
                }

                return result;
            }
        }
    }
}


