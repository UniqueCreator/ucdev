#include "pch.h"
#include "uc_uwp_gx_render_world_moment_shadow_maps.h"

#include <gsl/gsl>
#include <ppl.h>
#include <array>

#include <uc_dev/gx/dx12/gpu/texture_2d.h>
#include <uc_dev/gx/dx12/cmd/profiler.h>

#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include <uc_dev/gx/img_utils.h>
#include <uc_dev/gx/anm/anm.h>

#include <autogen/shaders/textured_skinned_lit_solid_graphics.h>
#include <autogen/shaders/textured_skinned_lit_depth_only_graphics.h>
#include <autogen/shaders/textured_skinned_lit_shadows_graphics.h>
#include <autogen/shaders/shadows_resolve_compute.h>
#include <autogen/shaders/plane_solid_graphics.h>
#include <autogen/shaders/plane_depth_only_graphics.h>
#include <autogen/shaders/plane_shadows_graphics.h>

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
                inline math::float4x4 military_mechanic_world_transform(double frame_time)
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

                    math::float4x4 t0 = math::translation(-8.0f, 15.0f, 0.0f);
                    math::float4x4 t1 = math::rotation_y(rotation_angle);
                    math::float4x4 t2 = math::translation(-30.0f, -15.0f, 0.0f);

                    return t1;// math::mul(t1, t0);
                }
            }

            render_world_moment_shadow_maps::render_world_moment_shadow_maps(initialize_context* c) : base(c)
            {
                concurrency::task_group g;

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_skinned = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_skinned_lit_solid_graphics::create_pso);
                });

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_plane = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::plane_solid_graphics::create_pso);
                });


                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_shadows_resolve = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::shadows_resolve_compute::create_pso);
                });


                //load blue noise
                g.run([this, c]
                {
                    m_blue_noise = gx::blue_noise::make_blue_noise(c->m_resources->resource_create_context(), c->m_resources->upload_queue());
                });

                g.wait();
            }

            render_world_moment_shadow_maps::~render_world_moment_shadow_maps()
            {

            }

            std::unique_ptr< submitable >render_world_moment_shadow_maps::do_render_shadows(shadow_render_context* ctx)
            {
                //now start new ones
                auto resources = ctx->m_resources;
                auto graphics = create_graphics_compute_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));


                {
                    auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"Shadows Pass");

                    graphics->clear_depth(ctx->m_shadow_depth_buffer, 0.0f);
                    graphics->set_render_target(ctx->m_shadow_depth_buffer);

                    {
                        set_view_port(ctx, graphics.get());
                    }

                    //Per many draw calls  -> frequency 1
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    graphics->set_pso(m_skinned_shadows);
                    graphics->set_descriptor_heaps();

                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, m_constants_frame_shadows);

                    //mechanic
                    {
                        auto profile_event0 = uc::gx::dx12::make_profile_event(graphics.get(), L"Mechanic");

                        //todo: move this into a big buffer for the whole scene
                        graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, m_constants_pass);

                        //geometry
                        graphics->set_vertex_buffer(0, ctx->m_geometry->skinned_mesh_position_view());
                        graphics->set_vertex_buffer(1, ctx->m_geometry->skinned_mesh_blend_weight_view());
                        graphics->set_vertex_buffer(2, ctx->m_geometry->skinned_mesh_blend_index_view());
                        graphics->set_index_buffer(ctx->m_geometry->indices_view());

                        //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                        graphics->draw_indexed(m_military_mechanic->m_indices->index_count(), m_military_mechanic->m_indices->index_offset(), m_military_mechanic->m_geometry->draw_offset());
                    }

                    //plane
                    {
                        auto profile_event0 = uc::gx::dx12::make_profile_event(graphics.get(), L"Plane");

                        math::float4x4 m = math::identity_matrix();

                        graphics->set_pso(m_plane_shadows);

                        graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, m);

                        gx::dx12::graphics_reset_vertex_buffer(graphics, 0);
                        gx::dx12::graphics_reset_index_buffer(graphics);
                        graphics->draw(6);
                    }

                    end_render_shadows(ctx, graphics.get());
                }

                {

                    graphics->transition_resource(ctx->m_shadow_depth_buffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_DEPTH_READ | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

                    {
                        auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"Shadows Resolve Pass");

                        graphics->set_pso(m_shadows_resolve);

                        graphics->set_compute_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, ctx->m_shadow_depth_buffer->srv_depth());
                        graphics->set_compute_dynamic_descriptor(gx::dx12::default_root_singature::slots::uav_1, ctx->m_shadow_map->uav());

                        graphics->dispatch(2048 / 32, 2048 / 8, 1);

                    }

                    graphics->transition_resource(ctx->m_shadow_map, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
                }

                return std::make_unique<graphics_compute_submitable>(std::move(graphics));
            }

            std::unique_ptr< submitable >render_world_moment_shadow_maps::do_render(render_context* ctx)
            {
                //now start new ones
                auto resources = ctx->m_resources;
                auto graphics = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));

                auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"Main Pass");

                begin_render(ctx, graphics.get());

                {
                    set_view_port(ctx, graphics.get());
                    graphics->set_descriptor_heaps();
                }

                //Per many draw calls  -> frequency 1
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                graphics->set_pso(m_skinned);
                graphics->set_descriptor_heaps();

                graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, m_constants_frame);

                //shadows and light
                {
                    struct render_object_frame_constants
                    {

                        math::float4x4 m_shadow_view;
                        math::float4x4 m_shadow_perspective;
                        math::float4   m_directional_light;
                    };

                    render_object_frame_constants r;

                    r.m_shadow_view = m_constants_frame_shadows.m_view;
                    r.m_shadow_perspective = m_constants_frame_shadows.m_perspective;
                    r.m_directional_light = m_light_direction;
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 1, r);
                }

                graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, ctx->m_shadow_map->srv(), 0);
                graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, m_blue_noise->srv(),1);


                //mechanic
                {
                    auto profile_event0 = uc::gx::dx12::make_profile_event(graphics.get(), L"Mechanic"); 
                    //todo: move this into a big buffer for the whole scene
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, m_constants_pass);

                    //geometry
                    graphics->set_vertex_buffer(0, ctx->m_geometry->skinned_mesh_position_view());
                    graphics->set_vertex_buffer(1, ctx->m_geometry->skinned_mesh_uv_view());
                    graphics->set_vertex_buffer(2, ctx->m_geometry->skinned_mesh_normal_view());
                    graphics->set_vertex_buffer(3, ctx->m_geometry->skinned_mesh_tangent_view());
                    graphics->set_vertex_buffer(4, ctx->m_geometry->skinned_mesh_blend_weight_view());
                    graphics->set_vertex_buffer(5, ctx->m_geometry->skinned_mesh_blend_index_view());
                    graphics->set_index_buffer(ctx->m_geometry->indices_view());

                    size_t start = 0;
                    size_t size = m_military_mechanic->m_primitive_ranges.size();

                    for (auto i = start; i < start + size; ++i)
                    {
                        {
                            //auto& t = m_military_mechanic->m_opaque_textures[i];
                            //material

                        }

                        {
                            auto& r = m_military_mechanic->m_primitive_ranges[i];
                            auto  base_index_offset = m_military_mechanic->m_indices->index_offset();
                            auto  base_vertex_offset = m_military_mechanic->m_geometry->draw_offset();

                            //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                            graphics->draw_indexed(r.size(), r.m_begin + base_index_offset, base_vertex_offset);
                        }
                    }
                }


                //plane
                {
                    auto profile_event0 = uc::gx::dx12::make_profile_event(graphics.get(), L"Plane");

                    math::float4x4 m = math::identity_matrix();

                    graphics->set_pso(m_plane);

                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, m);

                    gx::dx12::graphics_reset_vertex_buffer(graphics, 0);
                    gx::dx12::graphics_reset_index_buffer(graphics);

                    graphics->draw(6);
                }

                end_render(ctx, graphics.get());

                return std::make_unique<graphics_submitable>(std::move(graphics));
            }
            
        }
    }
}