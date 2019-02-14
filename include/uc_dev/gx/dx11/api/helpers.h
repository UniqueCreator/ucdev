#pragma once

#include <d3d11.h>
#include <d3d11on12.h>
#include <d3d12.h>

#include <cstdint>
#include <wrl/client.h>
#include <tuple>

#include <uc_dev/gx/dx11/api/error.h>

namespace uc
{
    namespace gx
    {
        namespace dx11
        {
            inline
                std::tuple
                <
                Microsoft::WRL::ComPtr<ID3D11Device>,
                Microsoft::WRL::ComPtr<ID3D11DeviceContext>
                >
                d3d11_on_d3d12_create_device(ID3D12Device* d, uint32_t flags, ID3D12CommandQueue* queue)
            {
                Microsoft::WRL::ComPtr<ID3D11Device>        ra;
                Microsoft::WRL::ComPtr<ID3D11DeviceContext> rb;

                flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
                // If the project is in a debug build, enable debugging via SDK Layers with this flag.
                flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
                Microsoft::WRL::ComPtr<ID3D12CommandQueue> q(queue);
                Microsoft::WRL::ComPtr<IUnknown> q0;

                throw_if_failed(q.As(&q0));
                D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
                IUnknown* a[] = { q0.Get() };

#if defined(_DEBUG)
                {
                    if (FAILED(D3D11On12CreateDevice(d, flags, &level, 1, a, 1, 0, &ra, &rb, nullptr)))
                    {
                        flags &= ~D3D11_CREATE_DEVICE_DEBUG;
                    }
                }
#endif
                if (!ra)
                {
                    throw_if_failed(D3D11On12CreateDevice(d, flags, &level, 1, a, 1, 0, &ra, &rb, nullptr));
                }

                return std::make_tuple(std::move(ra), std::move(rb));
            }

            inline Microsoft::WRL::ComPtr<ID3D11Resource> create_wrapped_resource(ID3D11On12Device* device, ID3D12Resource* r, D3D11_RESOURCE_FLAGS f, D3D12_RESOURCE_STATES in, D3D12_RESOURCE_STATES out)
            {
                Microsoft::WRL::ComPtr<ID3D11Resource> result;

                throw_if_failed(
                    device->CreateWrappedResource(
                        r,
                        &f,
                        in,
                        out,
                        IID_PPV_ARGS(&result)
                    ));

                return result;
            }
        }
    }
}
