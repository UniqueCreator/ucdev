#pragma once

#include <cstdint>

#include "skinned_meshes_allocator.h"
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
            class skinned_geometry_allocator : public util::noncopyable
            {
                public:

                using skinned_allocation = skinned_meshes_allocator::allocation;

                skinned_geometry_allocator(dx12::gpu_resource_create_context* c, uint32_t vertex_count);

                skinned_allocation*  allocate(size_t vertex_count);
                void                 free(skinned_allocation* free);

                dx12::gpu_buffer*    skinned_mesh_position() const;
                dx12::gpu_buffer*    skinned_mesh_uv() const;
                dx12::gpu_buffer*    skinned_mesh_normal() const;
                dx12::gpu_buffer*    skinned_mesh_tangent() const;
                dx12::gpu_buffer*    skinned_mesh_blend_weight() const;
                dx12::gpu_buffer*    skinned_mesh_blend_index() const;
    

                vertex_buffer_view   skinned_mesh_position_view() const;
                vertex_buffer_view   skinned_mesh_uv_view() const;
                vertex_buffer_view   skinned_mesh_normal_view() const;
                vertex_buffer_view   skinned_mesh_tangent_view() const;
                vertex_buffer_view   skinned_mesh_blend_weight_view() const;
                vertex_buffer_view   skinned_mesh_blend_index_view() const;

                void                 sync();

                private:

                dx12::gpu_resource_create_context* m_ctx;

                dx12::managed_gpu_buffer           m_skinned_mesh_position;
                dx12::managed_gpu_buffer           m_skinned_mesh_uv;
                dx12::managed_gpu_buffer           m_skinned_mesh_normal;
                dx12::managed_gpu_buffer           m_skinned_mesh_tangent;
                dx12::managed_gpu_buffer           m_skinned_mesh_blend_weight;
                dx12::managed_gpu_buffer           m_skinned_mesh_blend_index;
                skinned_meshes_allocator           m_skinned_meshes;
            };


            namespace details
            {
                struct skinned_geometry_allocator_deleter
                {
                    skinned_geometry_allocator* m_allocator = nullptr;

                    skinned_geometry_allocator_deleter() = default;
                    skinned_geometry_allocator_deleter(skinned_geometry_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (skinned_geometry_allocator::skinned_allocation* d) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_skinned_allocation = std::unique_ptr< skinned_geometry_allocator::skinned_allocation, details::skinned_geometry_allocator_deleter >;

            template <typename ...args>
            inline managed_skinned_allocation make_skinned_allocation(skinned_geometry_allocator* rc, args&&... a)
            {
                return managed_skinned_allocation(rc->allocate(std::forward<args>(a)...), details::skinned_geometry_allocator_deleter(rc));
            }
        }
    }
}

