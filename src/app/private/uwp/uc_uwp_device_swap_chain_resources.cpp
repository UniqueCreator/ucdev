#include "pch.h"

#include "uc_uwp_device_swap_chain_resources.h"

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.core.h>

#include <uc_dev/gx/d2d/d2d.h>
#include <uc_dev/gx/dx11/dx11.h>

namespace uc
{
    namespace uwp
    {
        namespace swap_chain
        {
            namespace details
            {
                constexpr DXGI_FORMAT back_buffer_format()
                {
                    return DXGI_FORMAT_R8G8B8A8_UNORM;  // This is the most common swap chain format.
                }

                inline Microsoft::WRL::ComPtr<IDXGISwapChain3> create_swap_chain(IDXGIFactory2* factory, ID3D12CommandQueue* queue, const winrt::Windows::UI::Core::CoreWindow& window, uint32_t back_buffer_width, uint32_t back_buffer_height)
                {
                    // Otherwise, create a new one using the same adapter as the existing Direct3D device.
                    DXGI_SCALING scaling = DXGI_SCALING_STRETCH;    // DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
                    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

                    swapChainDesc.Width                 = back_buffer_width;
                    swapChainDesc.Height                = back_buffer_height;
                    swapChainDesc.Format                = back_buffer_format();
                    swapChainDesc.Stereo                = false;
                    swapChainDesc.SampleDesc.Count      = 1;                                    // Don't use multi-sampling.
                    swapChainDesc.SampleDesc.Quality    = 0;
                    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                    swapChainDesc.BufferCount           = 3;                                    // Use triple-buffering to minimize latency.
                    swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // All Windows Universal apps must use _FLIP_ SwapEffects
                    swapChainDesc.Flags                 = 0;
                    swapChainDesc.Scaling               = scaling;
                    swapChainDesc.AlphaMode             = DXGI_ALPHA_MODE_IGNORE;


                    Microsoft::WRL::ComPtr<IDXGISwapChain1> result;
                    gx::dx12::throw_if_failed(

                        factory->CreateSwapChainForCoreWindow(
                            queue,
                            reinterpret_cast<IUnknown*>(winrt::get_abi(window)),
                            &swapChainDesc,
                            nullptr,
                            &result
                        )
                    );

                    Microsoft::WRL::ComPtr<IDXGISwapChain3> result2;
                    gx::dx12::throw_if_failed(result.As(&result2));

                    return result2;
                }

                inline Microsoft::WRL::ComPtr<IDXGISwapChain3> create_composition_chain(IDXGIFactory2* factory, ID3D12CommandQueue* queue, const winrt::Windows::UI::Core::CoreWindow&, uint32_t back_buffer_width, uint32_t back_buffer_height)
                {
                    // Otherwise, create a new one using the same adapter as the existing Direct3D device.
                    DXGI_SCALING scaling = DXGI_SCALING_STRETCH;    // DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
                    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

                    swapChainDesc.Width = back_buffer_width;
                    swapChainDesc.Height = back_buffer_height;
                    swapChainDesc.Format = back_buffer_format();
                    swapChainDesc.Stereo = false;
                    swapChainDesc.SampleDesc.Count = 1;                                    // Don't use multi-sampling.
                    swapChainDesc.SampleDesc.Quality = 0;
                    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                    swapChainDesc.BufferCount = 3;                                    // Use triple-buffering to minimize latency.
                    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // All Windows Universal apps must use _FLIP_ SwapEffects
                    swapChainDesc.Flags = 0;
                    swapChainDesc.Scaling = scaling;
                    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

                    Microsoft::WRL::ComPtr<IDXGISwapChain1> result;
                    gx::dx12::throw_if_failed(
                        factory->CreateSwapChainForComposition(
                            queue,
                            &swapChainDesc,
                            nullptr,
                            &result
                        )
                    );

                    Microsoft::WRL::ComPtr<IDXGISwapChain3> result2;
                    gx::dx12::throw_if_failed(result.As(&result2));

                    return result2;
                }

                inline Microsoft::WRL::ComPtr<IDXGISwapChain3> create_overlay_swap_chain(IDXGIFactory2* factory, ID3D12CommandQueue* queue, const winrt::Windows::UI::Core::CoreWindow& window, uint32_t back_buffer_width, uint32_t back_buffer_height)
                {
                    DXGI_SCALING scaling = DXGI_SCALING_NONE;
                    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

                    swapChainDesc.Width = back_buffer_width;
                    swapChainDesc.Height = back_buffer_height;
                    swapChainDesc.Format = back_buffer_format();
                    swapChainDesc.Stereo = false;
                    swapChainDesc.SampleDesc.Count      = 1;                                            // Don't use multi-sampling.
                    swapChainDesc.SampleDesc.Quality    = 0;
                    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                    swapChainDesc.BufferCount           = 3;                                            // Use triple-buffering to minimize latency.
                    swapChainDesc.SwapEffect            =  DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;            // for hud rendering we will use partial presentation optimizations
                    swapChainDesc.Flags                 = DXGI_SWAP_CHAIN_FLAG_FOREGROUND_LAYER;
                    swapChainDesc.Scaling               = scaling;
                    swapChainDesc.AlphaMode             = DXGI_ALPHA_MODE_PREMULTIPLIED;

                    Microsoft::WRL::ComPtr<IDXGISwapChain1> result;
                    gx::dx12::throw_if_failed(

                        factory->CreateSwapChainForCoreWindow(
                            queue,
                            reinterpret_cast<IUnknown*>(winrt::get_abi(window)),
                            &swapChainDesc,
                            nullptr,
                            &result
                        )
                    );

                    Microsoft::WRL::ComPtr<IDXGISwapChain3> result2;
                    gx::dx12::throw_if_failed(result.As(&result2));

                    return result2;
                }

                inline Microsoft::WRL::ComPtr<IDXGISwapChain3> create_overlay_composition_chain(IDXGIFactory2* factory, ID3D12CommandQueue* queue, uint32_t back_buffer_width, uint32_t back_buffer_height)
                {
                    DXGI_SCALING scaling = DXGI_SCALING_NONE;
                    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

                    swapChainDesc.Width = back_buffer_width;
                    swapChainDesc.Height = back_buffer_height;
                    swapChainDesc.Format = back_buffer_format();
                    swapChainDesc.Stereo = false;
                    swapChainDesc.SampleDesc.Count = 1;                                            // Don't use multi-sampling.
                    swapChainDesc.SampleDesc.Quality = 0;
                    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                    swapChainDesc.BufferCount = 3;                                            // Use triple-buffering to minimize latency.
                    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;            // for hud rendering we will use partial presentation optimizations
                    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FOREGROUND_LAYER;
                    swapChainDesc.Scaling = scaling;
                    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

                    Microsoft::WRL::ComPtr<IDXGISwapChain1> result;
                    gx::dx12::throw_if_failed(
                        factory->CreateSwapChainForComposition(
                            queue,
                            &swapChainDesc,
                            nullptr,
                            &result
                        )
                    );

                    Microsoft::WRL::ComPtr<IDXGISwapChain3> result2;
                    gx::dx12::throw_if_failed(result.As(&result2));

                    return result2;
                }

                static inline uint64_t next_frame(uint64_t frame)
                {
                    uint64_t r = frame + 1;
                    return r % 3;
                }
            }

            resources::resources( ID3D12Device* device, gx::dx12::gpu_resource_create_context* resource_creator, swap_chain_type type ) : m_factories( make_factories() ), m_device_d3d_12( device ), m_type(type)
            {
                {
                    Microsoft::WRL::ComPtr<ID3D12CommandQueue>                      present_queue;

                    {
                        auto desc = D3D12_COMMAND_QUEUE_DESC();
                        present_queue = gx::dx12::create_command_queue(m_device_d3d_12.Get(), &desc);
                        present_queue->SetName(L"PresentName");
                    }

                    m_direct_queue = std::make_unique< gx::dx12::gpu_command_queue>(m_device_d3d_12.Get(), present_queue.Get());
                    m_direct_command_manager = std::make_unique< gx::dx12::gpu_command_manager>(m_device_d3d_12.Get(), m_direct_queue.get());
                    m_direct_context_allocator = std::make_unique< gx::dx12::gpu_command_context_allocator>(resource_creator, m_direct_command_manager.get(), m_direct_queue.get());
                }

                create_d3d_11();
                create_d2d_3();
            }

            void resources::create_d3d_11()
            {
                auto r0 = gx::dx11::d3d11_on_d3d12_create_device(m_device_d3d_12.Get(), 0, m_direct_queue->queue());
                m_device_d3d_11 = std::move(std::get<0>(r0));
                m_device_context_d3d_11 = std::move(std::get<1>(r0));
                throw_if_failed(m_device_d3d_11.As(&m_device_d3d11on12));
            }

            void resources::create_d2d_3()
            {
                Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device;
                throw_if_failed(this->m_device_d3d_11.As(&dxgi_device));

                m_device_d2d_1          = gx::d2d::create_device(d2d_factory(), dxgi_device.Get());
                m_device_context_d2d_1  = gx::d2d::create_device_context(m_device_d2d_1.Get());
            }

            resources::~resources()
            {

            }

            void resources::set_window(const window_environment* environment, gx::dx12::gpu_resource_create_context* resource_creator)
            {
                auto width = static_cast<uint32_t> (environment->m_back_buffer_size.Width);
                auto height = static_cast<uint32_t> (environment->m_back_buffer_size.Height);

                const auto& window = environment->m_window;

                if (m_swap_chain)
                {

                    m_device_context_d2d_1->SetTarget(nullptr);

                    m_d2d_bitmap[0].Reset();
                    m_d2d_bitmap[1].Reset();
                    m_d2d_bitmap[2].Reset();

                    m_d3d11_back_buffers[0].Reset();
                    m_d3d11_back_buffers[1].Reset();
                    m_d3d11_back_buffers[2].Reset();


                    m_back_buffers[0].reset();
                    m_back_buffers[1].reset();
                    m_back_buffers[2].reset();


                    m_device_context_d3d_11->ClearState();
                    m_device_context_d3d_11->Flush();


                    uint32_t flags = (m_type != swap_chain_type::foreground_core_window) ? 0 : DXGI_SWAP_CHAIN_FLAG_FOREGROUND_LAYER;
                    gx::dx12::throw_if_failed(m_swap_chain->ResizeBuffers(3, width, height, details::back_buffer_format(), flags));
                }
                else
                {
                    switch (m_type)
                    {
                        case swap_chain_type::background_core_window:
                        {
                            m_swap_chain = details::create_swap_chain(m_factories->m_dxgi.Get(), m_direct_queue->queue(), window, width, height);
                            break;
                        }

                        case swap_chain_type::background_composition:
                        {
                            m_swap_chain = details::create_composition_chain(m_factories->m_dxgi.Get(), m_direct_queue->queue(), window, width, height);
                            break;
                        }

                        case swap_chain_type::foreground_core_window:
                        {
                            m_swap_chain = details::create_overlay_swap_chain(m_factories->m_dxgi.Get(), m_direct_queue->queue(), window, width, height);
                            break;
                        }

                        case swap_chain_type::foreground_composition:
                        {
                            m_swap_chain = details::create_overlay_composition_chain(m_factories->m_dxgi.Get(), m_direct_queue->queue(), width, height);
                            break;
                        }

                        default:assert(false); break;
                    }
                }

                auto frame0 = m_current_frame;
                auto frame1 = details::next_frame(m_current_frame);
                auto frame2 = details::next_frame(frame1);

                {
                    //Recreate the back buffers
                    Microsoft::WRL::ComPtr<ID3D12Resource> back_buffer[3];
                    gx::dx12::throw_if_failed(m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer[0])));
                    gx::dx12::throw_if_failed(m_swap_chain->GetBuffer(1, IID_PPV_ARGS(&back_buffer[1])));
                    gx::dx12::throw_if_failed(m_swap_chain->GetBuffer(2, IID_PPV_ARGS(&back_buffer[2])));

                 

                    //when you resize the buffers, buffer0 maps to the current frame, buffer1 next, and buffer2 next
                    m_back_buffers[frame0] = std::unique_ptr<gx::dx12::gpu_back_buffer>(resource_creator->create_back_buffer(back_buffer[0].Get()));
                    m_back_buffers[frame1] = std::unique_ptr<gx::dx12::gpu_back_buffer>(resource_creator->create_back_buffer(back_buffer[1].Get()));
                    m_back_buffers[frame2] = std::unique_ptr<gx::dx12::gpu_back_buffer>(resource_creator->create_back_buffer(back_buffer[2].Get()));
                }


                {
                    D3D11_RESOURCE_FLAGS d3d11_flags = { D3D11_BIND_RENDER_TARGET };
                    m_d3d11_back_buffers[frame0] = gx::dx11::create_wrapped_resource(m_device_d3d11on12.Get(), m_back_buffers[frame0]->resource(), d3d11_flags, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_PRESENT);
                    m_d3d11_back_buffers[frame1] = gx::dx11::create_wrapped_resource(m_device_d3d11on12.Get(), m_back_buffers[frame1]->resource(), d3d11_flags, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_PRESENT);
                    m_d3d11_back_buffers[frame2] = gx::dx11::create_wrapped_resource(m_device_d3d11on12.Get(), m_back_buffers[frame2]->resource(), d3d11_flags, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_PRESENT);
                }

              
                {
                    // Create a render target for D2D to draw directly to this back buffer.
                    Microsoft::WRL::ComPtr<IDXGISurface> surface_0;
                    Microsoft::WRL::ComPtr<IDXGISurface> surface_1;
                    Microsoft::WRL::ComPtr<IDXGISurface> surface_2;

                    gx::dx12::throw_if_failed(m_d3d11_back_buffers[frame0].As(&surface_0));
                    gx::dx12::throw_if_failed(m_d3d11_back_buffers[frame1].As(&surface_1));
                    gx::dx12::throw_if_failed(m_d3d11_back_buffers[frame2].As(&surface_2));

                    {
                        float dpi = environment->m_dpi;
                        m_d2d_bitmap[frame0] = gx::d2d::create_bitmap_from_dxgi_surface(m_device_context_d2d_1.Get(), surface_0.Get(), dpi, dpi);
                        m_d2d_bitmap[frame1] = gx::d2d::create_bitmap_from_dxgi_surface(m_device_context_d2d_1.Get(), surface_1.Get(), dpi, dpi);
                        m_d2d_bitmap[frame2] = gx::d2d::create_bitmap_from_dxgi_surface(m_device_context_d2d_1.Get(), surface_2.Get(), dpi, dpi);
                    }
                }
            }

            void resources::wait_for_gpu()
            {
                m_direct_queue->wait_for_idle_gpu();
            }

            void resources::move_to_next_frame()
            {
                auto fence = m_direct_queue->increment_fence();
                m_direct_queue->wait_for_fence( fence - 2 );
                // Advance the frame index.
                m_current_frame = (m_current_frame + 1) % 3;
            }

            void resources::present()
            {
                // The first argument instructs DXGI to block until VSync, putting the application
                // to sleep until the next VSync. This ensures we don't waste any cycles rendering
                // frames that will never be displayed to the screen.
                m_direct_queue->present(m_swap_chain.Get());
            }

            void resources::sync()
            {
                m_direct_context_allocator->sync();
            }
        }
    }
}
