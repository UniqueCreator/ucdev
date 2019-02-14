#pragma once

#include <uc_public/graphics/geometry/position_indexed_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class PositionIndexedGeometryInternal : public PositionIndexedGeometry
        {
            public:

            PositionIndexedGeometryInternal(uc::gx::dx12::managed_gpu_buffer&& vertices, uc::gx::dx12::managed_gpu_buffer&& indices);
        };
    }
}


