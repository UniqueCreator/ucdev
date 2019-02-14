#pragma once

#include <uc_public/graphics/geometry/parametrized_indexed_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>
#include "../buffer_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class ParametrizedIndexedGeometry::Impl
        {
            public:

            ParametrizedIndexedGeometry::Impl() {}
            ParametrizedIndexedGeometry::Impl(uc::gx::dx12::managed_gpu_buffer&& uv) : m_uv(std::move(uv)) {}

            const Buffer* uv() const
            {
                return &m_uv;
            }

            Buffer* uv()
            {
                return &m_uv;
            }

            BufferInternal                   m_uv;
        };
    }
}


