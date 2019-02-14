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

            class skinned_meshes_allocator final : public util::noncopyable
            {
                public:

                class skinned_meshes_allocator_impl;

                enum class component : uint32_t
                {
                    position = 0,
                    uv,
                    normal,
                    tangent,
                    blend_weight,
                    blend_index
                };

                class allocation
                {
                    private:

                    uint32_t                     m_vertex_count;
                    uint32_t                     m_vertex_offset;
                    void*                        m_opaque_handle;
                    skinned_meshes_allocator*    m_allocator;

                    public:

                    allocation(uint32_t vertex_count, uint32_t vertex_offset, void* opaque_handle, skinned_meshes_allocator* allocator);
                    
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
                    gpu_virtual_address m_normal;
                    gpu_virtual_address m_tangent;
                    gpu_virtual_address m_blend_weights;
                    gpu_virtual_address m_blend_indices;

                    uint32_t            m_positions_size;
                    uint32_t            m_uv_size;
                    uint32_t            m_normal_size;
                    uint32_t            m_tangent_size;
                    uint32_t            m_blend_weights_size;
                    uint32_t            m_blend_indices_size;
                };

                skinned_meshes_allocator( const allocator_addresses& options );

                uint32_t            stride(component) const;
                uint32_t            size(component) const;

                gpu_virtual_address address(component) const;
                vertex_buffer_view  view(component) const;

                allocation*         allocate(uint32_t vertex_count);

                void                free(allocation* free);
                void                sync();

                private:

                util::details::pimpl<skinned_meshes_allocator_impl> m_impl;
            };

            namespace details
            {
                struct skinned_meshes_allocation_deleter
                {
                    skinned_meshes_allocator* m_allocator = nullptr;

                    skinned_meshes_allocation_deleter() = default;
                    skinned_meshes_allocation_deleter(skinned_meshes_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (skinned_meshes_allocator::allocation* d ) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_skinned_mesh = std::unique_ptr< skinned_meshes_allocator::allocation, details::skinned_meshes_allocation_deleter >;

            template <typename ...args>
            inline managed_skinned_mesh make_skinned_mesh(skinned_meshes_allocator* rc, args&&... a)
            {
                return managed_skinned_mesh(rc->allocate(std::forward<args>(a)...), details::skinned_meshes_allocation_deleter(rc));
            }
        }
    }
}

