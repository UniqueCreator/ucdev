#pragma once

#include <cstdint>

#include "indexed_geometry_allocator.h"
#include "normal_geometry_allocator.h"
#include "skinned_geometry_allocator.h"
#include "static_geometry_allocator.h"

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
            struct geometry_allocator_options
            {
                uint32_t m_index_count;
                uint32_t m_skinned_mesh_vertex_count;
                uint32_t m_static_mesh_vertex_count;
                uint32_t m_normal_mesh_vertex_count;
            };

            //wraps geometry allocations in one interface
            class geometry_allocator : public util::noncopyable
            {
                public:

                using skinned_allocation    = skinned_meshes_allocator::allocation;
                using indexed_allocation    = index_buffer_allocator::allocation;
                using static_allocation     = static_meshes_allocator::allocation;
                using normal_allocation     = normal_meshes_allocator::allocation;

                geometry_allocator(dx12::gpu_resource_create_context* c, const geometry_allocator_options& options);

                skinned_allocation*  allocate_skinned_geometry(size_t vertex_count);
                void                 free_skinned_geometry(skinned_allocation* free);

                indexed_allocation*  allocate_indices(size_t index_count);
                void                 free_indices(indexed_allocation* f);

                static_allocation*   allocate_static_geometry(size_t vertex_count);
                void                 free_static_geometry(static_allocation* free);

                normal_allocation*   allocate_normal_geometry(size_t vertex_count);
                void                 free_normal_geometry(normal_allocation* free);

                // skinned
                dx12::gpu_buffer*    skinned_mesh_position() const;
                dx12::gpu_buffer*    skinned_mesh_uv() const;
                dx12::gpu_buffer*    skinned_mesh_normal() const;
                dx12::gpu_buffer*    skinned_mesh_tangent() const;
                dx12::gpu_buffer*    skinned_mesh_blend_weight() const;
                dx12::gpu_buffer*    skinned_mesh_blend_index() const;
                dx12::gpu_buffer*    indices() const;

                vertex_buffer_view   skinned_mesh_position_view() const;
                vertex_buffer_view   skinned_mesh_uv_view() const;
                vertex_buffer_view   skinned_mesh_normal_view() const;
                vertex_buffer_view   skinned_mesh_tangent_view() const;
                vertex_buffer_view   skinned_mesh_blend_weight_view() const;
                vertex_buffer_view   skinned_mesh_blend_index_view() const;

                // indices
                index_buffer_view    indices_view() const;

                // static meshes
                dx12::gpu_buffer*    parametrized_mesh_position() const;
                dx12::gpu_buffer*    parametrized_mesh_uv() const;
                vertex_buffer_view   parametrized_mesh_position_view() const;
                vertex_buffer_view   parametrized_mesh_uv_view() const;

                // normal meshes
                dx12::gpu_buffer*    normal_mesh_position() const;
                dx12::gpu_buffer*    normal_mesh_uv() const;
                dx12::gpu_buffer*    normal_mesh_normal() const;
                vertex_buffer_view   normal_mesh_position_view() const;
                vertex_buffer_view   normal_mesh_uv_view() const;
                vertex_buffer_view   normal_mesh_normal_view() const;

                void                               sync();

                private:
                skinned_geometry_allocator         m_skinned_meshes;
                indexed_geometry_allocator         m_indices;
                static_geometry_allocator          m_static_meshes;
                normal_geometry_allocator          m_normal_meshes;
            };

            namespace details
            {
                struct skinned_mesh_geometry_allocator_deleter
                {
                    geometry_allocator* m_allocator = nullptr;

                    skinned_mesh_geometry_allocator_deleter() = default;
                    skinned_mesh_geometry_allocator_deleter(geometry_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (geometry_allocator::skinned_allocation* d) const
                    {
                        m_allocator->free_skinned_geometry(d);
                    }
                };

                struct index_geometry_allocator_deleter
                {
                    geometry_allocator* m_allocator = nullptr;

                    index_geometry_allocator_deleter() = default;
                    index_geometry_allocator_deleter(geometry_allocator* allocator) : m_allocator(allocator) {}

                    void operator () (geometry_allocator::indexed_allocation* d) const
                    {
                        m_allocator->free_indices(d);
                    }
                };

            struct static_mesh_geometry_allocator_deleter
            {
                geometry_allocator* m_allocator = nullptr;

                static_mesh_geometry_allocator_deleter() = default;
                static_mesh_geometry_allocator_deleter(geometry_allocator* allocator) : m_allocator(allocator) {}

                void operator () (geometry_allocator::static_allocation* d) const
                {
                    m_allocator->free_static_geometry(d);
                }
            };

            struct normal_mesh_geometry_allocator_deleter
            {
                geometry_allocator* m_allocator = nullptr;

                normal_mesh_geometry_allocator_deleter() = default;
                normal_mesh_geometry_allocator_deleter(geometry_allocator* allocator) : m_allocator(allocator) {}

                void operator () (geometry_allocator::normal_allocation* d) const
                {
                    m_allocator->free_normal_geometry(d);
                }
            };

        }

        using managed_skinned_mesh_geometry = std::unique_ptr< geometry_allocator::skinned_allocation, details::skinned_mesh_geometry_allocator_deleter >;

        template <typename ...args>
        inline managed_skinned_mesh_geometry make_managed_skinned_mesh_geometry(geometry_allocator* rc, args&&... a)
        {
            return managed_skinned_mesh_geometry(rc->allocate_skinned_geometry(std::forward<args>(a)...), details::skinned_mesh_geometry_allocator_deleter(rc));
        }

        using managed_indexed_geometry = std::unique_ptr< geometry_allocator::indexed_allocation, details::index_geometry_allocator_deleter >;

        template <typename ...args>
        inline managed_indexed_geometry make_managed_indexed_geometry(geometry_allocator* rc, args&&... a)
        {
                return managed_indexed_geometry(rc->allocate_indices(std::forward<args>(a)...), details::index_geometry_allocator_deleter(rc));
            }

            using managed_static_mesh_geometry = std::unique_ptr< geometry_allocator::static_allocation, details::static_mesh_geometry_allocator_deleter >;

            template <typename ...args>
            inline managed_static_mesh_geometry make_managed_static_mesh_geometry(geometry_allocator* rc, args&&... a)
            {
                return managed_static_mesh_geometry(rc->allocate_static_geometry(std::forward<args>(a)...), details::static_mesh_geometry_allocator_deleter(rc));
            }

            using managed_normal_mesh_geometry = std::unique_ptr< geometry_allocator::normal_allocation, details::normal_mesh_geometry_allocator_deleter >;

            template <typename ...args>
            inline managed_normal_mesh_geometry make_managed_normal_mesh_geometry(geometry_allocator* rc, args&&... a)
            {
                return managed_normal_mesh_geometry(rc->allocate_normal_geometry(std::forward<args>(a)...), details::normal_mesh_geometry_allocator_deleter(rc));
            }
        }
    }
}

