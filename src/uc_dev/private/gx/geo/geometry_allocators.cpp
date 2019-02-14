#include "pch.h"

/*
#include <uc_dev/gx/geo/geometry_allocators.h>
#include <uc_dev/util/pimpl_impl.h>

#include <uc_dev/gx/geo/vertex_allocator.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            static inline uint32_t get_vertex_count(vertex_buffer_view v)
            {
                return v.SizeInBytes / v.StrideInBytes;
            }

            static inline vertex_buffer_view make_view(gpu_virtual_address address, size_t size, uint32_t stride)
            {
                vertex_buffer_view r;
                r.BufferLocation    = address;
                r.SizeInBytes       = static_cast<uint32_t>(size);
                r.StrideInBytes     = stride;
                return r;
            }

            class base_geometry_allocator_impl
            {
                public:
                vertex_buffer_view                      m_view;
                vertex_allocator                        m_allocator;
                uint32_t                                m_frame_index = 0;
                std::vector<vertex_allocator::handle>   m_pending_deletes[3];

                base_geometry_allocator_impl() : m_allocator(0)
                {

                }

                base_geometry_allocator_impl( vertex_buffer_view view) : m_view(view)
                    , m_allocator(get_vertex_count(view))
                {

                }
            };

            template uc::util::details::pimpl<base_geometry_allocator_impl>;

            base_geometry_allocator::base_geometry_allocator(vertex_buffer_view view) : m_impl(view)
            {

            }

            uint32_t base_geometry_allocator::stride() const
            {
                return m_impl->m_view.StrideInBytes;
            }

            uint32_t base_geometry_allocator::size() const
            {
                return m_impl->m_view.SizeInBytes;
            }

            gpu_virtual_address base_geometry_allocator::address() const
            {
                return m_impl->m_view.BufferLocation;
            }

            vertex_buffer_view base_geometry_allocator::view() const
            {
                return m_impl->m_view;
            }

            base_geometry_allocator::geometry_allocation base_geometry_allocator::allocate(uint32_t vertex_count)
            {
                auto r = m_impl->m_allocator.allocate(vertex_count);

                base_geometry_allocator::geometry_allocation v;
                v.m_opaque_handle   = r;
                v.m_vertex_count    = r->count();
                v.m_vertex_offset   = r->offset();
                return v;
            }

            void base_geometry_allocator::free(geometry_allocation free)
            {
                m_impl->m_pending_deletes[m_impl->m_frame_index].push_back(reinterpret_cast<vertex_allocator::handle>(free.m_opaque_handle));
            }

            void base_geometry_allocator::sync()
            {
                m_impl->m_frame_index += 1;
                m_impl->m_frame_index %= 3;

                for (auto&& i : m_impl->m_pending_deletes[m_impl->m_frame_index])
                {
                    m_impl->m_allocator.free(i);
                }
                m_impl->m_pending_deletes[m_impl->m_frame_index].resize(0);
            }
        }
    }
}
*/
