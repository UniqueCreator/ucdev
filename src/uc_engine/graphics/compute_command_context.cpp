#include "pch.h"

#include <uc_public/graphics/compute_command_context.h>
#include <uc_public/uc_pimpl_impl.h>

#include "compute_command_context_impl.h"
#include "compute_pipeline_state_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<ComputeCommandContext::Impl>;

        ComputeCommandContext::ComputeCommandContext() : m_impl()
        {

        }
        
        ComputeCommandContext::~ComputeCommandContext() = default;
        ComputeCommandContext& ComputeCommandContext::operator=(ComputeCommandContext&&) = default;

        void ComputeCommandContext::SetPipelineStateObject( const ComputePipelineState* state )
        {
            m_impl->m_context->set_pso( state->GetImpl()->pipeline_state() );
        }

        void ComputeCommandContext::Dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ)
        {
            m_impl->m_context->dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
        }
    }
}


