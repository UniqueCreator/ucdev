#include "pch.h"

#include <uc_public/graphics/composition_swap_chain_resources.h>
#include <uc_public/uc_pimpl_impl.h>

#include "swap_chain_resources_impl.h"

#include "resource_create_context_impl.h"
#include "direct_queue_impl.h"
#include "window_environment.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class CompositionSwapChainResources::Impl
        {
            public:

            std::unique_ptr<DirectQueue>                          m_queue;
            Microsoft::WRL::ComPtr<IDXGISwapChain3>               m_swap_chain;

            float                                                 m_composition_scale_x = 1.0f;
            float                                                 m_composition_scale_y = 1.0f;

            Size2D                                                m_logical_size = { 1.0f, 1.0f };
            winrt::Windows::Graphics::Display::DisplayInformation m_display_information = nullptr;
        };

        namespace
        {
            static auto CreateSwapChain(IDXGIFactory2* factory, ID3D12CommandQueue* queue, uint32_t back_buffer_width, uint32_t back_buffer_height)
            {
                // Otherwise, create a new one using the same adapter as the existing Direct3D device.
                DXGI_SCALING scaling = DXGI_SCALING_STRETCH;    // DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
                DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

                swapChainDesc.Width = back_buffer_width;
                swapChainDesc.Height = back_buffer_height;
                swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                swapChainDesc.Stereo =           false;
                swapChainDesc.SampleDesc.Count  = 1;                                    // Don't use multi-sampling.
                swapChainDesc.SampleDesc.Quality = 0;
                swapChainDesc.BufferUsage   = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                swapChainDesc.BufferCount   = 3;                                    // Use triple-buffering to minimize latency.
                swapChainDesc.SwapEffect    = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // All Windows Universal apps must use _FLIP_ SwapEffects
                swapChainDesc.Flags         = 0;
                swapChainDesc.Scaling       = scaling;
                swapChainDesc.AlphaMode     = DXGI_ALPHA_MODE_IGNORE;

                Microsoft::WRL::ComPtr<IDXGISwapChain1> result;
                uc::gx::dx12::throw_if_failed(
                    factory->CreateSwapChainForComposition(
                        queue,
                        &swapChainDesc,
                        nullptr,
                        &result
                    )
                );

                Microsoft::WRL::ComPtr<IDXGISwapChain3> result2;
                uc::gx::dx12::throw_if_failed(result.As(&result2));

                return result2;
            }
        }

        CompositionSwapChainResources::CompositionSwapChainResources( ResourceCreateContext* ctx, Size2D logicalSize, const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation, float compositionScaleX, float compositionScaleY )
        : Base( nullptr, ctx->CreateDirectQueue().release(), ctx )
        {

            m_impl->m_queue.reset(Base::m_impl->m_direct_queue);
            {
                auto environment = BuildSwapChainSize(logicalSize, displayInformation, compositionScaleX, compositionScaleY);
                auto w = static_cast<uint32_t>(environment.m_width);
                auto h = static_cast<uint32_t>(environment.m_height);

                Microsoft::WRL::ComPtr<IDXGIFactory4> r0;
                uint32_t flags = 0;

                #if defined(_DEBUG)
                flags |= DXGI_CREATE_FACTORY_DEBUG;
                #endif

                uc::gx::dx12::throw_if_failed(CreateDXGIFactory2(flags, IID_PPV_ARGS(&r0)));
                auto queue                          = m_impl->m_queue->GetImpl()->queue()->queue();
                m_impl->m_swap_chain                = CreateSwapChain(r0.Get(), queue, w, h);
                m_impl->m_logical_size              = logicalSize;
                m_impl->m_composition_scale_x       = compositionScaleX;
                m_impl->m_composition_scale_y       = compositionScaleY;
                m_impl->m_display_information       = displayInformation;
                Base::m_impl->m_swap_chain          = m_impl->m_swap_chain.Get();
                Base::m_impl->m_swap_buffer_format  = DXGI_FORMAT_R8G8B8A8_UNORM;

                Base::m_impl->ResetBufferCount(3);
                Base::m_impl->Resize(w, h);
            }
        }

        CompositionSwapChainResources::~CompositionSwapChainResources() = default;
        CompositionSwapChainResources& CompositionSwapChainResources::operator=(CompositionSwapChainResources&&) = default;

        void CompositionSwapChainResources::SetCompositionScale(float scaleX, float scaleY)
        {
            m_impl->m_composition_scale_x = scaleX;
            m_impl->m_composition_scale_y = scaleY;
            auto r = BuildSwapChainSize(m_impl->m_logical_size, m_impl->m_display_information, m_impl->m_composition_scale_x, m_impl->m_composition_scale_y);
            Resize(static_cast<uint32_t>(r.m_width), static_cast<uint32_t>(r.m_height));
        }

        void CompositionSwapChainResources::SetLogicalSize(Size2D size)
        {
            m_impl->m_logical_size = size;
            auto r = BuildSwapChainSize(size, m_impl->m_display_information, m_impl->m_composition_scale_x, m_impl->m_composition_scale_y);
            Resize(static_cast<uint32_t>(r.m_width), static_cast<uint32_t>(r.m_height));
        }

        void CompositionSwapChainResources::SetDisplayInformation(const winrt::Windows::Graphics::Display::DisplayInformation& displayInformation)
        {
            m_impl->m_display_information = displayInformation;
            auto r = BuildSwapChainSize(m_impl->m_logical_size, displayInformation, m_impl->m_composition_scale_x, m_impl->m_composition_scale_y);
            Resize(static_cast<uint32_t>(r.m_width), static_cast<uint32_t>(r.m_height));
        }
    }
}


