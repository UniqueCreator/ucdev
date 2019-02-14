#include "pch.h"

/*
#include <uc_dev/gx/geo/indexed_geometry_factory.h>

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
            indexed_geometry    create_indexed_geometry(gx::dx12::gpu_resource_create_context* rc, gx::dx12::gpu_upload_queue* upload_queue, const void* __restrict positions, size_t positions_size, const void* __restrict indices, size_t indices_size)
            {
                indexed_geometry r;

                auto sp = mem::align<256>(positions_size);
                auto si = mem::align<256>(indices_size);
                r.m_buffer = std::move(gx::dx12::create_buffer(rc, static_cast<uint32_t>(sp + si)));
                r.m_vertex_count = static_cast<uint32_t>(positions_size / indexed_geometry::position_stride);
                r.m_index_count = static_cast<uint32_t>(indices_size / indexed_geometry::index_stride);

                upload_queue->upload_buffer_gather(r.m_buffer.get(), positions, sp, positions_size, indices, si, indices_size);
                return r;
            }

            parametrized_geometry    create_parametrized_geometry(gx::dx12::gpu_resource_create_context* rc, gx::dx12::gpu_upload_queue* upload_queue, const void* __restrict positions, size_t positions_size, const void* __restrict indices, size_t indices_size, const void* __restrict uv, size_t uv_size)
            {
                parametrized_geometry r;

                auto sp = mem::align<256>(positions_size);
                auto si = mem::align<256>(indices_size);
                auto su = mem::align<256>(uv_size);

                r.m_buffer = std::move(gx::dx12::create_buffer(rc, static_cast<uint32_t>(sp + si + su)));
                r.m_vertex_count = static_cast<uint32_t>(positions_size / parametrized_geometry::position_stride);
                r.m_index_count = static_cast<uint32_t>(indices_size / parametrized_geometry::index_stride);

                upload_queue->upload_buffer_gather(r.m_buffer.get(), positions, sp, positions_size, indices, si, indices_size, uv, su, uv_size);
                return r;
            }

            multi_material_geometry    create_multi_material_geometry(gx::dx12::gpu_resource_create_context* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> indices, gsl::span<const gsl::byte> uv, const lip::reloc_array<lip::primitive_range>& ranges)
            {
                multi_material_geometry r;

                auto sp = mem::align<256>(positions.size());
                auto si = mem::align<256>(indices.size());
                auto su = mem::align<256>(uv.size());

                r.m_ranges.reserve(ranges.size());

                for (auto i : ranges)
                {
                    primitive_range range(i.m_begin, i.m_end);
                    r.m_ranges.push_back(std::move(range));
                }

                r.m_buffer = std::move(gx::dx12::create_buffer(rc, static_cast<uint32_t>(sp + si + su)));
                r.m_vertex_count = static_cast<uint32_t>(positions.size() / multi_material_geometry::position_stride);
                r.m_index_count = static_cast<uint32_t>(indices.size() / multi_material_geometry::index_stride);

                upload_queue->upload_buffer_gather(r.m_buffer.get(), positions.data(), sp, positions.size(), indices.data(), si, indices.size(), uv.data(), su, uv.size());
                return r;
            }

            skinned_geometry    create_skinned_geometry(gx::dx12::gpu_resource_create_context* rc, gx::dx12::gpu_upload_queue* upload_queue, gsl::span<const gsl::byte> positions, gsl::span<const gsl::byte> indices, gsl::span<const gsl::byte> uv, gsl::span<const gsl::byte> blend_weight, gsl::span<const gsl::byte> blend_indices, const lip::reloc_array<lip::primitive_range>& ranges)
            {
                skinned_geometry r;

                auto sp = mem::align<256>(positions.size());
                auto si = mem::align<256>(indices.size());
                auto su = mem::align<256>(uv.size());
                auto sbw = mem::align<256>(blend_weight.size());
                auto sbi = mem::align<256>(blend_indices.size());

                r.m_ranges.reserve(ranges.size());

                for (auto i : ranges)
                {
                    primitive_range range(i.m_begin, i.m_end);
                    r.m_ranges.push_back(std::move(range));
                }

                r.m_buffer = std::move(gx::dx12::create_buffer(rc, static_cast<uint32_t>(sp + si + su + sbw + sbi)));
                r.m_vertex_count = static_cast<uint32_t>(positions.size() / skinned_geometry::position_stride);
                r.m_index_count = static_cast<uint32_t>(indices.size() / skinned_geometry::index_stride);

                upload_queue->upload_buffer_gather(r.m_buffer.get(), positions.data(), sp, positions.size(), indices.data(), si, indices.size(), uv.data(), su, uv.size(), blend_weight.data(), sbw, blend_weight.size(), blend_indices.data(), sbi, blend_indices.size());
                return r;
            }
        }
    }
}
*/