#include "pch.h"
/*
#include <uc_dev/gx/geo/indexed_geometry.h>

#include <uc_dev/mem/alloc.h>
#include <uc_dev/gx/dx12/gpu/buffer.h>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            uint64_t size(const indexed_geometry* g)
            {
                return size(g->m_buffer.get());
            }

            uint64_t size(const indexed_geometry& g)
            {
                return size(g.m_buffer.get());
            }

            uint64_t size(const parametrized_geometry* g)
            {
                return size(g->m_buffer.get());
            }

            uint64_t size(const parametrized_geometry& g)
            {
                return size(g.m_buffer.get());
            }

            D3D12_VERTEX_BUFFER_VIEW make_position_buffer_view(const indexed_geometry* g, uint32_t offset)
            {
                D3D12_VERTEX_BUFFER_VIEW r = {};
                constexpr auto element_size = indexed_geometry::position_stride;
                r.BufferLocation = g->m_buffer->virtual_address() + offset * element_size;
                r.SizeInBytes = (g->m_vertex_count - offset) * element_size;
                r.StrideInBytes = element_size;
                return r;
            }

            D3D12_INDEX_BUFFER_VIEW make_index_buffer_view(const indexed_geometry* g, uint32_t begin, uint32_t end)
            {
                D3D12_VERTEX_BUFFER_VIEW r0 = make_position_buffer_view(g);
                D3D12_INDEX_BUFFER_VIEW  r1 = {};

                if (end == 0)
                {
                    end = g->m_index_count;
                }

                //indices are right after the geometry
                //we save managing many buffers in this way
                constexpr auto element_size = indexed_geometry::index_stride;
                r1.BufferLocation = g->m_buffer->virtual_address() + mem::align<256>(r0.SizeInBytes) + begin * element_size;
                r1.SizeInBytes = (end - begin) * element_size;

                static_assert(element_size == 4 || element_size == 2, "Invalid element size");
                r1.Format = element_size == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
                return r1;
            }

            D3D12_INDEX_BUFFER_VIEW make_index_buffer_view(const parametrized_geometry* g, uint32_t begin, uint32_t end)
            {
                D3D12_VERTEX_BUFFER_VIEW r0 = make_position_buffer_view(g);
                D3D12_INDEX_BUFFER_VIEW  r1 = {};

                if (end == 0)
                {
                    end = g->m_index_count;
                }

                //indices are right after the geometry
                //we save managing many buffers in this way
                constexpr auto element_size = indexed_geometry::index_stride;
                r1.BufferLocation = g->m_buffer->virtual_address() + mem::align<256>(r0.SizeInBytes) + begin * element_size;
                r1.SizeInBytes = (end - begin) * element_size;

                static_assert(element_size == 4 || element_size == 2, "Invalid element size");
                r1.Format = element_size == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
                return r1;
            }

            D3D12_VERTEX_BUFFER_VIEW make_uv_buffer_view(const parametrized_geometry* g, uint32_t offset)
            {
                D3D12_VERTEX_BUFFER_VIEW r0 = make_position_buffer_view(g);
                D3D12_INDEX_BUFFER_VIEW  r1 = make_index_buffer_view(g);

                D3D12_VERTEX_BUFFER_VIEW r = {};

                //uv's are located right after the indices
                constexpr auto element_size = parametrized_geometry::uv_stride;
                r.BufferLocation = g->m_buffer->virtual_address() + mem::align<256>(r0.SizeInBytes) + mem::align<256>(r1.SizeInBytes) + offset * element_size;
                r.SizeInBytes = (g->m_vertex_count - offset) * element_size;
                r.StrideInBytes = element_size;
                return r;
            }

            D3D12_VERTEX_BUFFER_VIEW make_blend_weights_buffer_view(const skinned_geometry* g, uint32_t offset)
            {
                D3D12_VERTEX_BUFFER_VIEW  r0 = make_position_buffer_view(g);
                D3D12_INDEX_BUFFER_VIEW   r1 = make_index_buffer_view(g);
                D3D12_VERTEX_BUFFER_VIEW  r2 = make_uv_buffer_view(g);

                D3D12_VERTEX_BUFFER_VIEW r = {};

                //uv's are located right after the indices
                constexpr auto element_size = skinned_geometry::blend_weights_stride;
                r.BufferLocation = g->m_buffer->virtual_address() + mem::align<256>(r0.SizeInBytes) + mem::align<256>(r1.SizeInBytes) + mem::align<256>(r2.SizeInBytes) + offset * element_size;
                r.SizeInBytes = (g->m_vertex_count - offset) * element_size;
                r.StrideInBytes = element_size;
                return r;
            }

            D3D12_VERTEX_BUFFER_VIEW make_blend_indices_buffer_view(const skinned_geometry* g, uint32_t offset)
            {
                D3D12_VERTEX_BUFFER_VIEW  r0 = make_position_buffer_view(g);
                D3D12_INDEX_BUFFER_VIEW   r1 = make_index_buffer_view(g);
                D3D12_VERTEX_BUFFER_VIEW  r2 = make_uv_buffer_view(g);
                D3D12_VERTEX_BUFFER_VIEW  r3 = make_blend_weights_buffer_view(g);

                D3D12_VERTEX_BUFFER_VIEW r = {};

                //uv's are located right after the indices
                constexpr auto element_size = skinned_geometry::blend_indices_stride;
                r.BufferLocation = g->m_buffer->virtual_address() + mem::align<256>(r0.SizeInBytes) + mem::align<256>(r1.SizeInBytes) + mem::align<256>(r2.SizeInBytes) + mem::align<256>(r3.SizeInBytes) + offset * element_size;
                r.SizeInBytes = (g->m_vertex_count - offset) * element_size;
                r.StrideInBytes = element_size;

                return r;
            }
        }
    }
}

*/
