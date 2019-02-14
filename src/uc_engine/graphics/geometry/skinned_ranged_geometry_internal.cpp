#include "pch.h"

#include "skinned_ranged_geometry_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "skinned_ranged_geometry_impl.h"
#include "parametrized_indexed_geometry_impl.h"
#include "position_indexed_geometry_impl.h"
#include "indexed_geometry_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        SkinnedRangedGeometryInternal::SkinnedRangedGeometryInternal(managed_gpu_buffer&& blend_weights, managed_gpu_buffer&& blend_indices, managed_gpu_buffer&& uv, managed_gpu_buffer&& vertices, managed_gpu_buffer&& indices)
        {
            m_impl = SkinnedRangedGeometryInternal::Impl(std::move(blend_weights), std::move(blend_indices));
            ParametrizedIndexedGeometry::m_impl = std::move(uv);
            PositionIndexedGeometry::m_impl = std::move(vertices);
            IndexedGeometry::m_impl = std::move(indices);
        }
    }
}


