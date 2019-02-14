#include "pch.h"

#include <uc_public/graphics/geometry/geometry_factory.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/graphics/graphics_types.h>
#include <uc_dev/gx/geo/indexed_geometry.h>

#include "geometry_factory_impl.h"

#include "position_indexed_geometry_internal.h"
#include "parametrized_indexed_geometry_internal.h"
#include "ranged_parametrized_geometry_internal.h"
#include "skinned_ranged_geometry_internal.h"
#include "../resource_create_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<GeometryFactory::Impl>;

        GeometryFactory::GeometryFactory(ResourceCreateContext* c) : m_impl(c)
        {

        }

        GeometryFactory::GeometryFactory()
        {

        }

        GeometryFactory::~GeometryFactory() = default;
        GeometryFactory& GeometryFactory::operator=(GeometryFactory&&) = default;

        GeometryFactory::Impl*   GeometryFactory::GetImpl()
        {
            return m_impl.get();
        }

        const GeometryFactory::Impl*   GeometryFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<PositionIndexedGeometry>      GeometryFactory::CreatePositionIndexedGeometry(uint32_t vertexCount, uint32_t indexCount)
        {
            auto&& rc = m_impl->rc();
            const auto stride = uc::gx::geo::default_geometry_position::stride::value;
            auto vertices   = uc::gx::dx12::create_buffer(rc->GetImpl()->rc(),vertexCount, stride);
            auto indices    = uc::gx::dx12::create_buffer(rc->GetImpl()->rc(), indexCount, 4);
            return std::make_unique<PositionIndexedGeometryInternal>( std::move(vertices), std::move(indices));
        }

        std::unique_ptr<ParametrizedIndexedGeometry>  GeometryFactory::CreateParametrizedIndexedGeometry(uint32_t vertexCount, uint32_t indexCount)
        {
            auto&& rc = m_impl->rc();
            const auto position_stride  = uc::gx::geo::default_geometry_position::stride::value;
            const auto uv_stride        = uc::gx::geo::default_geometry_uv::stride::value;
            auto vertices               = uc::gx::dx12::create_buffer(rc->GetImpl()->rc(), vertexCount, position_stride);
            auto indices                = uc::gx::dx12::create_buffer(rc->GetImpl()->rc(), indexCount, 4);
            auto uv                     = uc::gx::dx12::create_buffer(rc->GetImpl()->rc(), vertexCount, uv_stride);
            return std::make_unique<ParametrizedIndexedGeometryInternal>(std::move(uv), std::move(vertices), std::move(indices));
        }

        std::unique_ptr<SkinnedRangedGeometry>        GeometryFactory::CreateSkinnedRangedIndexedGeometry(uint32_t vertexCount, uint32_t indexCount)
        {

            auto&& rc = m_impl->rc();
            auto&& rcu = rc->GetImpl()->rc();

            const auto position_stride          = uc::gx::geo::default_geometry_position::stride::value;
            const auto uv_stride                = uc::gx::geo::default_geometry_uv::stride::value;
            const auto weights_stride           = uc::gx::geo::default_geometry_blend_weight::value;
            const auto weights_indices_stride   = uc::gx::geo::default_geometry_index::value;
            
            auto vertices                       = uc::gx::dx12::create_buffer(rcu, vertexCount, position_stride);
            auto indices                        = uc::gx::dx12::create_buffer(rcu, indexCount, 4);
            auto blend_weights                  = uc::gx::dx12::create_buffer(rcu, vertexCount, weights_stride);
            auto blend_indices                  = uc::gx::dx12::create_buffer(rcu, vertexCount, weights_indices_stride);

            auto uv = uc::gx::dx12::create_buffer(rcu, vertexCount, uv_stride);
            return std::make_unique<SkinnedRangedGeometryInternal>(std::move(blend_weights), std::move(blend_indices), std::move(uv), std::move(vertices), std::move(indices));
        }
    }
}


