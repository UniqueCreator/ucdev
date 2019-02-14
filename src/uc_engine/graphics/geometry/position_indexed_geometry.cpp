#include "pch.h"

#include <uc_public/graphics/geometry/position_indexed_geometry.h>
#include <uc_public/uc_pimpl_impl.h>

#include "position_indexed_geometry_impl.h"
#include "../buffer_impl.h"
#include <uc_dev/gx/geo/indexed_geometry.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<PositionIndexedGeometry::Impl>;

        PositionIndexedGeometry::PositionIndexedGeometry()
        {

        }

        PositionIndexedGeometry::~PositionIndexedGeometry() = default;
        PositionIndexedGeometry& PositionIndexedGeometry::operator=(PositionIndexedGeometry&&) = default;

        PositionIndexedGeometry::Impl*   PositionIndexedGeometry::GetImpl()
        {
            return m_impl.get();
        }

        const PositionIndexedGeometry::Impl*   PositionIndexedGeometry::GetImpl() const
        {
            return m_impl.get();
        }

        const Buffer* PositionIndexedGeometry::GetPositionsBuffer() const
        {
            return m_impl->vertices();
        }

        const VertexBufferView PositionIndexedGeometry::GetPositionsView() const
        {
            VertexBufferView r;
            const auto&& v      = GetPositionsBuffer();
            r.m_bufferLocation  = v->GetVirtualAddress();
            r.m_sizeInBytes     = v->GetSize();
            r.m_strideInBytes   = uc::gx::geo::default_geometry_position::stride::value;
            return r;
        }

        const VertexBufferView PositionIndexedGeometry::GetPositionsView(uint32_t offsetInVertices, uint32_t vertexCount) const
        {
            VertexBufferView r;
            const auto stride = uc::gx::geo::default_geometry_position::stride::value;
            const auto&& v = GetPositionsBuffer();
            r.m_bufferLocation = v->GetVirtualAddress() + offsetInVertices * stride;
            r.m_sizeInBytes = vertexCount * stride;
            r.m_strideInBytes = stride;
            assert(r.m_sizeInBytes < v->GetSize());
            return r;
        }
    }
}


