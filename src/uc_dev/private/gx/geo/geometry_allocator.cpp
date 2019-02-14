#include "pch.h"

#include <uc_dev/gx/geo/geometry_allocator.h>
#include <uc_dev/gx/dx12/gpu/buffer.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            geometry_allocator::geometry_allocator(dx12::gpu_resource_create_context* rc, const geometry_allocator_options& options) :
              m_skinned_meshes(rc, options.m_skinned_mesh_vertex_count)
            , m_indices(rc, options.m_index_count)
            , m_static_meshes(rc, options.m_static_mesh_vertex_count)
            , m_normal_meshes(rc, options.m_normal_mesh_vertex_count)
            {



            }

            geometry_allocator::skinned_allocation*  geometry_allocator::allocate_skinned_geometry(size_t vertex_count)
            {
                return m_skinned_meshes.allocate(static_cast<uint32_t>(vertex_count));
            }

            void geometry_allocator::free_skinned_geometry(geometry_allocator::skinned_allocation* free)
            {
                m_skinned_meshes.free(free);
            }

            geometry_allocator::indexed_allocation*  geometry_allocator::allocate_indices(size_t index_count)
            {
                return m_indices.allocate(static_cast<uint32_t>(index_count));
            }

            void    geometry_allocator::free_indices(geometry_allocator::indexed_allocation* f)
            {
                m_indices.free(f);
            }

            dx12::gpu_buffer* geometry_allocator::skinned_mesh_position() const
            {
                return m_skinned_meshes.skinned_mesh_position();
            }

            dx12::gpu_buffer* geometry_allocator::skinned_mesh_uv() const
            {
                return m_skinned_meshes.skinned_mesh_uv();
            }

            dx12::gpu_buffer* geometry_allocator::skinned_mesh_normal() const
            {
                return m_skinned_meshes.skinned_mesh_normal();
            }

            dx12::gpu_buffer* geometry_allocator::skinned_mesh_tangent() const
            {
                return m_skinned_meshes.skinned_mesh_tangent();
            }

            dx12::gpu_buffer* geometry_allocator::skinned_mesh_blend_weight() const
            {
                return m_skinned_meshes.skinned_mesh_blend_weight();
            }

            dx12::gpu_buffer* geometry_allocator::skinned_mesh_blend_index() const
            {
                return m_skinned_meshes.skinned_mesh_blend_index();
            }

            dx12::gpu_buffer* geometry_allocator::indices() const
            {
                return m_indices.indices();
            }

            vertex_buffer_view   geometry_allocator::skinned_mesh_position_view() const
            {
                return m_skinned_meshes.skinned_mesh_position_view();
            }

            vertex_buffer_view   geometry_allocator::skinned_mesh_uv_view() const
            {
                return m_skinned_meshes.skinned_mesh_uv_view();
            }

            vertex_buffer_view   geometry_allocator::skinned_mesh_normal_view() const
            {
                return m_skinned_meshes.skinned_mesh_normal_view();
            }

            vertex_buffer_view   geometry_allocator::skinned_mesh_tangent_view() const
            {
                return m_skinned_meshes.skinned_mesh_tangent_view();
            }


            vertex_buffer_view   geometry_allocator::skinned_mesh_blend_weight_view() const
            {
                return m_skinned_meshes.skinned_mesh_blend_weight_view();
            }

            vertex_buffer_view   geometry_allocator::skinned_mesh_blend_index_view() const
            {
                return m_skinned_meshes.skinned_mesh_blend_index_view();
            }

            index_buffer_view geometry_allocator::indices_view() const
            {
                return m_indices.indices_view();
            }

            //--------------------------------------------------------------------------------------------------------
            geometry_allocator::static_allocation*  geometry_allocator::allocate_static_geometry(size_t vertex_count)
            {
                return m_static_meshes.allocate(static_cast<uint32_t>(vertex_count));
            }

            void geometry_allocator::free_static_geometry(geometry_allocator::static_allocation* free)
            {
                m_static_meshes.free(free);
            }
            
            dx12::gpu_buffer* geometry_allocator::parametrized_mesh_position() const
            {
                return m_static_meshes.static_mesh_position();
            }

            dx12::gpu_buffer*  geometry_allocator::parametrized_mesh_uv() const
            {
                return m_static_meshes.static_mesh_uv();
            }

            vertex_buffer_view  geometry_allocator::parametrized_mesh_position_view() const
            {
                return m_static_meshes.static_mesh_position_view();
            }

            vertex_buffer_view  geometry_allocator::parametrized_mesh_uv_view() const
            {
                return m_static_meshes.static_mesh_uv_view();
            }

            //--------------------------------------------------------------------------------------------------------
            geometry_allocator::normal_allocation*  geometry_allocator::allocate_normal_geometry(size_t vertex_count)
            {
                return m_normal_meshes.allocate(static_cast<uint32_t>(vertex_count));
            }

            void geometry_allocator::free_normal_geometry(geometry_allocator::normal_allocation* free)
            {
                m_normal_meshes.free(free);
            }

            dx12::gpu_buffer* geometry_allocator::normal_mesh_position() const
            {
                return m_normal_meshes.normal_mesh_position();
            }

            dx12::gpu_buffer*  geometry_allocator::normal_mesh_uv() const
            {
                return m_normal_meshes.normal_mesh_uv();
            }

            dx12::gpu_buffer*  geometry_allocator::normal_mesh_normal() const
            {
                return m_normal_meshes.normal_mesh_normal();
            }

            vertex_buffer_view  geometry_allocator::normal_mesh_position_view() const
            {
                return m_normal_meshes.normal_mesh_position_view();
            }

            vertex_buffer_view  geometry_allocator::normal_mesh_uv_view() const
            {
                return m_normal_meshes.normal_mesh_uv_view();
            }

            vertex_buffer_view  geometry_allocator::normal_mesh_normal_view() const
            {
                return m_normal_meshes.normal_mesh_normal_view();
            }

            void geometry_allocator::sync()
            {
                //todo: parallel
                m_skinned_meshes.sync();
                m_indices.sync();
                m_static_meshes.sync();
                m_normal_meshes.sync();
            }
        }
    }
}

