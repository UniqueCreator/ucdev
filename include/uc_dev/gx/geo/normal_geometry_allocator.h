#pragma once

#include <cstdint>

#include "normal_meshes_allocator.h"
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
            class normal_geometry_allocator : public util::noncopyable
            {
                public:

                using normal_allocation = normal_meshes_allocator::allocation;

                normal_geometry_allocator(dx12::gpu_resource_create_context* c, uint32_t vertex_count);

                normal_allocation*   allocate(size_t vertex_count);
                void                 free(normal_allocation* free);

                dx12::gpu_buffer*    normal_mesh_position() const;
                dx12::gpu_buffer*    normal_mesh_uv() const;
                dx12::gpu_buffer*    normal_mesh_normal() const;

                vertex_buffer_view   normal_mesh_position_view() const;
                vertex_buffer_view   normal_mesh_uv_view() const;
                vertex_buffer_view   normal_mesh_normal_view() const;
                void                 sync();

                private:

                dx12::gpu_resource_create_context* m_ctx;

                dx12::managed_gpu_buffer           m_normal_mesh_position;
                dx12::managed_gpu_buffer           m_normal_mesh_uv;
                dx12::managed_gpu_buffer           m_normal_mesh_normal;
                normal_meshes_allocator            m_normal_meshes;
            };


            namespace details
            {
                struct normal_geometry_allocator_deleter
                {
                    normal_geometry_allocator* m_allocator = nullptr;

                    normal_geometry_allocator_deleter() = default;
                    normal_geometry_allocator_deleter(normal_geometry_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (normal_geometry_allocator::normal_allocation* d) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_normal_allocation = std::unique_ptr< normal_geometry_allocator::normal_allocation, details::normal_geometry_allocator_deleter >;

            template <typename ...args>
            inline managed_normal_allocation make_normal_allocation(normal_geometry_allocator* rc, args&&... a)
            {
                return managed_normal_allocation(rc->allocate(std::forward<args>(a)...), details::normal_geometry_allocator_deleter(rc));
            }
        }
    }
}

