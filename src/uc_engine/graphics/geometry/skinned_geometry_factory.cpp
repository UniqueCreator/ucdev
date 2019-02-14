#include "pch.h"

#include <uc_public/graphics/geometry/skinned_geometry_factory.h>
#include <uc_public/graphics/geometry/skinned_geometry_allocation.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/graphics/graphics_types.h>

#include "skinned_geometry_allocation_impl.h"
#include "skinned_geometry_factory_impl.h"
#include "../resource_create_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class SkinnedGeometryAllocationInternal : public SkinnedGeometryAllocation
        {
        public:

            SkinnedGeometryAllocationInternal(uc::gx::geo::managed_skinned_allocation&& mesh)
            {
                m_impl = std::move(mesh);
            }
        };

        template details::pimpl<SkinnedGeometryFactory::Impl>;

        SkinnedGeometryFactory::SkinnedGeometryFactory(ResourceCreateContext* c, uint32_t vertexCount) : m_impl(c, vertexCount)
        {

        }

        SkinnedGeometryFactory::SkinnedGeometryFactory()
        {

        }

        SkinnedGeometryFactory::~SkinnedGeometryFactory() = default;
        SkinnedGeometryFactory& SkinnedGeometryFactory::operator=(SkinnedGeometryFactory&&) = default;

        SkinnedGeometryFactory::Impl*   SkinnedGeometryFactory::GetImpl()
        {
            return m_impl.get();
        }

        const SkinnedGeometryFactory::Impl*   SkinnedGeometryFactory::GetImpl() const
        {
            return m_impl.get();
        }

        void SkinnedGeometryFactory::Sync()
        {
            m_impl->allocator()->sync();
        }

        const VertexBufferView SkinnedGeometryFactory::GetPositionsView() const
        {
            VertexBufferView r;
            auto v = m_impl->allocator()->skinned_mesh_position_view();
            r.m_bufferLocation = v.BufferLocation;
            r.m_sizeInBytes = v.SizeInBytes;
            r.m_strideInBytes = v.StrideInBytes;
            return r;
        }

        const VertexBufferView SkinnedGeometryFactory::GetUVView() const
        {
            VertexBufferView r;
            auto v = m_impl->allocator()->skinned_mesh_uv_view();
            r.m_bufferLocation = v.BufferLocation;
            r.m_sizeInBytes = v.SizeInBytes;
            r.m_strideInBytes = v.StrideInBytes;
            return r;
        }

        const VertexBufferView SkinnedGeometryFactory::GetBlendWeightsView() const
        {
            VertexBufferView r;
            auto v = m_impl->allocator()->skinned_mesh_blend_weight_view();
            r.m_bufferLocation = v.BufferLocation;
            r.m_sizeInBytes = v.SizeInBytes;
            r.m_strideInBytes = v.StrideInBytes;
            return r;
        }

        const VertexBufferView SkinnedGeometryFactory::GetBlendIndicesView() const
        {
            VertexBufferView r;
            auto v = m_impl->allocator()->skinned_mesh_blend_index_view();
            r.m_bufferLocation = v.BufferLocation;
            r.m_sizeInBytes = v.SizeInBytes;
            r.m_strideInBytes = v.StrideInBytes;
            return r;
        }

        const Buffer*  SkinnedGeometryFactory::GetPositions() const
        {
            return m_impl->position();
        }

        const Buffer* SkinnedGeometryFactory::GetUV() const
        {
            return m_impl->uv();
        }

        const Buffer* SkinnedGeometryFactory::GetBlendWeights() const
        {
            return m_impl->blend_weight();
        }

        const Buffer* SkinnedGeometryFactory::GetBlendIndices() const
        {
            return m_impl->blend_index();
        }

        std::unique_ptr<SkinnedGeometryAllocation> SkinnedGeometryFactory::CreateSkinnedGeometry(uint32_t vertexCount)
        {
            return std::make_unique<SkinnedGeometryAllocationInternal>(uc::gx::geo::make_skinned_allocation(m_impl->allocator(), vertexCount));
        }
    }
}


