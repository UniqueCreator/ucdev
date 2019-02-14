#include "pch.h"

#include <uc_public/graphics/device_resources.h>
#include "device_resources_impl.h"
#include "resource_create_context_impl.h"
#include "compute_queue_impl.h"
#include "copy_queue_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        DeviceResources::Impl::Impl()
        {

        }

        DeviceResources::Impl::Impl(ResourceCreateContext* c, BackgroundSwapChainResources* b) : m_resource_create_context(c), m_background_swap_chain(b)
        {
            InitializeQueues();
        }

        DeviceResources::Impl::Impl(ResourceCreateContext* c, BackgroundSwapChainResources* b, OverlaySwapChainResources* o) : m_resource_create_context(c), m_background_swap_chain(b), m_overlay_swap_chain(o)
        {
            InitializeQueues();
        }

        void DeviceResources::Impl::InitializeQueues()
        {
            m_copy_queue = m_resource_create_context->CreateCopyQueue();
            m_compute_queue = m_resource_create_context->CreateComputeQueue();

            auto d = m_resource_create_context->GetImpl()->Device();
            auto rc = m_resource_create_context->GetImpl()->rc();

            using namespace uc::gx::dx12;

            auto copy_queue = m_copy_queue->GetImpl()->queue();
            auto compute_queue = m_compute_queue->GetImpl()->queue();

            m_copy_command_manager      = std::make_unique< gpu_command_manager>(d, copy_queue);
            m_compute_command_manager   = std::make_unique< gpu_command_manager>(d, compute_queue);

            m_copy_context_allocator    = std::make_unique< gpu_command_context_allocator>(rc, m_copy_command_manager.get(), copy_queue);
            m_compute_context_allocator = std::make_unique< gpu_command_context_allocator>(rc, m_compute_command_manager.get(), compute_queue);

            m_upload_queue_internal     = std::make_unique< UploadQueueInternal >(std::make_unique<gpu_upload_queue>(d, rc, m_copy_context_allocator.get(), copy_queue));
        }

        void DeviceResources::Impl::Sync()
        {
            m_upload_queue_internal->Sync();
            m_copy_context_allocator->sync();
            m_compute_context_allocator->sync();
        }

        std::unique_ptr<CopyCommandContext> DeviceResources::Impl::CreateCopyCommandContext()
        {
            return std::make_unique<CopyCommandContextInternal>(uc::gx::dx12::create_copy_command_context(m_copy_context_allocator.get()));
        }

        std::unique_ptr<ComputeCommandContext> DeviceResources::Impl::CreateComputeCommandContext()
        {
            return std::make_unique<ComputeCommandContextInternal>(uc::gx::dx12::create_compute_command_context(m_compute_context_allocator.get()));
        }
    }
}


