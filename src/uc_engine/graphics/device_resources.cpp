#include "pch.h"

#include <uc_public/graphics/device_resources.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/graphics/upload_queue.h>
#include <uc_public/graphics/resource_create_context.h>

#include <uc_public/graphics/background_swap_chain_resources.h>
#include <uc_public/graphics/overlay_swap_chain_resources.h>

#include "device_resources_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<DeviceResources::Impl>;

        DeviceResources::DeviceResources( ResourceCreateContext* ctx, BackgroundSwapChainResources* background ) : m_impl(ctx, background)
        {
            background;
        }

        DeviceResources::DeviceResources(ResourceCreateContext* ctx, BackgroundSwapChainResources* background, OverlaySwapChainResources* overlay) : m_impl(ctx, background, overlay)
        {
            background;
            overlay;
        }

        DeviceResources::~DeviceResources() = default;
        DeviceResources& DeviceResources::operator=(DeviceResources&&) = default;

        const UploadQueue* DeviceResources::GetUploadQueue() const
        {
            return m_impl->m_upload_queue_internal.get();
        }

        UploadQueue* DeviceResources::GetUploadQueue()
        {
            return m_impl->m_upload_queue_internal.get();
        }

        const ResourceCreateContext* DeviceResources::GetResourceCreateContext() const
        {
            return m_impl->m_resource_create_context;
        }

        ResourceCreateContext* DeviceResources::GetResourceCreateContext()
        {
            return m_impl->m_resource_create_context;
        }

        const BackgroundSwapChainResources* DeviceResources::GetBackgroundSwapChainResources() const
        {
            return m_impl->m_background_swap_chain;
        }

        BackgroundSwapChainResources* DeviceResources::GetBackgroundSwapChainResources()
        {
            return m_impl->m_background_swap_chain;
        }

        const OverlaySwapChainResources* DeviceResources::GetOverlaySwapChainResources() const
        {
            return m_impl->m_overlay_swap_chain;
        }

        OverlaySwapChainResources* DeviceResources::GetOverlaySwapChainResources()
        {
            return m_impl->m_overlay_swap_chain;
        }

        void DeviceResources::WaitForGpu()
        {
            m_impl->m_compute_queue->WaitForIdleGpu();
            m_impl->m_copy_queue->WaitForIdleGpu();
        }

        void DeviceResources::Present()
        {
            GetBackgroundSwapChainResources()->Present();
            SwapChainResources* o = GetOverlaySwapChainResources();
            if (o)
            {
                o->Present();
            }
        }

        void DeviceResources::Sync()
        {
            m_impl->Sync();
        }

        void DeviceResources::ResetViewDependentResources()
        {
            m_impl->m_resource_create_context->ResetViewDependentResources();
        }

        std::unique_ptr<CopyCommandContext>    DeviceResources::CreateCopyCommandContext()
        {
            return m_impl->CreateCopyCommandContext();
        }

        std::unique_ptr<ComputeCommandContext> DeviceResources::CreateComputeCommandContext()
        {
            return m_impl->CreateComputeCommandContext();
        }
    }
}


