#include "pch.h"
#include "uc_uwp_renderer_debug_overlay_page.h"

#include <cstdint>
#include <string>

#include <d3d11.h>
#include <d2d1_3.h>

#include <uc_dev/gx/dx12/gpu/info.h>
#include <uc_dev/gx/d2d/d2d.h>
#include <uc_dev/gx/d2d/dwrite.h>

namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            inline void throw_if_failed(HRESULT h)
            {
                gx::dx12::throw_if_failed(h);
            }

            debug_page::debug_page(initialize_context* ctx) : base(ctx)
            {
                auto resources = ctx->m_resources;
                
                m_text_format = gx::dwrite::create_text_format(resources->dwrite_factory());

                auto&& context = resources->device_context_2d(device_resources::swap_chains::overlay);
                m_brush = gx::d2d::create_solid_color_brush(context);
            }

            debug_page::~debug_page()
            {

            }

            void debug_page::do_update(update_context* )
            {

            }

            void initialize_async()
            {
             
            }

            std::unique_ptr< submitable >  debug_page::do_render( render_context* ctx )
            {
                //todo refactor this, get out of the details and simplify code, see win2d
                {
                    auto resources = ctx->m_resources;

                    auto graphics = gx::dx12::create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::overlay));

                    D2D1_SIZE_F rtSize = resources->back_buffer_d2d(device_resources::swap_chains::overlay)->GetSize();

                    auto width  = ctx->m_back_buffer_size.m_width;
                    auto height = ctx->m_back_buffer_size.m_height;

                    D2D1_RECT_F textRect = D2D1::RectF(0, 0, rtSize.width, rtSize.height);

                    uint32_t m_elapsed_update_time = 0;

                    static const WCHAR text[] = L"Press A on the Xbox Controller to change resolution:";

                    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

                    auto adapters = gx::dx12::get_adapters();
                    auto luid     = ctx->m_resources->device_d2d12()->GetAdapterLuid();

                    {
                        using namespace std;
                        auto it = find_if(begin(adapters), end(adapters), [luid](const auto& a)
                        {
                            DXGI_ADAPTER_DESC1 d;
                            a->GetDesc1(&d);
                            return luid.HighPart == d.AdapterLuid.HighPart && luid.LowPart == d.AdapterLuid.LowPart;
                        });

                        if (it != end(adapters))
                        {
                            adapter = *it;
                        }
                    }

                    //Get a description of the GPU or another simulator device
                    DXGI_ADAPTER_DESC1 d = {};

                    if (adapter)
                    {
                        adapter->GetDesc1(&d);
                    }

                    const std::wstring w = L"Update time: " + std::to_wstring(m_elapsed_update_time) + L"ms Render time: " + std::to_wstring(m_profile_timer.milliseconds()) + L" ms\n";
                    const std::wstring w2 = w + d.Description + L" Video Memory(MB): " + std::to_wstring(d.DedicatedVideoMemory / (1024 * 1024)) + L" System Memory(MB): " + std::to_wstring(d.DedicatedSystemMemory / (1024 * 1024)) + L" Shared Memory(MB): " + std::to_wstring(d.SharedSystemMemory / (1024 * 1024)) + L" ms\n";
                    const std::wstring w3 = w2 + L"Resolution : (" + std::to_wstring(width) + L"x" + std::to_wstring(height) + L"). Press A on the Xbox Controller to change it.";


                    ID3D11Resource *const r[] = { resources->back_buffer_dx11(device_resources::swap_chains::overlay) };
                    // Acquire our wrapped render target resource for the current back buffer.
                    resources->device_11on12(device_resources::swap_chains::overlay)->AcquireWrappedResources(r, 1);

                    // Render text directly to the back buffer.

                    auto&& device_context = resources->device_context_2d(device_resources::swap_chains::overlay);

                    device_context->SetTarget(resources->back_buffer_d2d(device_resources::swap_chains::overlay));
                    device_context->BeginDraw();
                    device_context->Clear();
                    device_context->SetTransform(D2D1::Matrix3x2F::Identity());

                    device_context->DrawText(
                        w3.c_str(), static_cast<uint32_t> (w3.length()),
                        m_text_format.Get(),
                        &textRect,
                        m_brush.Get()
                    );

                    throw_if_failed( device_context->EndDraw() );

                    device_context->SetTarget(nullptr);

                    // Release our wrapped render target resource. Releasing 
                    // transitions the back buffer resource to the state specified
                    // as the OutState when the wrapped resource was created.
                    resources->device_11on12(device_resources::swap_chains::overlay)->ReleaseWrappedResources(r, 1);

                    // Flush to submit the 11 command list to the shared command queue.
                    resources->device_context_d3d11(device_resources::swap_chains::overlay)->Flush();

                    m_profile_timer.reset();

                    return std::make_unique<graphics_submitable>(std::move(graphics));
                }
            }
        }
    }
}