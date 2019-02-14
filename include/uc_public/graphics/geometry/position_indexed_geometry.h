#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/geometry/indexed_geometry.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class PositionIndexedGeometry : public IndexedGeometry
        {
        public:

            class Impl;

            UC_ENGINE_API ~PositionIndexedGeometry();

            UC_ENGINE_API PositionIndexedGeometry(PositionIndexedGeometry&&) = default;
            UC_ENGINE_API PositionIndexedGeometry& operator=(PositionIndexedGeometry&&);

            UC_ENGINE_API const Buffer* GetPositionsBuffer() const;

            UC_ENGINE_API const VertexBufferView GetPositionsView() const;
            UC_ENGINE_API const VertexBufferView GetPositionsView(uint32_t offset, uint32_t size) const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            PositionIndexedGeometry();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<PositionIndexedGeometry::Impl>;
    }
}



