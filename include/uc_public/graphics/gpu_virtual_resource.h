#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class GpuVirtualResource : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API GpuVirtualResource(GpuVirtualResource&&) = default;
            UC_ENGINE_API GpuVirtualResource& operator=(GpuVirtualResource&&);

            UC_ENGINE_API GpuVirtualAddress GetVirtualAddress() const;

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            GpuVirtualResource();
            ~GpuVirtualResource();
            details::pimpl<Impl> m_impl;

        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<GpuVirtualResource::Impl>;
    }
}


