#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle;
        class GpuVirtualResource;

        class GpuCommandContext : private NonCopyable
        {
            public:

            class Impl;
            UC_ENGINE_API ~GpuCommandContext();

            UC_ENGINE_API GpuCommandContext(GpuCommandContext&&) = default;
            UC_ENGINE_API GpuCommandContext& operator=(GpuCommandContext&&);

            UC_ENGINE_API std::unique_ptr<FenceHandle> Submit();
            UC_ENGINE_API void SubmitAndWaitToExecute();
            UC_ENGINE_API void TransitionResource(const GpuVirtualResource* r, ResourceState old_state, ResourceState new_state);

            protected:

            GpuCommandContext();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<GpuCommandContext::Impl>;
    }
}


