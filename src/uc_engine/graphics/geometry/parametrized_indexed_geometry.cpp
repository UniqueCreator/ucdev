#include "pch.h"

#include <uc_public/graphics/geometry/parametrized_indexed_geometry.h>
#include <uc_public/uc_pimpl_impl.h>

#include "parametrized_indexed_geometry_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<ParametrizedIndexedGeometry::Impl>;

        ParametrizedIndexedGeometry::ParametrizedIndexedGeometry()
        {

        }

        ParametrizedIndexedGeometry::~ParametrizedIndexedGeometry() = default;
        ParametrizedIndexedGeometry& ParametrizedIndexedGeometry::operator=(ParametrizedIndexedGeometry&&) = default;

        ParametrizedIndexedGeometry::Impl*   ParametrizedIndexedGeometry::GetImpl()
        {
            return m_impl.get();
        }

        const ParametrizedIndexedGeometry::Impl*   ParametrizedIndexedGeometry::GetImpl() const
        {
            return m_impl.get();
        }

        const Buffer* ParametrizedIndexedGeometry::GetParametersBuffer() const
        {
            return nullptr;
        }

        const VertexBufferView ParametrizedIndexedGeometry::GetParametersView() const
        {
            return VertexBufferView();
        }

        const VertexBufferView ParametrizedIndexedGeometry::GetParametersView(uint32_t offset, uint32_t size) const
        {
            offset;
            size;
            return VertexBufferView();
        }
    }
}


