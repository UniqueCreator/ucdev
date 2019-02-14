#pragma once

#include <uc_public/graphics/geometry/skinned_ranged_geometry.h>
#include <uc_dev/gx/dx12/dx12.h>
#include "../buffer_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class SkinnedRangedGeometry::Impl
        {
        public:

            SkinnedRangedGeometry::Impl() {}
            SkinnedRangedGeometry::Impl(uc::gx::dx12::managed_gpu_buffer&& blend_weights, uc::gx::dx12::managed_gpu_buffer&& blend_indices) : m_blend_weights(std::move(blend_weights)), m_blend_indices(std::move(blend_indices)) {}

            const Buffer* blend_weights() const
            {
                return &m_blend_weights;
            }

            Buffer* blend_weights()
            {
                return &m_blend_weights;
            }

            Buffer* blend_indices() 
            {
                return &m_blend_weights;
            }

            const Buffer* blend_indices() const
            {
                return &m_blend_indices;
            }

            BufferInternal  m_blend_weights;
            BufferInternal  m_blend_indices;
        };
    }
}


