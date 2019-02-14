#include "pch.h"

#include "uc_uwp_gx_render_object_factory.h"

#include <gsl/gsl>

#include <uc_dev/gx/geo/skinned_geometry_factory.h>
#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include "uc_uwp_device_resources.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            /*
            std::unique_ptr<skinned_multi_material_render_object> render_object_factory< skinned_multi_material_render_object >::make_render_object(const wchar_t* file_name, device_resources* resources)
            {
                std::unique_ptr< skinned_multi_material_render_object > r = std::make_unique< skinned_multi_material_render_object  >();

                auto mesh = lip::create_from_compressed_lip_file<lip::normal_skinned_model>(file_name);

                auto span_uv            = gsl::make_span(mesh->m_uv.data(), mesh->m_uv.size());
                auto span_normals       = gsl::make_span(mesh->m_normals.data(), mesh->m_normals.size());
                auto span_indices       = gsl::make_span(mesh->m_indices.data(), mesh->m_indices.size());
                auto span_positions     = gsl::make_span(mesh->m_positions.data(), mesh->m_positions.size());
                auto span_blend_weights = gsl::make_span(mesh->m_blend_weights.data(), mesh->m_blend_weights.size());
                auto span_blend_indices = gsl::make_span(mesh->m_blend_indices.data(), mesh->m_blend_indices.size());
                auto primitive_ranges   = gsl::make_span(mesh->m_primitive_ranges.cbegin(), mesh->m_primitive_ranges.cend());

                r->m_geometry = gx::geo::create_skinned_geometry(resources->resource_create_context(), resources->upload_queue(), gsl::as_bytes(span_positions), gsl::as_bytes(span_indices), gsl::as_bytes(span_uv), gsl::as_bytes(span_blend_weights), gsl::as_bytes(span_blend_indices), mesh->m_primitive_ranges);

                r->m_opaque_textures.resize(mesh->m_textures.size());

                for (auto i = 0U; i < mesh->m_textures.size(); ++i)
                {
                    auto& texture = mesh->m_textures[i];

                    auto w = texture.m_width;
                    auto h = texture.m_height;

                    r->m_opaque_textures[i] = gx::dx12::create_texture_2d(resources->resource_create_context(), w, h, static_cast<DXGI_FORMAT>(texture.view()));

                    D3D12_SUBRESOURCE_DATA s = gx::sub_resource_data(&texture);
                    resources->upload_queue()->upload_texture_2d(r->m_opaque_textures[i].get(), 0, 1, &s);
                }

                resources->compute_queue()->insert_wait_on(resources->upload_queue()->flush());

                return r;
            }
            */

            std::unique_ptr< skinned_render_object > render_object_factory< skinned_render_object >::make_render_object(const wchar_t* file_name, device_resources* resources, gx::geo::geometry_allocator* allocator)
            {
                std::unique_ptr< skinned_render_object > r = std::make_unique< skinned_render_object >();

                auto mesh = lip::create_from_compressed_lip_file<lip::derivatives_skinned_model>(file_name);

                auto span_uv            = gsl::make_span(mesh->m_uv.data(), mesh->m_uv.size());
                auto span_normals       = gsl::make_span(mesh->m_normals.data(), mesh->m_normals.size());
                auto span_tangents      = gsl::make_span(mesh->m_tangents.data(), mesh->m_tangents.size());
                auto span_indices       = gsl::make_span(mesh->m_indices.data(), mesh->m_indices.size());
                auto span_positions     = gsl::make_span(mesh->m_positions.data(), mesh->m_positions.size());
                auto span_blend_weights = gsl::make_span(mesh->m_blend_weights.data(), mesh->m_blend_weights.size());
                auto span_blend_indices = gsl::make_span(mesh->m_blend_indices.data(), mesh->m_blend_indices.size());

                r->m_geometry           = gx::geo::create_skinned_mesh(allocator, resources->upload_queue(), gsl::as_bytes(span_positions), gsl::as_bytes(span_uv), gsl::as_bytes(span_normals), gsl::as_bytes(span_tangents), gsl::as_bytes(span_blend_weights), gsl::as_bytes(span_blend_indices));
                r->m_indices            = gx::geo::create_indexed_geometry(allocator, resources->upload_queue(), gsl::as_bytes(span_indices) );

                r->m_opaque_textures.resize(mesh->m_textures.size());

                for (auto i = 0U; i < mesh->m_textures.size(); ++i)
                {
                    auto& texture = mesh->m_textures[i];

                    auto w = texture.m_levels[0].m_width;
                    auto h = texture.m_levels[0].m_height;

                    r->m_opaque_textures[i] = gx::dx12::create_texture_2d(resources->resource_create_context(), w, h, static_cast<DXGI_FORMAT>(texture.m_levels[0].view()));

                    D3D12_SUBRESOURCE_DATA s = gx::sub_resource_data(&texture.m_levels[0]);
                    resources->upload_queue()->upload_texture_2d(r->m_opaque_textures[i].get(), 0, 1, &s);
                }

                r->m_primitive_ranges.reserve(mesh->m_primitive_ranges.size());

                for (auto&& i : mesh->m_primitive_ranges)
                {
                    gx::geo::primitive_range range(i.m_begin, i.m_end);
                    r->m_primitive_ranges.push_back(std::move(range));
                }

                resources->compute_queue()->insert_wait_on(resources->upload_queue()->flush());

                return r;
            }

            std::unique_ptr< parametrized_render_object > render_object_factory< parametrized_render_object >::make_render_object(const wchar_t* file_name, device_resources* resources, gx::geo::geometry_allocator* allocator)
            {
                std::unique_ptr< parametrized_render_object > r = std::make_unique< parametrized_render_object  >();

                auto mesh = lip::create_from_compressed_lip_file<lip::parametrized_model>(file_name);

                auto span_uv = gsl::make_span(mesh->m_uv.data(), mesh->m_uv.size());
                auto span_indices = gsl::make_span(mesh->m_indices.data(), mesh->m_indices.size());
                auto span_positions = gsl::make_span(mesh->m_positions.data(), mesh->m_positions.size());

                r->m_geometry = gx::geo::create_static_mesh(allocator, resources->upload_queue(), gsl::as_bytes(span_positions), gsl::as_bytes(span_uv));
                r->m_indices = gx::geo::create_indexed_geometry(allocator, resources->upload_queue(), gsl::as_bytes(span_indices));

                //todo: to processing on the compute queue
                resources->compute_queue()->insert_wait_on(resources->upload_queue()->flush());

                return r;
            }

            std::unique_ptr< static_render_object > render_object_factory< static_render_object >::make_render_object(const wchar_t* file_name, device_resources* resources, gx::geo::geometry_allocator* allocator)
            {
                std::unique_ptr< static_render_object > r = std::make_unique< static_render_object  >();

                auto mesh = lip::create_from_compressed_lip_file<lip::multi_textured_model>(file_name);

                auto span_uv = gsl::make_span(mesh->m_uv.data(), mesh->m_uv.size());
                auto span_indices = gsl::make_span(mesh->m_indices.data(), mesh->m_indices.size());
                auto span_positions = gsl::make_span(mesh->m_positions.data(), mesh->m_positions.size());

                r->m_geometry = gx::geo::create_static_mesh(allocator, resources->upload_queue(), gsl::as_bytes(span_positions), gsl::as_bytes(span_uv));
                r->m_indices = gx::geo::create_indexed_geometry(allocator, resources->upload_queue(), gsl::as_bytes(span_indices));

                r->m_opaque_textures.resize(mesh->m_textures.size());

                for (auto i = 0U; i < mesh->m_textures.size(); ++i)
                {
                    auto& texture = mesh->m_textures[i];

                    auto w = texture.m_levels[0].m_width;
                    auto h = texture.m_levels[0].m_height;

                    r->m_opaque_textures[i] = gx::dx12::create_texture_2d(resources->resource_create_context(), w, h, static_cast<DXGI_FORMAT>(texture.m_levels[0].view()));

                    D3D12_SUBRESOURCE_DATA s = gx::sub_resource_data(&texture.m_levels[0]);
                    resources->upload_queue()->upload_texture_2d(r->m_opaque_textures[i].get(), 0, 1, &s);
                }

                r->m_primitive_ranges.reserve(mesh->m_primitive_ranges.size());

                for (auto&& i : mesh->m_primitive_ranges)
                {
                    gx::geo::primitive_range range(i.m_begin, i.m_end);
                    r->m_primitive_ranges.push_back(std::move(range));
                }

                //todo: to processing on the compute queue
                resources->compute_queue()->insert_wait_on(resources->upload_queue()->flush());

                return r;
            }
        }
    }
}