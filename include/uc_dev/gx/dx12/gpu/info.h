#pragma once

#include <dxgi1_4.h>
#include <cstdint>
#include <wrl/client.h>
#include <vector>

#include <uc_dev/gx/dx12/api/helpers.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline std::vector< Microsoft::WRL::ComPtr<IDXGIAdapter1> > get_adapters()
            {
                auto factory = create_dxgi_factory1();

                Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

                std::vector< Microsoft::WRL::ComPtr<IDXGIAdapter1>  >  adapters;

                uint32_t i = 0;
                while (factory->EnumAdapters1(i++, &adapter) != DXGI_ERROR_NOT_FOUND)
                {
                    adapters.push_back(adapter);
                }

                return adapters;
            }

            inline std::vector< Microsoft::WRL::ComPtr<IDXGIAdapter1> > get_warp_adapters()
            {
                auto factory = create_dxgi_factory4();

                Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

                std::vector< Microsoft::WRL::ComPtr<IDXGIAdapter1>  >  adapters;

                throw_if_failed(factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)));
                adapters.push_back(adapter);

                return adapters;
            }

            inline std::vector<DXGI_ADAPTER_DESC1> get_adapters_description()
            {
                auto&& info = get_adapters();
                std::vector< DXGI_ADAPTER_DESC1 > desc;

                for (auto i : info)
                {
                    DXGI_ADAPTER_DESC1 d;
                    throw_if_failed(i->GetDesc1(&d));
                    desc.emplace_back(std::move(d));
                }
                return desc;
            }

            namespace details
            {
                inline bool operator==(const LUID& a, const LUID& b)
                {
                    return a.HighPart == b.HighPart && a.LowPart == b.LowPart;
                }
            }

            inline DXGI_ADAPTER_DESC1 get_adapter_description(LUID adapter)
            {
                auto&& desc = get_adapters_description();

                for (auto i : desc)
                {
                    using namespace details;

                    if (i.AdapterLuid == adapter)
                    {
                        return i;
                    }
                }

                //adapters should be found
                throw_if_failed(E_FAIL);
                return DXGI_ADAPTER_DESC1();
            }

            inline DXGI_ADAPTER_DESC1 get_device_description(ID3D12Device* device)
            {
                return get_adapter_description( device->GetAdapterLuid() );
            }
        }
    }
}
