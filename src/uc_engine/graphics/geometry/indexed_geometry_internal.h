#pragma once

#include <uc_public/graphics/geometry/indexed_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class IndexedGeometryInternal : public IndexedGeometry
        {
            public:

            IndexedGeometryInternal();
            IndexedGeometryInternal(uc::gx::dx12::managed_gpu_buffer&& indices);
        };
    }
}


