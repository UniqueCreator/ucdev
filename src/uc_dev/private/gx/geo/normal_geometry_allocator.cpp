#include "pch.h"

#include <uc_dev/gx/geo/normal_geometry_allocator.h>
#include <uc_dev/gx/dx12/gpu/buffer.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            namespace details
            {
                static inline uint32_t make_positions_size( uint32_t vertex_count )
                {
                    return vertex_count * default_geometry_position::stride::value;
                }

                static inline uint32_t make_uv_size(uint32_t vertex_count)
                {
                    return vertex_count * default_geometry_uv::stride::value;
                }

                static inline uint32_t make_blend_weight_size(uint32_t vertex_count)
                {
                    return vertex_count * default_geometry_blend_weight::stride::value;
                }

                static inline uint32_t make_blend_index_size(uint32_t vertex_count)
                {
                    return vertex_count * default_geometry_blend_index::stride::value;
                }

                static inline uint32_t make_normal_size(uint32_t vertex_count)
                {
                    return vertex_count * default_geometry_normal::stride::value;
                }

                static inline uint32_t make_index_size(uint32_t index_count)
                {
                    return index_count * default_geometry_index::stride::value;
                }

                static inline normal_meshes_allocator::allocator_addresses make_normal_addresses(
                    const dx12::gpu_buffer* positions,
                    const dx12::gpu_buffer* uv,
                    const dx12::gpu_buffer* normal)
                {
                    normal_meshes_allocator::allocator_addresses r;

                    {
                        r.m_positions       = positions->virtual_address();
                        r.m_positions_size  = static_cast<uint32_t>(size(positions));
                    }

                    {
                        r.m_uv = uv->virtual_address();
                        r.m_uv_size = static_cast<uint32_t>(size(uv));
                    }

                    {
                        r.m_normals       = normal->virtual_address();
                        r.m_normals_size  = static_cast<uint32_t>(size(normal));
                    }

                    return r;
                }
            }

            normal_geometry_allocator::normal_geometry_allocator(dx12::gpu_resource_create_context* rc, uint32_t vertex_count) :
            m_normal_mesh_position (dx12::create_buffer(rc, details::make_positions_size( vertex_count)))
            , m_normal_mesh_uv(dx12::create_buffer(rc, details::make_uv_size(vertex_count)))
            , m_normal_mesh_normal(dx12::create_buffer(rc, details::make_normal_size(vertex_count)))
            , m_normal_meshes(details::make_normal_addresses( m_normal_mesh_position.get(), m_normal_mesh_uv.get(), m_normal_mesh_normal.get()))
            {

            }

            normal_geometry_allocator::normal_allocation*  normal_geometry_allocator::allocate(size_t vertex_count)
            {
                return m_normal_meshes.allocate(static_cast<uint32_t>(vertex_count));
            }

            void normal_geometry_allocator::free(normal_geometry_allocator::normal_allocation* free)
            {
                m_normal_meshes.free(free);
            }

            dx12::gpu_buffer* normal_geometry_allocator::normal_mesh_position() const
            {
                return m_normal_mesh_position.get();
            }

            dx12::gpu_buffer* normal_geometry_allocator::normal_mesh_uv() const
            {
                return m_normal_mesh_uv.get();
            }

            dx12::gpu_buffer* normal_geometry_allocator::normal_mesh_normal() const
            {
                return m_normal_mesh_normal.get();
            }

            vertex_buffer_view   normal_geometry_allocator::normal_mesh_position_view() const
            {
                return m_normal_meshes.view(gx::geo::normal_meshes_allocator::component::position);
            }

            vertex_buffer_view   normal_geometry_allocator::normal_mesh_uv_view() const
            {
                return m_normal_meshes.view(gx::geo::normal_meshes_allocator::component::uv);
            }

            vertex_buffer_view   normal_geometry_allocator::normal_mesh_normal_view() const
            {
                return m_normal_meshes.view(gx::geo::normal_meshes_allocator::component::normal);
            }

            void normal_geometry_allocator::sync()
            {
                m_normal_meshes.sync();
            }
        }
    }
}

