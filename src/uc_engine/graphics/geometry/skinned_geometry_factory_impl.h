#pragma once

#include <uc_public/graphics/geometry/skinned_geometry_factory.h>
#include <uc_public/graphics/resource_create_context.h>
#include <uc_dev/gx/geo/skinned_geometry_allocator.h>

#include "../buffer_internal.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        class SkinnedGeometryFactory::Impl
        {
            public:

            SkinnedGeometryFactory::Impl() {}
            SkinnedGeometryFactory::Impl(ResourceCreateContext* rc, uint32_t vertexCount);

            uc::gx::geo::skinned_geometry_allocator* allocator() const;

            const Buffer* position() const;
            const Buffer* uv() const;
            const Buffer* blend_weight() const;
            const Buffer* blend_index() const;

            private:
            ResourceCreateContext*  m_rc;
            uint32_t                m_vertex_count;

            std::unique_ptr<uc::gx::geo::skinned_geometry_allocator> m_mesh_allocator;
            BufferInternal2 m_position;
            BufferInternal2 m_uv;
            BufferInternal2 m_blend_weight;
            BufferInternal2 m_blend_index;
        };
    }
}


