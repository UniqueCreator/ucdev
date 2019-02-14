#include "pch.h"

#include <uc_public/graphics/geometry/indexed_geometry_allocation.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/gx/geo/indexed_geometry_allocator.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class IndexedGeometryAllocation::Impl
        {
            public:
            Impl()
            {

            }

            Impl(uc::gx::geo::managed_indexed_allocation&& alloc) : m_alloc(std::move(alloc))
            {

            }
                
            uint32_t index_count() const
            {
                return m_alloc->index_count();
            }

            uint32_t index_offset() const
            {
                return m_alloc->index_offset();
            }

            protected:

            uc::gx::geo::managed_indexed_allocation m_alloc;
        };
    }
}


