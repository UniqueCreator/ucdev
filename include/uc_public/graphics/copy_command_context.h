#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/gpu_command_context.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class CopyCommandContext : public GpuCommandContext
        {
            using Base = GpuCommandContext;

            public:

            class Impl;
            UC_ENGINE_API ~CopyCommandContext();

            UC_ENGINE_API CopyCommandContext(CopyCommandContext&&) = default;
            UC_ENGINE_API CopyCommandContext& operator=(CopyCommandContext&&);

            protected:

            CopyCommandContext();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<CopyCommandContext::Impl>;
    }
}


