#include "pch.h"

#include <uc_public/graphics/compute_pipeline_state.h>
#include <uc_public/uc_pimpl_impl.h>

#include "compute_pipeline_state_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<ComputePipelineState::Impl>;

        ComputePipelineState::ComputePipelineState()
        {

        }

        ComputePipelineState::~ComputePipelineState() = default;
        ComputePipelineState& ComputePipelineState::operator=(ComputePipelineState&&) = default;

        ComputePipelineState::Impl*   ComputePipelineState::GetImpl()
        {
            return m_impl.get();
        }

        const ComputePipelineState::Impl*   ComputePipelineState::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


