#include "pch.h"

#include <uc_public/graphics/geometry/indexed_geometry_allocation.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/gx/geo/indexed_mesh_allocator.h>

#include "indexed_geometry_allocation_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<IndexedGeometryAllocation::Impl>;

        IndexedGeometryAllocation::IndexedGeometryAllocation()
        {

        }

        IndexedGeometryAllocation::~IndexedGeometryAllocation() = default;
        IndexedGeometryAllocation& IndexedGeometryAllocation::operator=(IndexedGeometryAllocation&&) = default;

        IndexedGeometryAllocation::Impl*   IndexedGeometryAllocation::GetImpl()
        {
            return m_impl.get();
        }

        const IndexedGeometryAllocation::Impl*   IndexedGeometryAllocation::GetImpl() const
        {
            return m_impl.get();
        }

        uint32_t IndexedGeometryAllocation::GetIndexOffset()   const
        {
            return m_impl->index_offset();
        }

        uint32_t IndexedGeometryAllocation::GetIndexCount()    const
        {
            return m_impl->index_count();
        }
    }
}


