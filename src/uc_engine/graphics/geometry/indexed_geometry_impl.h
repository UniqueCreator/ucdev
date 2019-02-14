#pragma once

#include <uc_public/graphics/geometry/indexed_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>
#include "../buffer_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class IndexedGeometry::Impl
        {
            public:

            IndexedGeometry::Impl() {}
            IndexedGeometry::Impl(uc::gx::dx12::managed_gpu_buffer&& indices) : m_indices(std::move(indices)) {}

            const Buffer* indices() const
            {
                return &m_indices;
            }

            Buffer* indices()
            {
                return &m_indices;
            }

            BufferInternal                   m_indices;
        };
    }
}


