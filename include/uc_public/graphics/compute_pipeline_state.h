#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ComputePipelineState : private NonCopyable
        {

            public:

            class Impl;

            UC_ENGINE_API ~ComputePipelineState();

            UC_ENGINE_API ComputePipelineState(ComputePipelineState&&) = default;
            UC_ENGINE_API ComputePipelineState& operator=(ComputePipelineState&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            ComputePipelineState();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<ComputePipelineState::Impl>;
    }
}


