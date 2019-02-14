#pragma once

#include "upload_queue_internal.h"
#include "resource_create_context_internal.h"

#include "compute_queue_internal.h"
#include "copy_queue_internal.h"
#include "compute_command_context_internal.h"
#include "copy_command_context_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class DeviceResources::Impl
        {
            public:

            Impl();
            Impl(ResourceCreateContext* c, BackgroundSwapChainResources* b);
            Impl(ResourceCreateContext* c, BackgroundSwapChainResources* b, OverlaySwapChainResources* o);

            
            ResourceCreateContext*                                              m_resource_create_context = nullptr;
            BackgroundSwapChainResources*                                       m_background_swap_chain;
            OverlaySwapChainResources*                                          m_overlay_swap_chain;
            
            std::unique_ptr<CopyQueue>                                          m_copy_queue;
            std::unique_ptr<ComputeQueue>                                       m_compute_queue;

            std::unique_ptr<uc::gx::dx12::gpu_command_manager>                  m_copy_command_manager;
            std::unique_ptr<uc::gx::dx12::gpu_command_context_allocator>        m_copy_context_allocator;

            std::unique_ptr<uc::gx::dx12::gpu_command_manager>                  m_compute_command_manager;
            std::unique_ptr<uc::gx::dx12::gpu_command_context_allocator>        m_compute_context_allocator;


            std::unique_ptr<UploadQueueInternal>                                m_upload_queue_internal;

            std::unique_ptr<CopyCommandContext>                                 CreateCopyCommandContext();
            std::unique_ptr<ComputeCommandContext>                              CreateComputeCommandContext();

            void Sync();

            private:
            void InitializeQueues();


        };
    }
}


