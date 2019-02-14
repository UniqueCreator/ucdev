#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>

#include <uc_public/graphics/gpu_virtual_resource.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class Buffer : public GpuVirtualResource
        {
            public:

            class Impl;

            UC_ENGINE_API ~Buffer();

            UC_ENGINE_API Buffer(Buffer&&) = default;
            UC_ENGINE_API Buffer& operator=(Buffer&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API GpuVirtualAddress GetVirtualAddress() const;
            UC_ENGINE_API uint32_t          GetSize() const;

            protected:
            Buffer();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Buffer::Impl>;
    }
}


