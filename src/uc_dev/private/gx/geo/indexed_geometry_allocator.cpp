#include "pch.h"

#include <uc_dev/gx/geo/indexed_geometry_allocator.h>
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
                static inline uint32_t make_index_size(uint32_t index_count)
                {
                    return index_count * default_geometry_index::stride::value;
                }

            }

            indexed_geometry_allocator::indexed_geometry_allocator(dx12::gpu_resource_create_context* rc, uint32_t index_count) :
              m_index_memory(dx12::create_buffer(rc, details::make_index_size(index_count)))
            , m_indices(m_index_memory->virtual_address(), size(m_index_memory.get()))
            {

            }

            indexed_geometry_allocator::indexed_allocation*  indexed_geometry_allocator::allocate(size_t index_count)
            {
                return m_indices.allocate(static_cast<uint32_t>(index_count));
            }

            void    indexed_geometry_allocator::free(indexed_geometry_allocator::indexed_allocation* f)
            {
                m_indices.free(f);
            }

            dx12::gpu_buffer* indexed_geometry_allocator::indices() const
            {
                return m_index_memory.get();
            }

            index_buffer_view indexed_geometry_allocator::indices_view() const
            {
                return m_indices.view();
            }

            void indexed_geometry_allocator::sync()
            {
                m_indices.sync();
            }
        }
    }
}

