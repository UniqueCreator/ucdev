#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/gpu_command_context.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ComputePipelineState;

        class ComputeCommandContext : public GpuCommandContext
        {
            using Base = GpuCommandContext;

            public:

            class Impl;
            UC_ENGINE_API ~ComputeCommandContext();

            UC_ENGINE_API ComputeCommandContext(ComputeCommandContext&&) = default;
            UC_ENGINE_API ComputeCommandContext& operator=(ComputeCommandContext&&);

            UC_ENGINE_API void SetPipelineStateObject(const ComputePipelineState* state);
            UC_ENGINE_API void Dispatch(uint32_t threadGroupCountX, uint32_t threadGroupCountY, uint32_t threadGroupCountZ);

            protected:

            ComputeCommandContext();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<ComputeCommandContext::Impl>;
    }
}


