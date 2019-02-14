#pragma once

#include "geometry_allocator.h"
#include <gsl/gsl>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_upload_queue;
        }

        namespace geo
        {
            managed_skinned_mesh_geometry create_skinned_mesh(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> uv, gsl::span<const gsl::byte> normals, gsl::span<const gsl::byte> tangents, gsl::span<const gsl::byte> blend_weight, gsl::span<const gsl::byte> blend_index);
            managed_indexed_geometry      create_indexed_geometry(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> indices);
            managed_static_mesh_geometry  create_static_mesh(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> uv);
            managed_normal_mesh_geometry  create_normal_mesh(geometry_allocator* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> uv, gsl::span<const gsl::byte> normal);
            
        }
    }
}

