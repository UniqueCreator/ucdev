#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class GraphicsPipelineState : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API ~GraphicsPipelineState();

            UC_ENGINE_API GraphicsPipelineState(GraphicsPipelineState&&) = default;
            UC_ENGINE_API GraphicsPipelineState& operator=(GraphicsPipelineState&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            GraphicsPipelineState();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<GraphicsPipelineState::Impl>;
    }
}


