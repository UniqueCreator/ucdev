#pragma once

#include <cstdint>

#include "indexed_mesh_allocator.h"
#include <uc_dev/gx/dx12/gpu/managed_buffer.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource_create_context;
        }

        namespace geo
        {
            class indexed_geometry_allocator : public util::noncopyable
            {
                public:

                using indexed_allocation = index_buffer_allocator::allocation;

                indexed_geometry_allocator(dx12::gpu_resource_create_context* c, uint32_t index_count);

                indexed_allocation*  allocate(size_t index_count);
                void                 free(indexed_allocation* f);
                dx12::gpu_buffer*    indices() const;
                index_buffer_view    indices_view() const;

                void                 sync();

                private:
                dx12::gpu_resource_create_context* m_ctx;
                dx12::managed_gpu_buffer           m_index_memory;
                index_buffer_allocator             m_indices;
            };

            namespace details
            {
                struct indexed_geometry_allocator_deleter
                {
                    indexed_geometry_allocator* m_allocator = nullptr;

                    indexed_geometry_allocator_deleter() = default;
                    indexed_geometry_allocator_deleter(indexed_geometry_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (indexed_geometry_allocator::indexed_allocation* d) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_indexed_allocation = std::unique_ptr< indexed_geometry_allocator::indexed_allocation, details::indexed_geometry_allocator_deleter >;

            template <typename ...args>
            inline managed_indexed_allocation make_indexed_allocation(indexed_geometry_allocator* rc, args&&... a)
            {
                return managed_indexed_allocation(rc->allocate(std::forward<args>(a)...), details::indexed_geometry_allocator_deleter(rc));
            }
        }
    }
}

