#include "pch.h"

#include <uc_dev/gx/geo/skinned_geometry_factory.h>
#include <uc_dev/gx/geo/geometry_allocator.h>

#include <uc_dev/mem/alloc.h>
#include <uc_dev/gx/dx12/cmd/upload_queue.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/gx/dx12/gpu/buffer.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            managed_skinned_mesh_geometry create_skinned_mesh(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> uv, gsl::span<const gsl::byte> normals, gsl::span<const gsl::byte> tangents,  gsl::span<const gsl::byte> blend_weight, gsl::span<const gsl::byte> blend_index)
            {
                size_t vertex_count = positions.size() / 12;    //stride

                managed_skinned_mesh_geometry r = make_managed_skinned_mesh_geometry(rc, vertex_count);

                upload_queue->upload_buffer(rc->skinned_mesh_position(), &positions[0], positions.size(), r->byte_offset(skinned_meshes_allocator::component::position));
                upload_queue->upload_buffer(rc->skinned_mesh_uv(), &uv[0], uv.size(), r->byte_offset(skinned_meshes_allocator::component::uv));
                upload_queue->upload_buffer(rc->skinned_mesh_normal(), &normals[0], normals.size(), r->byte_offset(skinned_meshes_allocator::component::normal));
                upload_queue->upload_buffer(rc->skinned_mesh_tangent(), &tangents[0], tangents.size(), r->byte_offset(skinned_meshes_allocator::component::tangent));
                upload_queue->upload_buffer(rc->skinned_mesh_blend_weight(), &blend_weight[0], blend_weight.size(), r->byte_offset(skinned_meshes_allocator::component::blend_weight));
                upload_queue->upload_buffer(rc->skinned_mesh_blend_index(), &blend_index[0], blend_index.size(), r->byte_offset(skinned_meshes_allocator::component::blend_index));
                
                return r;
            }

            managed_indexed_geometry create_indexed_geometry(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> indices)
            {
                size_t index_count = indices.size() / 4;    //stride

                managed_indexed_geometry r = make_managed_indexed_geometry(rc, index_count);

                upload_queue->upload_buffer(rc->indices(), &indices[0], indices.size(), r->byte_offset());

                return r;
            }

            managed_static_mesh_geometry create_static_mesh(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> uv)
            {
                size_t vertex_count = positions.size() / 12;    //stride

                managed_static_mesh_geometry r = make_managed_static_mesh_geometry(rc, vertex_count);
                upload_queue->upload_buffer(rc->parametrized_mesh_position(), &positions[0], positions.size(), r->byte_offset(static_meshes_allocator::component::position));
                upload_queue->upload_buffer(rc->parametrized_mesh_uv(), &uv[0], uv.size(), r->byte_offset(static_meshes_allocator::component::uv));

                return r;
            }

            managed_normal_mesh_geometry create_normal_mesh(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> uv, gsl::span<const gsl::byte> normal)
            {
                size_t vertex_count = positions.size() / 12;    //stride

                managed_normal_mesh_geometry r = make_managed_normal_mesh_geometry(rc, vertex_count);

                upload_queue->upload_buffer(rc->normal_mesh_position(), &positions[0], positions.size(), r->byte_offset(normal_meshes_allocator::component::position));
                upload_queue->upload_buffer(rc->normal_mesh_uv(), &uv[0], uv.size(), r->byte_offset(normal_meshes_allocator::component::uv));
                upload_queue->upload_buffer(rc->normal_mesh_normal(), &normal[0], normal.size(), r->byte_offset(normal_meshes_allocator::component::normal));
                
                return r;
            }

        }
    }
}