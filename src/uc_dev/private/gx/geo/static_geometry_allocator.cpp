#include "pch.h"

#include <uc_dev/gx/geo/static_geometry_allocator.h>
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

                static inline uint32_t make_index_size(uint32_t index_count)
                {
                    return index_count * default_geometry_index::stride::value;
                }

                static inline static_meshes_allocator::allocator_addresses make_static_addresses(
                    const dx12::gpu_buffer* positions,
                    const dx12::gpu_buffer* uv)
                {
                    static_meshes_allocator::allocator_addresses r;

                    {
                        r.m_positions       = positions->virtual_address();
                        r.m_positions_size  = static_cast<uint32_t>(size(positions));
                    }

                    {
                        r.m_uv = uv->virtual_address();
                        r.m_uv_size = static_cast<uint32_t>(size(uv));
                    }

                    return r;
                }
            }

            static_geometry_allocator::static_geometry_allocator(dx12::gpu_resource_create_context* rc, uint32_t vertex_count) :
            m_static_mesh_position (dx12::create_buffer(rc, details::make_positions_size( vertex_count)))
            , m_static_mesh_uv(dx12::create_buffer(rc, details::make_uv_size(vertex_count)))
            , m_static_meshes(details::make_static_addresses( m_static_mesh_position.get(), m_static_mesh_uv.get()))
            {

            }

            static_geometry_allocator::static_allocation*  static_geometry_allocator::allocate(size_t vertex_count)
            {
                return m_static_meshes.allocate(static_cast<uint32_t>(vertex_count));
            }

            void static_geometry_allocator::free(static_geometry_allocator::static_allocation* free)
            {
                m_static_meshes.free(free);
            }

            dx12::gpu_buffer* static_geometry_allocator::static_mesh_position() const
            {
                return m_static_mesh_position.get();
            }

            dx12::gpu_buffer* static_geometry_allocator::static_mesh_uv() const
            {
                return m_static_mesh_uv.get();
            }

            vertex_buffer_view   static_geometry_allocator::static_mesh_position_view() const
            {
                return m_static_meshes.view(gx::geo::static_meshes_allocator::component::position);
            }

            vertex_buffer_view   static_geometry_allocator::static_mesh_uv_view() const
            {
                return m_static_meshes.view(gx::geo::static_meshes_allocator::component::uv);
            }

            void static_geometry_allocator::sync()
            {
                m_static_meshes.sync();
            }
        }
    }
}

