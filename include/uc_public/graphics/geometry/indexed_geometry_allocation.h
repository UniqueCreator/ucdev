#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        enum IndexedGeometryStride
        {
            Default = 4
        };

        class IndexedGeometryAllocation : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API ~IndexedGeometryAllocation();

            UC_ENGINE_API IndexedGeometryAllocation(IndexedGeometryAllocation&&) = default;
            UC_ENGINE_API IndexedGeometryAllocation& operator=(IndexedGeometryAllocation&&);

            UC_ENGINE_API uint32_t GetIndexOffset()   const;
            UC_ENGINE_API uint32_t GetIndexCount()    const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            IndexedGeometryAllocation();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<IndexedGeometryAllocation::Impl>;
    }
}


