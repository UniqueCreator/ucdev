#include "pch.h"
#include "uc_uwp_device_resources.h"


#include <uc_dev/gx/d2d/dwrite.h>
#include <uc_dev/gx/d2d/d2d.h>

#include <uc_dev/gx/dx11/dx11.h>

#include <dcomp.h>

#include "uc_uwp_device_swap_chain_resources.h"

namespace uc
{
    namespace uwp
    {
        device_resources::device_resources() : m_swap_chain_count(0)
        {
            create_d3d_12();
        }

        device_resources::~device_resources()
        {
            wait_for_gpu();
        }

        namespace
        {
            static inline bool IsNvidia(IDXGIAdapter* a )
            {
                DXGI_ADAPTER_DESC desc;
                gx::dx12::throw_if_failed(a->GetDesc(&desc));
                return desc.VendorId == 4318;
            }
        }

        void device_resources::create_d3d_12()
        {
            using namespace gx::dx12;

            std::vector< Microsoft::WRL::ComPtr<IDXGIAdapter1> > adapters;

            //adapters = get_warp_adapters();
            adapters = get_adapters();

#if defined(_DEBUG)
            {
                auto hresult = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug));

                if (hresult == S_OK)
                {
                    m_debug->EnableDebugLayer();
                    if (!IsNvidia(adapters[0].Get()))
                    {
                        m_debug->SetEnableGPUBasedValidation(TRUE);
                    }
                }
            }
#endif
            m_device = create_device_always(adapters[0].Get());                    //put here if you have other adapters to test
      
            

            /*
            //test here to check for support of overlays
            Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput;
            gx::dx12::throw_if_failed(adapters[0]->EnumOutputs(0, &dxgiOutput));
            Microsoft::WRL::ComPtr<IDXGIOutput2> dxgiOutput2;
            gx::dx12::throw_if_failed(dxgiOutput.As(&dxgiOutput2));
            */

            Microsoft::WRL::ComPtr<ID3D12CommandQueue>                      present_copy_queue;
            Microsoft::WRL::ComPtr<ID3D12CommandQueue>                      present_compute_queue;
            {
                auto desc = D3D12_COMMAND_QUEUE_DESC();
                desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
                present_copy_queue = create_command_queue(m_device.Get(), &desc);
                present_copy_queue->SetName(L"PresentCopyQueue");
            }

            {
                auto desc = D3D12_COMMAND_QUEUE_DESC();
                desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
                present_compute_queue = create_command_queue(m_device.Get(), &desc);
                present_compute_queue->SetName(L"PresentComputeQueue");
            }

            m_resource_creator              = std::unique_ptr<gpu_resource_create_context >(new gpu_resource_create_context(m_device.Get()));

            m_copy_queue                    = std::make_unique< gpu_command_queue>(m_device.Get(), present_copy_queue.Get());
            m_copy_command_manager          = std::make_unique< gpu_command_manager>(m_device.Get(), m_copy_queue.get());
            m_copy_context_allocator        = std::make_unique< gpu_command_context_allocator>(m_resource_creator.get(), m_copy_command_manager.get(), m_copy_queue.get());

            m_compute_queue                 = std::make_unique< gpu_command_queue>(m_device.Get(), present_compute_queue.Get());
            m_compute_command_manager       = std::make_unique< gpu_command_manager>(m_device.Get(), m_compute_queue.get());
            m_compute_context_allocator     = std::make_unique< gpu_command_context_allocator>(m_resource_creator.get(), m_compute_command_manager.get(), m_compute_queue.get());

            m_upload_queue                  = std::make_unique<gpu_upload_queue>(m_device.Get(), m_resource_creator.get(), m_copy_context_allocator.get(), m_copy_queue.get());
            
            m_dwrite_factory                = gx::dwrite::create_dwrite_factory();
        }

        void device_resources::set_window(const window_environment* environment )
        {
            _ASSERT(m_swap_chain_count>0);
            //calling of this functions must happen before the render process in the frame
            wait_for_gpu();

            m_resource_creator->reset_view_dependent_resources();
            for(auto i = 0U; i < m_swap_chain_count; i++)
                m_swap_chains[i]->set_window(environment, m_resource_creator.get());
        }

        int device_resources::add_swapchain(swap_chain::swap_chain_type type)
        {
            _ASSERT(m_swap_chain_count < MAX_SWAP_CHAINS);
            m_swap_chains[m_swap_chain_count] = std::make_unique<swap_chain::resources>(m_device.Get(), m_resource_creator.get(), type);
            return m_swap_chain_count++;
        }

        // Wait for pending GPU work to complete.
        void device_resources::wait_for_gpu()
        {
            m_copy_queue->wait_for_idle_gpu();
            m_compute_queue->wait_for_idle_gpu();
            for (auto i = 0U; i < m_swap_chain_count; ++i)
                m_swap_chains[i]->wait_for_gpu();

        }

        // Prepare to render the next frame.
        void device_resources::move_to_next_frame()
        {
            for (auto i = 0U; i < m_swap_chain_count; ++i)
                m_swap_chains[i]->move_to_next_frame();
        }

        void device_resources::present()
        {
            // The first argument instructs DXGI to block until VSync, putting the application
            // to sleep until the next VSync. This ensures we don't waste any cycles rendering
            // frames that will never be displayed to the screen.
            for (auto i = 0U; i < m_swap_chain_count; ++i)
                m_swap_chains[i]->present();
        }

        void device_resources::sync()
        {
            for (auto i = 0U; i < m_swap_chain_count; ++i)
            {
                m_swap_chains[i]->sync();
            }

            m_upload_queue->sync();
            m_resource_creator->sync();

            m_copy_context_allocator->sync();
            m_compute_context_allocator->sync();
        }
    }
}