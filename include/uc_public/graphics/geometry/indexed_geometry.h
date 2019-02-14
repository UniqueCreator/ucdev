#pragma once

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class Buffer;

        class IndexedGeometry : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API ~IndexedGeometry();

            UC_ENGINE_API IndexedGeometry(IndexedGeometry&&) = default;
            UC_ENGINE_API IndexedGeometry& operator=(IndexedGeometry&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API const Buffer* GetIndicesBuffer() const;

            UC_ENGINE_API const IndexBufferView GetIndicesView() const;
            UC_ENGINE_API const IndexBufferView GetIndicesView(uint32_t offset, uint32_t size) const;

        protected:
            IndexedGeometry();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<IndexedGeometry::Impl>;
    }
}



