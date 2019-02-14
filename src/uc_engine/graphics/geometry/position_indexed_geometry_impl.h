#pragma once

#include <uc_public/graphics/geometry/position_indexed_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>
#include "../buffer_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class PositionIndexedGeometry::Impl
        {
        public:

            PositionIndexedGeometry::Impl() {}
            PositionIndexedGeometry::Impl(uc::gx::dx12::managed_gpu_buffer&& vertices) : m_vertices(std::move(vertices)) {}

            const Buffer* vertices() const
            {
                return &m_vertices;
            }

            Buffer* vertices()
            {
                return &m_vertices;
            }

            BufferInternal                   m_vertices;
        };
    }
}


