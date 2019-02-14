#pragma once

#include <uc_public/graphics/geometry/parametrized_indexed_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ParametrizedIndexedGeometryInternal : public ParametrizedIndexedGeometry
        {
            public:
            using managed_gpu_buffer = uc::gx::dx12::managed_gpu_buffer;

            ParametrizedIndexedGeometryInternal(managed_gpu_buffer&& uv, managed_gpu_buffer&& vertices, managed_gpu_buffer&& indices);
        };
    }
}


