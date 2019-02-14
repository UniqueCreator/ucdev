#pragma once

#include <uc_public/graphics/geometry/skinned_ranged_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class SkinnedRangedGeometryInternal : public SkinnedRangedGeometry
        {
            public:
            using managed_gpu_buffer = uc::gx::dx12::managed_gpu_buffer;

            SkinnedRangedGeometryInternal::SkinnedRangedGeometryInternal(managed_gpu_buffer&& blend_weights, managed_gpu_buffer&& blend_indices, managed_gpu_buffer&& uv, managed_gpu_buffer&& vertices, managed_gpu_buffer&& indices);
        };
    }
}


