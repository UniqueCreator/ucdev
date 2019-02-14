#include "pch.h"
#include "uc_uwp_gx_render_world_3.h"

#include <gsl/gsl>
#include <ppl.h>

#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include <uc_dev/gx/img_utils.h>

#include <uc_dev/gx/dx12/gpu/texture_2d.h>

#include <autogen/shaders/textured_solid_graphics.h>
#include <autogen/shaders/textured_depth_only_graphics.h>

#include "uc_uwp_gx_render_object_factory.h"
#include "uc_uwp_device_resources.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            namespace details
            {
                inline math::float4x4 bear_world_transform(double frame_time)
                {
                    //todo: move this in the update pahse
                    const float total = 3.14159265358979323846f * 2.0f;
                    const  uint32_t steps = 144;

                    const float step_rotation = total / steps;
                    static uint32_t step = 0;

                    static float angle = 0.0f;

                    //const float rotation_angle = (step)* step_rotation;

                    const float rotation_angle = static_cast<float>(angle + 100.0f * frame_time * step_rotation);
                    angle = rotation_angle;

                    math::float4x4 t0 = math::translation(10.0f, 15.0f, 0.0f);
                    math::float4x4 t1 = math::rotation_y(rotation_angle);
                    math::float4x4 t2 = math::translation(-30.0f, -15.0f, 0.0f);

                    //bear_transform = math::mul(t1, math::mul(t2, t1));
                    return math::mul(t1, t0);
                }
            }

            render_world_3::render_world_3(initialize_context* c) : base(c)
            {
                concurrency::task_group g;

                //load bear texture
                g.run([this,c]()
                {
                    auto r0 = gx::imaging::read_image(L"appdata/images/bear.jpg");

                    auto w = r0.width();
                    auto h = r0.height();

                    auto resources = c->m_resources;
                    m_texture_bear = gx::dx12::create_texture_2d(resources->resource_create_context(), w, h, DXGI_FORMAT_R8G8B8A8_UNORM);
                    D3D12_SUBRESOURCE_DATA s = gx::sub_resource_data(r0);
                    resources->upload_queue()->upload_texture_2d(m_texture_bear.get(), 0, 1, &s);
                });

                //load bear mesh
                g.run([this, c]()
                {
                    m_bear = gxu::make_render_object_from_file<parametrized_render_object>(L"appdata/meshes/bear.parametrized.model", c->m_resources, c->m_geometry);
                });

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_textured = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_solid_graphics::create_pso);
                });

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_textured_depth = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_depth_only_graphics::create_pso);
                });

                m_camera->set_view_position(math::set(0.0, 0.0f, -25.5f, 0.0f));
                m_camera->set_far(1200.0f);

                g.wait();
            }

            render_world_3::~render_world_3()
            {

            }

            void render_world_3::do_update(update_context* ctx)
            {
                auto t = details::bear_world_transform(ctx->m_frame_time);
                *m_bear_transform = t;
            }

            std::unique_ptr< submitable >render_world_3::do_render(render_context* ctx)
            {
                //now start new ones
                auto resources = ctx->m_resources;
                auto graphics = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));

                begin_render(ctx, graphics.get());

                {
                    set_view_port(ctx, graphics.get());
                    graphics->set_descriptor_heaps();
                }

                //Per many draw calls  -> frequency 1
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                graphics->set_pso(m_textured);

                {
                    frame_constants frame;
                    frame.m_view = uc::math::transpose(uc::gx::view_matrix(camera()));
                    frame.m_perspective = uc::math::transpose(uc::gx::perspective_matrix(camera()));
                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                }

                //robot
                {
                    //draw
                    draw_constants draw;
                    draw.m_world = uc::math::transpose(*m_bear_transform);

                    graphics->set_pso(m_textured);
                    graphics->set_descriptor_heaps();

                    //todo: move this into a big buffer for the whole scene
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    //geometry

                    graphics->set_vertex_buffer(0, ctx->m_geometry->parametrized_mesh_position_view());
                    graphics->set_vertex_buffer(1, ctx->m_geometry->parametrized_mesh_uv_view());
                    graphics->set_index_buffer(ctx->m_geometry->indices_view());

                    //material
                    graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, m_texture_bear->srv());

                    //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)

                    auto  base_index_offset = m_bear->m_indices->index_offset();
                    auto  base_vertex_offset = m_bear->m_geometry->draw_offset();
                    auto  index_count = m_bear->m_indices->index_count();

                    //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                    graphics->draw_indexed(index_count, base_index_offset, base_vertex_offset);
                }

                end_render(ctx, graphics.get());

                return std::make_unique<graphics_submitable>(std::move(graphics));
            }

            std::unique_ptr< submitable >render_world_3::do_render_depth(render_context* ctx)
            {
                auto resources = ctx->m_resources;
                //now start new ones
                auto graphics = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));
                begin_render_depth(ctx, graphics.get());

                {
                    set_view_port(ctx, graphics.get());
                    graphics->set_descriptor_heaps();
                }

                //Per many draw calls  -> frequency 1
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                graphics->set_pso(m_textured_depth);

                {
                    frame_constants frame;
                    frame.m_view = uc::math::transpose(uc::gx::view_matrix(camera()));
                    frame.m_perspective = uc::math::transpose(uc::gx::perspective_matrix(camera()));
                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                }

                //bear
                {
                    //draw
                    draw_constants draw;
                    draw.m_world = uc::math::transpose(*m_bear_transform);

                    //todo: move this into a big buffer for the whole scene
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


                    //geometry
                    graphics->set_vertex_buffer(0, ctx->m_geometry->parametrized_mesh_position_view());
                    graphics->set_index_buffer(ctx->m_geometry->indices_view());

                    //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                    auto  base_index_offset = m_bear->m_indices->index_offset();
                    auto  base_vertex_offset = m_bear->m_geometry->draw_offset();
                    auto  index_count = m_bear->m_indices->index_count();

                    //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                    graphics->draw_indexed(index_count, base_index_offset, base_vertex_offset);
                }

                end_render_depth(ctx, graphics.get());
                return std::make_unique<graphics_submitable>(std::move(graphics));
            }
        }
    }
}