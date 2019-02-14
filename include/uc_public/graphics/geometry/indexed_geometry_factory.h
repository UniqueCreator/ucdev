#pragma once

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class Buffer;
        class ResourceCreateContext;
        class IndexedGeometryAllocation;


        class IndexedGeometryFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API IndexedGeometryFactory(ResourceCreateContext* c, uint32_t vertexCount);

            UC_ENGINE_API ~IndexedGeometryFactory();

            UC_ENGINE_API IndexedGeometryFactory(IndexedGeometryFactory&&) = default;
            UC_ENGINE_API IndexedGeometryFactory& operator=(IndexedGeometryFactory&&);

            UC_ENGINE_API std::unique_ptr<IndexedGeometryAllocation>    CreateIndexedGeometry(uint32_t indexCount);

            UC_ENGINE_API const Buffer*         GetIndices() const;
            UC_ENGINE_API const IndexBufferView GetIndicesView() const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API void Sync();

        protected:
            IndexedGeometryFactory();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<IndexedGeometryFactory::Impl>;
    }
}


