#include "pch.h"

#include <uc_public/graphics/gpu_command_context.h>
#include <uc_public/uc_pimpl_impl.h>

#include "gpu_command_context_impl.h"
#include "fence_handle_internal.h"
#include "gpu_virtual_resource_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<GpuCommandContext::Impl>;

        GpuCommandContext::GpuCommandContext() : m_impl()
        {

        }
        
        GpuCommandContext::~GpuCommandContext() = default;
        GpuCommandContext& GpuCommandContext::operator=(GpuCommandContext&&) = default;

        std::unique_ptr<FenceHandle> GpuCommandContext::Submit()
        {
            auto fence = m_impl->m_context->submit(uc::gx::dx12::gpu_command_context::do_not_wait_to_execute);
            return std::make_unique<FenceHandleInternal>(fence);
        }

        void GpuCommandContext::SubmitAndWaitToExecute()
        {
            m_impl->m_context->submit(uc::gx::dx12::gpu_command_context::wait_to_execute);
        }

        void GpuCommandContext::TransitionResource(const GpuVirtualResource* r, ResourceState old_state, ResourceState new_state)
        {
            auto b = r->GetImpl()->resource();

            m_impl->m_context->transition_resource(b, static_cast<D3D12_RESOURCE_STATES>(old_state), static_cast<D3D12_RESOURCE_STATES>(new_state));
        }
    }
}


