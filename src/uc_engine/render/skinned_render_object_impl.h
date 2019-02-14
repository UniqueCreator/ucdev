#pragma once

#include <uc_public/render/skinned_render_object.h>

#include <uc_public/graphics/geometry/skinned_geometry_allocation.h>
#include <uc_public/graphics/geometry/indexed_geometry_allocation.h>
#include <uc_public/graphics/texture_2d.h>

#include <vector>

namespace UniqueCreator
{
    namespace Render
    {
        class SkinnedRenderObject::Impl
        {
        public:

            Impl() {}

            Impl(
                    std::unique_ptr<Graphics::IndexedGeometryAllocation>&& indices,
                    std::unique_ptr<Graphics::SkinnedGeometryAllocation>&& geometry,
                    std::vector< Graphics::PrimitiveRange>&& primitive_ranges,
                    std::vector< std::unique_ptr<Graphics::Texture2D> >&& textures

                ) : m_indices(std::move(indices))
                , m_geometry(std::move(geometry))
                , m_primitive_ranges(std::move(primitive_ranges))
                , m_texture_storage( std::move(textures))
            {
                auto s = m_texture_storage.size();
                m_texture_wrappers.resize( s );

                for ( auto i = 0U; i < s; ++i)
                {
                    m_texture_wrappers[i] = m_texture_storage[i].get();
                }

            }

            std::unique_ptr<Graphics::IndexedGeometryAllocation> m_indices;
            std::unique_ptr<Graphics::SkinnedGeometryAllocation> m_geometry;
            std::vector< Graphics::PrimitiveRange>               m_primitive_ranges;
            std::vector< std::unique_ptr<Graphics::Texture2D> >  m_texture_storage;
            std::vector< Graphics::Texture2D* >                  m_texture_wrappers;
        };
    }
}


