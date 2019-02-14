#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/geometry/ranged_parametrized_geometry.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle;

        class SkinnedRangedGeometry : public RangedParametrizedGeometry
        {
        public:

            class Impl;

            UC_ENGINE_API ~SkinnedRangedGeometry();

            UC_ENGINE_API SkinnedRangedGeometry(SkinnedRangedGeometry&&) = default;
            UC_ENGINE_API SkinnedRangedGeometry& operator=(SkinnedRangedGeometry&&);

            UC_ENGINE_API const Buffer* GetBlendWeightsBuffer() const;
            UC_ENGINE_API const Buffer* GetBlendIndicesBuffer() const;

            UC_ENGINE_API const VertexBufferView GetBlendWeightsView() const;
            UC_ENGINE_API const VertexBufferView GetBlendWeightsView(uint32_t offset, uint32_t size) const;

            UC_ENGINE_API const VertexBufferView GetBlendIndicesView() const;
            UC_ENGINE_API const VertexBufferView GetBlendIndicesView(uint32_t offset, uint32_t size) const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            SkinnedRangedGeometry();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedRangedGeometry::Impl>;
    }
}



