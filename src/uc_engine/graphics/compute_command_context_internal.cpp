#include "pch.h"

#include "compute_command_context_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "compute_command_context_impl.h"
#include "gpu_command_context_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        ComputeCommandContextInternal::ComputeCommandContextInternal(uc::gx::dx12::managed_compute_command_context&& context)
        {
            m_impl                    = ComputeCommandContext::Impl(std::move(context));
            GpuCommandContext::m_impl = GpuCommandContext::Impl(m_impl->m_context.get());
        }
    }
}


