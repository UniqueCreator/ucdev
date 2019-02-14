#include "pch.h"

#include <uc_dev/gx/geo/normal_meshes_allocator.h>
#include <vector>
#include <uc_dev/util/pimpl_impl.h>
#include <uc_dev/gx/geo/vertex_allocator.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            namespace details
            {
                static inline uint32_t get_vertex_count(size_t size_in_bytes, size_t stride_in_bytes)
                {
                    return static_cast<uint32_t>(size_in_bytes / stride_in_bytes);
                }

                static inline vertex_buffer_view make_view(gpu_virtual_address address, size_t size, uint32_t stride)
                {
                    vertex_buffer_view r;
                    r.BufferLocation = address;
                    r.SizeInBytes = static_cast<uint32_t>(size);
                    r.StrideInBytes = stride;
                    return r;
                }
            }

            class normal_meshes_allocator::normal_meshes_allocator_impl
            {
                public:

                struct allocator_views
                {
                    gpu_virtual_address m_addresses[3];
                    uint32_t            m_sizes[3];
                    uint32_t            m_strides[3] = { default_geometry_position::stride::value, default_geometry_uv::stride::value, default_geometry_normal::stride::value };
                };

                allocator_views                         m_views;
                vertex_allocator                        m_allocator;
                uint32_t                                m_frame_index = 0;
                std::vector<vertex_allocator::handle>   m_pending_deletes[3];

                normal_meshes_allocator_impl() : m_allocator(0)
                {

                }

                normal_meshes_allocator_impl(const allocator_views& views) : m_views(views)
                    , m_allocator(details::get_vertex_count( views.m_sizes[0], views.m_strides[0]))
                {
                }
            };

            template uc::util::details::pimpl<normal_meshes_allocator::normal_meshes_allocator_impl>;

            namespace details
            {
                static inline normal_meshes_allocator::normal_meshes_allocator_impl::allocator_views make_views(const normal_meshes_allocator::allocator_addresses& options)
                {
                    normal_meshes_allocator::normal_meshes_allocator_impl::allocator_views r;

                    r.m_addresses[static_cast<uint32_t>(normal_meshes_allocator::component::position)] = options.m_positions;
                    r.m_addresses[static_cast<uint32_t>(normal_meshes_allocator::component::uv)] = options.m_uv;
                    r.m_addresses[static_cast<uint32_t>(normal_meshes_allocator::component::normal)] = options.m_normals;

                    r.m_sizes[static_cast<uint32_t>(normal_meshes_allocator::component::position)] = options.m_positions_size;
                    r.m_sizes[static_cast<uint32_t>(normal_meshes_allocator::component::uv)] = options.m_uv_size;
                    r.m_sizes[static_cast<uint32_t>(normal_meshes_allocator::component::normal)] = options.m_normals_size;

                    return r;
                }
            }

            normal_meshes_allocator::normal_meshes_allocator(const allocator_addresses& options) : m_impl(details::make_views(options))
            {
            }

            uint32_t normal_meshes_allocator::stride(normal_meshes_allocator::component c) const
            {
                return static_cast<uint32_t>(m_impl->m_views.m_strides[static_cast<uint32_t>(c)]);
            }

            uint32_t normal_meshes_allocator::size(normal_meshes_allocator::component c) const
            {
                return static_cast<uint32_t>(m_impl->m_views.m_sizes[static_cast<uint32_t>(c)]);
            }
            gpu_virtual_address normal_meshes_allocator::address(normal_meshes_allocator::component c) const
            {
                return m_impl->m_views.m_addresses[static_cast<uint32_t>(c)];
            }

            vertex_buffer_view  normal_meshes_allocator::view( normal_meshes_allocator::component c ) const
            {
                return details::make_view(address(c), size(c), stride(c));
            }

            normal_meshes_allocator::allocation* normal_meshes_allocator::allocate(uint32_t vertex_count)
            {
                auto r = m_impl->m_allocator.allocate(vertex_count);
                return new normal_meshes_allocator::allocation(r->count(), r->offset(), r, this);
            }

            void normal_meshes_allocator::free(normal_meshes_allocator::allocation* free)
            {
                if (free)
                {
                    m_impl->m_pending_deletes[m_impl->m_frame_index].push_back(reinterpret_cast<vertex_allocator::handle>(free->handle()));
                    delete free;
                }
            }
            
            void normal_meshes_allocator::sync()
            {
                m_impl->m_frame_index += 1;
                m_impl->m_frame_index %= 3;

                for (auto&& i : m_impl->m_pending_deletes[m_impl->m_frame_index])
                {
                    m_impl->m_allocator.free(i);
                }
                m_impl->m_pending_deletes[m_impl->m_frame_index].resize(0);
            }

            normal_meshes_allocator::allocation::allocation(uint32_t vertex_count, uint32_t vertex_offset, void* opaque_handle, normal_meshes_allocator* allocator) :
                m_vertex_count(vertex_count)
                , m_vertex_offset(vertex_offset)
                , m_opaque_handle(opaque_handle)
                , m_allocator( allocator )
            {

            }

            void*    normal_meshes_allocator::allocation::handle() const
            {
                return m_opaque_handle;
            }

            uint32_t normal_meshes_allocator::allocation::draw_count() const
            {
                return m_vertex_count;
            }

            uint32_t normal_meshes_allocator::allocation::draw_offset() const
            {
                return m_vertex_offset;
            }

            gpu_virtual_address normal_meshes_allocator::allocation::base_address(component c) const
            {
                return m_allocator->address(c);
            }

            gpu_virtual_address normal_meshes_allocator::allocation::address(component c) const
            {
                return base_address(c) + byte_offset(c);
            }

            uint32_t normal_meshes_allocator::allocation::byte_size(normal_meshes_allocator::component c) const
            {
                uint32_t strides[4] = { default_geometry_position::stride::value, default_geometry_uv::stride::value, default_geometry_blend_weight::stride::value, default_geometry_blend_index::stride::value };
                return draw_count() * strides[static_cast<uint32_t>(c)];
            }

            uint32_t normal_meshes_allocator::allocation::byte_offset(normal_meshes_allocator::component c) const
            {
                uint32_t strides[4] = { default_geometry_position::stride::value, default_geometry_uv::stride::value, default_geometry_blend_weight::stride::value, default_geometry_blend_index::stride::value };
                return draw_offset() * strides[static_cast<uint32_t>(c)];
            }
        }
    }
}

