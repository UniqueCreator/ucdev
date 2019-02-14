#include "pch.h"

#include <uc_dev/gx/geo/indexed_mesh_allocator.h>

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
                static inline uint32_t get_index_count(uint64_t size_in_bytes, size_t stride_in_bytes)
                {
                    return static_cast<uint32_t>(size_in_bytes / stride_in_bytes);
                }

                static inline index_buffer_view make_view(gpu_virtual_address address, size_t size)
                {
                    index_buffer_view r;
                    r.BufferLocation = address;
                    r.SizeInBytes = static_cast<uint32_t>(size);
                    r.Format = DXGI_FORMAT_R32_UINT;
                    return r;
                }
            }

            class index_buffer_allocator::index_buffer_allocator_impl
            {
                public:
                gpu_virtual_address                     m_address;
                uint64_t                                m_size;
                static constexpr size_t                 m_stride = 4;
                vertex_allocator                        m_allocator;
                uint32_t                                m_frame_index = 0;
                std::vector<vertex_allocator::handle>   m_pending_deletes[3];

                index_buffer_allocator_impl() : m_allocator(0)
                {

                }

                index_buffer_allocator_impl(const gpu_virtual_address address, uint64_t size ) :
                    m_allocator(details::get_index_count(size, m_stride))
                {
                    m_address = address;
                    m_size = size;
                }
            };

            template uc::util::details::pimpl<index_buffer_allocator::index_buffer_allocator_impl>;

            index_buffer_allocator::index_buffer_allocator(const gpu_virtual_address address, uint64_t size) : m_impl(address, size)
            {

            }

            uint32_t index_buffer_allocator::stride() const
            {
                return static_cast<uint32_t>(m_impl->m_stride);
            }

            uint32_t index_buffer_allocator::size() const
            {
                return static_cast<uint32_t>(m_impl->m_size);
            }
            gpu_virtual_address index_buffer_allocator::address() const
            {
                return m_impl->m_address;
            }

            index_buffer_view  index_buffer_allocator::view( ) const
            {
                return details::make_view(address(), size());
            }

            index_buffer_allocator::allocation* index_buffer_allocator::allocate(uint32_t vertex_count)
            {
                auto r = m_impl->m_allocator.allocate(vertex_count);
                return new index_buffer_allocator::allocation(r->count(), r->offset(), r);
            }

            void index_buffer_allocator::free(index_buffer_allocator::allocation* free)
            {
                if (free)
                {
                    m_impl->m_pending_deletes[m_impl->m_frame_index].push_back(reinterpret_cast<vertex_allocator::handle>(free->handle()));
                    delete free;
                }
            }
            
            void index_buffer_allocator::sync()
            {
                m_impl->m_frame_index += 1;
                m_impl->m_frame_index %= 3;

                for (auto&& i : m_impl->m_pending_deletes[m_impl->m_frame_index])
                {
                    m_impl->m_allocator.free(i);
                }
                m_impl->m_pending_deletes[m_impl->m_frame_index].resize(0);
            }

            index_buffer_allocator::allocation::allocation(uint32_t index_count, uint32_t index_offset, void* opaque_handle) :
                m_index_count(index_count)
                , m_index_offset(index_offset)
                , m_opaque_handle(opaque_handle)
            {

            }

            void*    index_buffer_allocator::allocation::handle() const
            {
                return m_opaque_handle;
            }

            uint32_t index_buffer_allocator::allocation::index_count() const
            {
                return m_index_count;
            }

            uint32_t index_buffer_allocator::allocation::index_offset() const
            {
                return m_index_offset;
            }

            uint32_t index_buffer_allocator::allocation::byte_size() const
            {
                uint32_t stride = 4;
                return index_count() * stride;
            }

            uint32_t index_buffer_allocator::allocation::byte_offset() const
            {
                uint32_t stride = 4;
                return index_offset() * stride;
            }
        }
    }
}

