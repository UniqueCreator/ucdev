#pragma once

#include <cstdint>

#include "static_meshes_allocator.h"
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
            class static_geometry_allocator : public util::noncopyable
            {
                public:

                using static_allocation = static_meshes_allocator::allocation;

                static_geometry_allocator(dx12::gpu_resource_create_context* c, uint32_t vertex_count);

                static_allocation*  allocate(size_t vertex_count);
                void                 free(static_allocation* free);

                dx12::gpu_buffer*    static_mesh_position() const;
                dx12::gpu_buffer*    static_mesh_uv() const;

                vertex_buffer_view   static_mesh_position_view() const;
                vertex_buffer_view   static_mesh_uv_view() const;

                void                 sync();

                private:

                dx12::gpu_resource_create_context* m_ctx;

                dx12::managed_gpu_buffer           m_static_mesh_position;
                dx12::managed_gpu_buffer           m_static_mesh_uv;
                static_meshes_allocator            m_static_meshes;
            };


            namespace details
            {
                struct static_geometry_allocator_deleter
                {
                    static_geometry_allocator* m_allocator = nullptr;

                    static_geometry_allocator_deleter() = default;
                    static_geometry_allocator_deleter(static_geometry_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (static_geometry_allocator::static_allocation* d) const
                    {
                        m_allocator->free(d);
                    }
                };
            }

            using managed_static_allocation = std::unique_ptr< static_geometry_allocator::static_allocation, details::static_geometry_allocator_deleter >;

            template <typename ...args>
            inline managed_static_allocation make_static_allocation(static_geometry_allocator* rc, args&&... a)
            {
                return managed_static_allocation(rc->allocate(std::forward<args>(a)...), details::static_geometry_allocator_deleter(rc));
            }
        }
    }
}

