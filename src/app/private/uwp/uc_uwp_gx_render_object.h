#pragma once

#include <cstdint>
#include <vector>

#include <uc_dev/math/math.h>
#include <uc_dev/util/noncopyable.h>
#include <uc_dev/gx/geo/indexed_geometry.h>
#include <uc_dev/gx/dx12/gpu/managed_texture_2d.h>
#include <uc_dev/gx/geo/geometry_allocator.h>

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            struct skinned_render_object : private util::noncopyable
            {
                std::vector<gx::dx12::managed_gpu_texture_2d> m_opaque_textures;
                std::vector<gx::geo::primitive_range>         m_primitive_ranges;
                gx::geo::managed_indexed_geometry             m_indices;
                gx::geo::managed_skinned_mesh_geometry        m_geometry;
            };

            struct parametrized_render_object : private util::noncopyable
            {
                gx::geo::managed_indexed_geometry             m_indices;
                gx::geo::managed_static_mesh_geometry         m_geometry;
            };

            struct static_render_object : private util::noncopyable
            {
                std::vector<gx::dx12::managed_gpu_texture_2d> m_opaque_textures;
                std::vector<gx::geo::primitive_range>         m_primitive_ranges;
                gx::geo::managed_indexed_geometry             m_indices;
                gx::geo::managed_static_mesh_geometry         m_geometry;
            };
        }
    }
}
