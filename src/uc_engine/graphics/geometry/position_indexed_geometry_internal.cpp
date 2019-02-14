#include "pch.h"

#include "position_indexed_geometry_internal.h"
#include <uc_public/uc_pimpl_impl.h>

#include "position_indexed_geometry_impl.h"
#include "indexed_geometry_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        PositionIndexedGeometryInternal::PositionIndexedGeometryInternal(uc::gx::dx12::managed_gpu_buffer&& vertices, uc::gx::dx12::managed_gpu_buffer&& indices)
        {
            m_impl = std::move(vertices);
            IndexedGeometry::m_impl = std::move(indices);
        }
    }
}


