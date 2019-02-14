#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/geometry/parametrized_indexed_geometry.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RangedParametrizedGeometry : public ParametrizedIndexedGeometry
        {
        public:

            class Impl;

            UC_ENGINE_API ~RangedParametrizedGeometry();

            UC_ENGINE_API RangedParametrizedGeometry(RangedParametrizedGeometry&&) = default;
            UC_ENGINE_API RangedParametrizedGeometry& operator=(RangedParametrizedGeometry&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            RangedParametrizedGeometry();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<RangedParametrizedGeometry::Impl>;
    }
}



