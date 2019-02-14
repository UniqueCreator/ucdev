#pragma once

#include <memory>

#include <d3d11.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_5.h>
#include <windows.foundation.h>
#include <windows.ui.core.h>

#include <wrl/client.h>
#include <d2d1_3.h>
#include <dwrite.h>

#include <uc_dev/util/noncopyable.h>
#include <uc_dev/gx/dx12/dx12.h>

#include "uc_uwp_renderer_impl_window.h"

namespace uc
{
    class gx::dx12::gpu_resource_create_context;

    namespace uwp
    {
        struct window_environment;

        namespace swap_chain
        {
            inline void throw_if_failed(HRESULT hr)
            {
                gx::dx12::throw_if_failed(hr);
            }

            struct factories : public util::noncopyable
            {
                //factory
                Microsoft::WRL::ComPtr<IDXGIFactory4>                           m_dxgi;
                Microsoft::WRL::ComPtr<ID2D1Factory3>                           m_d2d;
            };

            inline Microsoft::WRL::ComPtr<ID2D1Factory3> create_d2d_factory()
            {
                Microsoft::WRL::ComPtr<ID2D1Factory3> result;
                D2D1_FACTORY_OPTIONS options = {};
                options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
                throw_if_failed(D2D1CreateFactory<ID2D1Factory3>(D2D1_FACTORY_TYPE_MULTI_THREADED, options, result.GetAddressOf()));
                return result;
            }

            inline std::unique_ptr<factories> make_factories()
            {
                Microsoft::WRL::ComPtr<IDXGIFactory4> r0;
                throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&r0)));

                std::unique_ptr<factories> result = std::make_unique<factories>();

                result->m_dxgi = std::move(r0);
                result->m_d2d = create_d2d_factory();

                return result;
            }

            enum struct swap_chain_type
            {
                background_core_window,
                background_composition,
				foreground_core_window,
				foreground_composition,
            };

            class resources : public util::noncopyable
            {
                public:

                resources(  ID3D12Device* device, gx::dx12::gpu_resource_create_context* resource_creator, swap_chain_type type );
                ~resources();

                void set_window( const window_environment* environment, gx::dx12::gpu_resource_create_context* resource_creator );

                gx::dx12::gpu_back_buffer* back_buffer_dx12() const
                {
                    return m_back_buffers[m_current_frame].get();
                }

                IDXGISwapChain3* swap_chain() const
                {
                    return m_swap_chain.Get();
                }

                ID3D11Resource* back_buffer_dx11() const
                {
                    return m_d3d11_back_buffers[m_current_frame].Get();
                }

                ID2D1Bitmap1* back_buffer_d2d() const
                {
                    return m_d2d_bitmap[m_current_frame].Get();
                }

                ID3D12Device* device_d2d12() const
                {
                    return m_device_d3d_12.Get();
                }

                gx::dx12::gpu_command_context_allocator* direct_command_context_allocator() const
                {
                    return m_direct_context_allocator.get();
                }

                gx::dx12::gpu_command_queue* direct_queue() const
                {
                    return this->m_direct_queue.get();
                }

                ID2D1DeviceContext* device_context_2d() const
                {
                    return m_device_context_d2d_1.Get();
                }

                ID2D1Factory3* d2d_factory() const
                {
                    return m_factories->m_d2d.Get();
                }

                ID3D11Device* device_d3d11() const
                {
                    return m_device_d3d_11.Get();
                }

                ID3D11On12Device* device_11on12() const
                {
                    return m_device_d3d11on12.Get();
                }

                ID3D11DeviceContext* device_context_d3d11() const
                {
                    return m_device_context_d3d_11.Get();
                }

                gx::dx12::gpu_command_context_allocator* command_context_allocator() const
                {
                    return m_direct_context_allocator.get();
                }

                gx::dx12::gpu_command_queue* queue() const
                {
                    return this->m_direct_queue.get();
                }

                void set_source_size(uint32_t width, uint32_t height )
                {
                    throw_if_failed(m_swap_chain->SetSourceSize(width, height));
                }

                void wait_for_gpu();
                void move_to_next_frame();
                void present();
                void sync();

            private:

                std::unique_ptr<factories>                                      m_factories;

                //dx12 buffers
                Microsoft::WRL::ComPtr<ID3D12Device>                            m_device_d3d_12;
                std::unique_ptr<gx::dx12::gpu_back_buffer>                      m_back_buffers[3];

                //dx11 buffers
                Microsoft::WRL::ComPtr<ID3D11Device>                            m_device_d3d_11;
                Microsoft::WRL::ComPtr<ID3D11DeviceContext>                     m_device_context_d3d_11;
                Microsoft::WRL::ComPtr<ID3D11On12Device>                        m_device_d3d11on12;
                Microsoft::WRL::ComPtr<ID3D11Resource>                          m_d3d11_back_buffers[3];

                //d2d buffers
                Microsoft::WRL::ComPtr<ID2D1Device>                             m_device_d2d_1;
                Microsoft::WRL::ComPtr<ID2D1DeviceContext>                      m_device_context_d2d_1;
                Microsoft::WRL::ComPtr<ID2D1Bitmap1>                            m_d2d_bitmap[3];

                Microsoft::WRL::ComPtr<IDXGISwapChain3>                         m_swap_chain;

                uint64_t                                                        m_current_frame = 0;

                //submission control
                std::unique_ptr<gx::dx12::gpu_command_queue>                    m_direct_queue;
                std::unique_ptr<gx::dx12::gpu_command_manager>                  m_direct_command_manager;
                std::unique_ptr<gx::dx12::gpu_command_context_allocator>        m_direct_context_allocator;

                void create_d3d_11();
                void create_d2d_3();
                swap_chain_type                                                 m_type;
            };
        }
    }
}