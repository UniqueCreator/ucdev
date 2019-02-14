#pragma once

#include <uc_public/graphics/geometry/indexed_geometry_factory.h>
#include <uc_public/graphics/resource_create_context.h>
#include <uc_dev/gx/geo/indexed_geometry_allocator.h>

#include "../buffer_internal.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        class IndexedGeometryFactory::Impl
        {
            public:

            IndexedGeometryFactory::Impl() {}
            IndexedGeometryFactory::Impl(ResourceCreateContext* rc, uint32_t vertexCount);

            uc::gx::geo::indexed_geometry_allocator* allocator() const;

            const Buffer* indices() const;

            private:
            ResourceCreateContext*  m_rc;

            std::unique_ptr<uc::gx::geo::indexed_geometry_allocator> m_index_allocator;
            BufferInternal2 m_indices;
        };
    }
}


