#include "pch.h"

#include <uc_public/graphics/geometry/ranged_parametrized_geometry.h>
#include <uc_public/uc_pimpl_impl.h>

#include "ranged_parametrized_geometry_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<RangedParametrizedGeometry::Impl>;

        RangedParametrizedGeometry::RangedParametrizedGeometry()
        {

        }

        RangedParametrizedGeometry::~RangedParametrizedGeometry() = default;
        RangedParametrizedGeometry& RangedParametrizedGeometry::operator=(RangedParametrizedGeometry&&) = default;

        RangedParametrizedGeometry::Impl*   RangedParametrizedGeometry::GetImpl()
        {
            return m_impl.get();
        }
    }
}


