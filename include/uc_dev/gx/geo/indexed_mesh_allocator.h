#pragma once

#include <cstdint>

#include <uc_dev/util/noncopyable.h>
#include <uc_dev/util/pimpl.h>
#include <uc_dev/gx/dx12/gpu/managed_buffer.h>
#include "vertex_strides.h"

#include <d3d12.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            using index_buffer_view     = D3D12_INDEX_BUFFER_VIEW;
            using gpu_virtual_address   = D3D12_GPU_VIRTUAL_ADDRESS;

            class index_buffer_allocator final : public util::noncopyable
            {
                class index_buffer_allocator_impl;

                public:

                class allocation
                {
                    private:

                    uint32_t m_index_count;
                    uint32_t m_index_offset;
                    void*    m_opaque_handle;

                    public:

                    allocation(uint32_t index_count, uint32_t index_offset, void* opaque_handle);
                    
                    void*    handle() const;
                    uint32_t index_count() const;
                    uint32_t index_offset() const;

                    uint32_t byte_size() const;
                    uint32_t byte_offset() const;
                };

                index_buffer_allocator( const gpu_virtual_address address, uint64_t size);

                uint32_t            stride() const;
                uint32_t            size() const;

                gpu_virtual_address address() const;
                index_buffer_view   view() const;

                allocation*         allocate(uint32_t vertex_count);

                void                free(allocation* free);
                void                sync();

                private:

                util::details::pimpl<index_buffer_allocator_impl> m_impl;
            };

            namespace details
            {
                struct index_buffer_allocation_deleter
                {
                    index_buffer_allocator* m_allocator = nullptr;

                    index_buffer_allocation_deleter() = default;
                    index_buffer_allocation_deleter(index_buffer_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (index_buffer_allocator::allocation* d ) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_indexed_mesh = std::unique_ptr< index_buffer_allocator::allocation, details::index_buffer_allocation_deleter >;

            template <typename ...args>
            inline managed_indexed_mesh make_index_buffer(index_buffer_allocator* rc, args&&... a)
            {
                return managed_skinned_mesh(rc->allocate(std::forward<args>(a)...), details::indexed_meshes_allocation_deleter(rc));
            }
        }
    }
}

