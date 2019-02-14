#include "pch.h"

#include <uc_public/graphics/geometry/skinned_geometry_allocation.h>
#include <uc_public/uc_pimpl_impl.h>

#include <uc_dev/gx/geo/skinned_geometry_allocator.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class SkinnedGeometryAllocation::Impl
        {
        public:
            Impl()
            {

            }

            Impl(uc::gx::geo::managed_skinned_allocation&& alloc) : m_alloc(std::move(alloc))
            {

            }


            uint32_t vertex_count() const
            {
                return m_alloc->draw_count();
            }

            uint32_t vertex_offset() const
            {
                return m_alloc->draw_offset();
            }

        protected:

            uc::gx::geo::managed_skinned_allocation m_alloc;
        };
    }
}


