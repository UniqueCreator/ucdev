#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        enum SkinnedGeometryStride
        {
            Position        = 12,
            UV              = 8,
            BlendWeight     = 16,
            BlendIndex      = 4
        };

        class SkinnedGeometryAllocation : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API ~SkinnedGeometryAllocation();

            UC_ENGINE_API SkinnedGeometryAllocation(SkinnedGeometryAllocation&&) = default;
            UC_ENGINE_API SkinnedGeometryAllocation& operator=(SkinnedGeometryAllocation&&);

            UC_ENGINE_API uint32_t GetVertexOffset()   const;
            UC_ENGINE_API uint32_t GetVertexCount()    const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            SkinnedGeometryAllocation();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedGeometryAllocation::Impl>;
    }
}


