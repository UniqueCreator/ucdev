#include "pch.h"

#include "direct_command_context_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "direct_command_context_impl.h"
#include "gpu_command_context_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        DirectCommandContextInternal::DirectCommandContextInternal(uc::gx::dx12::managed_graphics_command_context&& context)
        {
            m_impl                    = DirectCommandContext::Impl(std::move(context));
            GpuCommandContext::m_impl = GpuCommandContext::Impl(m_impl->m_context.get());
        }
    }
}


