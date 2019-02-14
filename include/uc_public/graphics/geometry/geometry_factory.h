#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/geometry/position_indexed_geometry.h>
#include <uc_public/graphics/geometry/parametrized_indexed_geometry.h>
#include <uc_public/graphics/geometry/skinned_ranged_geometry.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ResourceCreateContext;

        class GeometryFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API GeometryFactory(ResourceCreateContext* c);

            UC_ENGINE_API ~GeometryFactory();

            UC_ENGINE_API GeometryFactory(GeometryFactory&&) = default;
            UC_ENGINE_API GeometryFactory& operator=(GeometryFactory&&);

            UC_ENGINE_API std::unique_ptr<PositionIndexedGeometry>      CreatePositionIndexedGeometry(uint32_t vertexCount, uint32_t indexCount );
            UC_ENGINE_API std::unique_ptr<ParametrizedIndexedGeometry>  CreateParametrizedIndexedGeometry(uint32_t vertexCount, uint32_t indexCount);
            UC_ENGINE_API std::unique_ptr<SkinnedRangedGeometry>        CreateSkinnedRangedIndexedGeometry(uint32_t vertexCount, uint32_t indexCount);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            GeometryFactory();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<GeometryFactory::Impl>;

        class GeometryFactoryUtils
        {

        };
    }
}


