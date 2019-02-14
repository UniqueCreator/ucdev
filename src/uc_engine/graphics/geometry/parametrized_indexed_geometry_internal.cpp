#include "pch.h"

#include "parametrized_indexed_geometry_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "parametrized_indexed_geometry_impl.h"
#include "position_indexed_geometry_impl.h"
#include "indexed_geometry_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        ParametrizedIndexedGeometryInternal::ParametrizedIndexedGeometryInternal(managed_gpu_buffer&& uv, managed_gpu_buffer&& vertices, managed_gpu_buffer&& indices) 
        {
            m_impl = std::move(uv);
            PositionIndexedGeometry::m_impl = std::move(vertices);
            IndexedGeometry::m_impl = std::move(indices);
        }
    }
}


