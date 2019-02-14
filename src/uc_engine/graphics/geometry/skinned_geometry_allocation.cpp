#include "pch.h"

#include <uc_public/graphics/geometry/skinned_geometry_allocation.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/gx/geo/skinned_geometry_allocator.h>

#include "skinned_geometry_allocation_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<SkinnedGeometryAllocation::Impl>;

        SkinnedGeometryAllocation::SkinnedGeometryAllocation()
        {

        }

        SkinnedGeometryAllocation::~SkinnedGeometryAllocation() = default;
        SkinnedGeometryAllocation& SkinnedGeometryAllocation::operator=(SkinnedGeometryAllocation&&) = default;

        SkinnedGeometryAllocation::Impl*   SkinnedGeometryAllocation::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedGeometryAllocation::Impl*   SkinnedGeometryAllocation::GetImpl() const
        {
            return m_impl.get();
        }

        uint32_t SkinnedGeometryAllocation::GetVertexOffset()   const
        {
            return m_impl->vertex_offset();
        }

        uint32_t SkinnedGeometryAllocation::GetVertexCount()    const
        {
            return m_impl->vertex_count();
        }
    }
}


