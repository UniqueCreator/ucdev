#include "pch.h"

#include <uc_public/graphics/graphics_pipeline_state.h>
#include <uc_public/uc_pimpl_impl.h>

#include "graphics_pipeline_state_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<GraphicsPipelineState::Impl>;

        GraphicsPipelineState::GraphicsPipelineState()
        {

        }

        GraphicsPipelineState::~GraphicsPipelineState() = default;
        GraphicsPipelineState& GraphicsPipelineState::operator=(GraphicsPipelineState&&) = default;

        GraphicsPipelineState::Impl*   GraphicsPipelineState::GetImpl()
        {
            return m_impl.get();
        }

        const GraphicsPipelineState::Impl*   GraphicsPipelineState::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


