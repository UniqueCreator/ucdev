#include "pch.h"

#include <uc_public/graphics/geometry/skinned_ranged_geometry.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_dev/gx/geo/indexed_geometry.h>
#include "skinned_ranged_geometry_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<SkinnedRangedGeometry::Impl>;

        SkinnedRangedGeometry::SkinnedRangedGeometry()
        {

        }

        SkinnedRangedGeometry::~SkinnedRangedGeometry() = default;
        SkinnedRangedGeometry& SkinnedRangedGeometry::operator=(SkinnedRangedGeometry&&) = default;

        SkinnedRangedGeometry::Impl*   SkinnedRangedGeometry::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedRangedGeometry::Impl*   SkinnedRangedGeometry::GetImpl() const
        {
            return m_impl.get();
        }

        const Buffer* SkinnedRangedGeometry::GetBlendWeightsBuffer() const
        {
            return nullptr;
        }

        const Buffer* SkinnedRangedGeometry::GetBlendIndicesBuffer() const
        {
            return nullptr;
        }

        const VertexBufferView SkinnedRangedGeometry::GetBlendWeightsView() const
        {
            VertexBufferView r;
            const auto&& v = GetBlendWeightsBuffer();
            r.m_bufferLocation = v->GetVirtualAddress();
            r.m_sizeInBytes = v->GetSize();
            r.m_strideInBytes = uc::gx::geo::default_geometry_blend_weight::value;
            return r;
        }

        const VertexBufferView SkinnedRangedGeometry::GetBlendWeightsView(uint32_t offsetInVertices, uint32_t vertexCount) const
        {
            VertexBufferView r;
            const auto stride = uc::gx::geo::default_geometry_blend_weight::value;
            const auto&& v = GetBlendWeightsBuffer();
            r.m_bufferLocation = v->GetVirtualAddress() + offsetInVertices * stride;
            r.m_sizeInBytes = vertexCount * stride;
            r.m_strideInBytes = stride;
            assert(r.m_sizeInBytes < v->GetSize());
            return r;
        }

        const VertexBufferView SkinnedRangedGeometry::GetBlendIndicesView() const
        {
            VertexBufferView r;
            const auto&& v      = GetBlendIndicesBuffer();
            r.m_bufferLocation  = v->GetVirtualAddress();
            r.m_sizeInBytes     = v->GetSize();
            r.m_strideInBytes   = uc::gx::geo::default_geometry_blend_index::value;
            return r;
        }

        const VertexBufferView SkinnedRangedGeometry::GetBlendIndicesView(uint32_t offset, uint32_t size) const
        {
            offset;
            size;
            return VertexBufferView();
        }
    }
}


