#pragma once

#include <cstdint>

#include <uc_dev/util/pimpl.h>
#include <uc_dev/util/noncopyable.h>
#include <uc_dev/gx/dx12/gpu/managed_buffer.h>
#include "vertex_strides.h"

#include <d3d12.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            using vertex_buffer_view     = D3D12_VERTEX_BUFFER_VIEW;
            using gpu_virtual_address    = D3D12_GPU_VIRTUAL_ADDRESS;

            class normal_meshes_allocator final : public util::noncopyable
            {
                public:

                class normal_meshes_allocator_impl;

                enum class component : uint32_t
                {
                    position = 0,
                    uv,
                    normal
                };

                class allocation
                {
                    private:

                    uint32_t                     m_vertex_count;
                    uint32_t                     m_vertex_offset;
                    void*                        m_opaque_handle;
                    normal_meshes_allocator*     m_allocator;

                    public:

                    allocation(uint32_t vertex_count, uint32_t vertex_offset, void* opaque_handle, normal_meshes_allocator* allocator);
                    
                    void*    handle() const;
                    uint32_t draw_count() const;
                    uint32_t draw_offset() const;

                    uint32_t byte_size(component c) const;
                    uint32_t byte_offset(component c) const;

                    gpu_virtual_address base_address(component c) const;
                    gpu_virtual_address address(component c) const;
                };

                struct allocator_addresses
                {
                    gpu_virtual_address m_positions;
                    gpu_virtual_address m_uv;
                    gpu_virtual_address m_normals;

                    uint32_t            m_positions_size;
                    uint32_t            m_uv_size;
                    uint32_t            m_normals_size;
                };

                normal_meshes_allocator( const allocator_addresses& options );

                uint32_t            stride(component) const;
                uint32_t            size(component) const;

                gpu_virtual_address address(component) const;
                vertex_buffer_view  view(component) const;

                allocation*         allocate(uint32_t vertex_count);

                void                free(allocation* free);
                void                sync();

                private:

                util::details::pimpl<normal_meshes_allocator_impl> m_impl;
            };

            namespace details
            {
                struct normal_meshes_allocation_deleter
                {
                    normal_meshes_allocator* m_allocator = nullptr;

                    normal_meshes_allocation_deleter() = default;
                    normal_meshes_allocation_deleter(normal_meshes_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (normal_meshes_allocator::allocation* d ) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_normal_mesh = std::unique_ptr< normal_meshes_allocator::allocation, details::normal_meshes_allocation_deleter >;

            template <typename ...args>
            inline managed_normal_mesh make_normal_mesh(normal_meshes_allocator* rc, args&&... a)
            {
                return managed_normal_mesh(rc->allocate(std::forward<args>(a)...), details::normal_meshes_allocation_deleter(rc));
            }
        }
    }
}

