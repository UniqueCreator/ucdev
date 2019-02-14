#pragma once

#include <dxgi1_5.h>
#include <cstdint>
#include <wrl/client.h>

#include "error.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline Microsoft::WRL::ComPtr<IDXGIFactory2> create_dxgi_factory1()
            {
                Microsoft::WRL::ComPtr<IDXGIFactory2> result;
                throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<IDXGIFactory4> create_dxgi_factory4()
            {
                Microsoft::WRL::ComPtr<IDXGIFactory4> result;
                throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&result)));
                return result;
            }
        }
    }
}
