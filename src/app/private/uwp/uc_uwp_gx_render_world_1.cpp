#include "pch.h"

#include "uc_uwp_gx_render_world_1.h"

#include <gsl/gsl>
#include <ppl.h>
#include <array>

#include <uc_dev/gx/dx12/gpu/texture_2d.h>
#include <uc_dev/gx/dx12/cmd/profiler.h>

#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include <uc_dev/gx/img_utils.h>
#include <uc_dev/gx/anm/anm.h>

#include <autogen/shaders/textured_skinned_lit_depth_only_graphics.h>
#include <autogen/shaders/textured_skinned_lit_material_graphics.h>
#include <autogen/shaders/textured_skinned_lit_shadows_graphics.h> //todo: remove lit prefix, nothing to do with light
#include <autogen/shaders/skeleton_graphics.h>
          

#include <autogen/shaders/plane_solid_graphics.h>
#include <autogen/shaders/plane_depth_only_graphics.h>
#include <autogen/shaders/plane_shadows_graphics.h>

#include <autogen/shaders/shadows_resolve_compute.h>


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
                struct skinned_draw_constants
                {
                    math::float4x4                  m_world;
                    std::array<math::float4x4, 127> m_joints_palette;

                    skinned_draw_constants()
                    {
                        m_world = math::identity_matrix();

                        for (auto&& i : m_joints_palette)
                        {
                            i = math::identity_matrix();
                        }
                    }
                };

                inline math::float4x4 robot_world_transform(double frame_time)
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

            render_world_1::render_world_1(initialize_context* c) : base(c)
            {
                concurrency::task_group g;

                g.run([this, c]
                {
                    m_robot = gxu::make_render_object_from_file<skinned_render_object>(L"appdata/meshes/robot.derivatives_skinned_model.model", c->m_resources, c->m_geometry);
                });

                {
                    g.run([this, c]
                    {
                        auto resources = c->m_resources;
                        m_skinned = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_skinned_lit_material_graphics::create_pso);
                    });

                    g.run([this, c]
                    {
                        auto resources = c->m_resources;
                        m_skinned_depth = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_skinned_lit_depth_only_graphics::create_pso);
                    });

                    g.run([this, c]
                    {
                        auto resources = c->m_resources;
                        m_skinned_shadows = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_skinned_lit_shadows_graphics::create_pso);
                    });

                }

                {
                    g.run([this, c]
                    {
                        auto resources = c->m_resources;
                        m_plane_depth = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::plane_depth_only_graphics::create_pso);
                    });

                    g.run([this, c]
                    {
                        auto resources = c->m_resources;
                        m_plane_shadows = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::plane_shadows_graphics::create_pso);
                    });

                    g.run([this, c]
                    {
                        auto resources = c->m_resources;
                        m_plane = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::plane_solid_graphics::create_pso);
                    });
                }

                {

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
                }


                //load preprocessed textured model
                g.run([this]()
                {
                    m_skeleton    = lip::create_from_compressed_lip_file<lip::skeleton>(L"appdata/skeletons/robot.skeleton");
                });

                const std::array<std::wstring, 21> animation_names =
                {
                    L"appdata/animations/breathing_idle.animation",
                    L"appdata/animations/climbing.animation",
                    L"appdata/animations/closing.animation",
                    L"appdata/animations/closing_a_lid.animation",
                    L"appdata/animations/crouch_death.animation",
                    L"appdata/animations/drawing_gun.animation",
                    L"appdata/animations/gunplay.animation",
                    L"appdata/animations/hanging_idle.animation",
                    L"appdata/animations/hit_reaction.animation",
                    L"appdata/animations/inspecting.animation",
                    L"appdata/animations/jump_away.animation",
                    L"appdata/animations/jumping.animation",
                    L"appdata/animations/lifting.animation",
                    L"appdata/animations/picking_up_object.animation",
                    L"appdata/animations/rifle_jump.animation",
                    L"appdata/animations/rifle_run.animation",
                    L"appdata/animations/running.animation",
                    L"appdata/animations/running_jump.animation",
                    L"appdata/animations/shooting.animation",
                    L"appdata/animations/smash.animation",
                    L"appdata/animations/swagger_walk.animation",
                };

                m_animations.resize(animation_names.size());

                for (auto i = 0U; i < animation_names.size(); ++i)
                {
                    g.run([this, i, &animation_names]()
                    {
                        m_animations[i] = lip::create_from_compressed_lip_file<lip::joint_animations>(animation_names[i]);
                    });
                }

                gx::pinhole_camera_helper::set_up(m_camera.get(), 0.0f, 1.0f, 0.0f);
                gx::pinhole_camera_helper::set_forward(m_camera.get(), 0.0f, 0.0f, -1.0f);
                gx::pinhole_camera_helper::set_view_position(m_camera.get(), 15.0, 1.0f, 25.5f);

                gx::pinhole_camera_helper::set_look_at(m_camera.get(), math::vector3(15.0, 15.0f, 25.5f), math::vector3(15, 0, 5), math::vector3(0.0, 1.0, 0.0));

                m_camera->set_far(1200.0f);

                m_light_direction = math::normalize3(math::vector3(1.0, 1.0, 0.0));

                math::float4 light_direction = m_light_direction;
                math::float4 shadows_forward = math::negate(light_direction);
                math::float4 shadows_up = math::orthogonal3_vector(light_direction);

                m_shadow_camera->set_view_position(math::point3(6, 100, 0));
                m_shadow_camera->set_forward(shadows_forward);
                m_shadow_camera->set_up(shadows_up);


                g.wait();

                for (auto i = 0U; i < m_animations.size(); ++i)
                {
                    m_skeleton_instance.push_back(std::make_unique<gx::anm::skeleton_instance>(m_skeleton.get()));
                    m_animation_instance.push_back(std::make_unique<gx::anm::animation_instance>(m_animations[i].get(), m_skeleton.get()));
                }

                m_draw_constants.resize(m_animations.size());
            }

            render_world_1::~render_world_1()
            {

            }

            void render_world_1::do_update(update_context* ctx)
            {
                for (auto&& s : m_skeleton_instance)
                {
                    s->reset();
                }

                concurrency::parallel_for(size_t(0), m_animations.size(), [this, ctx](size_t i) -> void
                {
                    m_animation_instance[i]->accumulate(m_skeleton_instance[i].get(), ctx->m_frame_time);
                });

                concurrency::parallel_for(size_t(0), m_animations.size(), [this](size_t i) ->void
                {
                    auto skeleton = m_skeleton.get();

                    //draw
                    auto&& draw = m_draw_constants[i];

                    draw.m_world = math::identity_matrix(); // uc::math::transpose(m_robot_transform);
                    {
                        math::float4x4 t = math::translation_x(1.5f * i);

                        auto joints = gx::anm::local_to_world_joints2(skeleton, m_skeleton_instance[i]->local_transforms(), t);

                        for (auto j = 0U; j < joints.size(); ++j)
                        {
                            math::float4x4 bind_pose    = math::load44u(&skeleton->m_joint_inverse_bind_pose2[j].m_a0);
                            math::float4x4 joint        = math::load44u(reinterpret_cast<float*>(&joints[j]));
                            math::float4x4 palette      = math::mul(bind_pose, joint);
                            draw.m_joints_palette[j]    = math::transpose(palette);
                        }
                    }
                });

                {
                    m_constants_frame.m_view = uc::math::transpose(uc::gx::view_matrix(camera()));
                    m_constants_frame.m_perspective = uc::math::transpose(uc::gx::perspective_matrix(camera()));

                    //todo: improve
                    {
                        //do a scene bounding volume, something that encompasses all meshes, should be as tight as possible. transform it to view space and make the orthogonal projection
                        math::float4 p0 = math::point3(0.0f, 0.0f, 0.0f);
                        math::float4 p1 = math::vector3(25.0f, 25.0f, 25.0f);
                        math::aabb1 scene = { p0, p1 };



                        math::euclidean_transform_3d view = math::make_euclidean_transform_3d(gx::view_matrix(m_shadow_camera.get()));
                        math::aabb1 scene_bounds_vs = math::transform(scene, view);

                        math::float4 aabb_min = math::bounds_min(scene_bounds_vs);
                        math::float4 aabb_max = math::bounds_max(scene_bounds_vs);

                        m_shadow_camera->set_x_min(math::get_x(aabb_min));
                        m_shadow_camera->set_x_max(math::get_x(aabb_max));

                        m_shadow_camera->set_y_min(math::get_y(aabb_min));
                        m_shadow_camera->set_y_max(math::get_y(aabb_max));

                        m_shadow_camera->set_z_min(math::get_z(aabb_max));
                        m_shadow_camera->set_z_max(math::get_z(aabb_min));

                        m_constants_frame_shadows.m_view = uc::math::transpose(uc::gx::view_matrix(m_shadow_camera.get()));
                        m_constants_frame_shadows.m_perspective = uc::math::transpose(uc::gx::perspective_matrix(m_shadow_camera.get()));
                    }
                }
            }

            std::unique_ptr< submitable > render_world_1::do_render(render_context* ctx)
            {
                //now start new ones
                auto resources = ctx->m_resources;
                auto graphics = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));

                auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"do_render");

                begin_render(ctx, graphics.get());

                {
                    set_view_port(ctx, graphics.get());
                    graphics->set_descriptor_heaps();
                }

                //Per many draw calls  -> frequency 1
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                graphics->set_pso(m_skinned);
                graphics->set_descriptor_heaps();

                {
                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, m_constants_frame);
                }

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
                graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, m_blue_noise->srv(), 1);


                //robot
                {
                    //todo: move this into a big buffer for the whole scene
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    //geometry
                    graphics->set_vertex_buffer(0, ctx->m_geometry->skinned_mesh_position_view());
                    graphics->set_vertex_buffer(1, ctx->m_geometry->skinned_mesh_uv_view());
                    graphics->set_vertex_buffer(2, ctx->m_geometry->skinned_mesh_normal_view());
                    graphics->set_vertex_buffer(3, ctx->m_geometry->skinned_mesh_tangent_view());
                    graphics->set_vertex_buffer(4, ctx->m_geometry->skinned_mesh_blend_weight_view());
                    graphics->set_vertex_buffer(5, ctx->m_geometry->skinned_mesh_blend_index_view());
                    graphics->set_index_buffer(ctx->m_geometry->indices_view());


                    for ( auto i = 0U; i < m_animations.size(); ++i)
                    {
                        //draw
                        auto&& draw = m_draw_constants[i];

                        graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);

                        size_t start = 0;
                        size_t size = m_robot->m_primitive_ranges.size();

                        for (auto j = start; j < start + size; ++j)
                        {
                            {
                                auto& t = m_robot->m_opaque_textures[j];
                                //material
                                graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, t->srv(), 2);
                            }

                            {
                                auto& r = m_robot->m_primitive_ranges[j];
                                auto  base_index_offset = m_robot->m_indices->index_offset();
                                auto  base_vertex_offset = m_robot->m_geometry->draw_offset();

                                //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                                graphics->draw_indexed(r.size(), r.m_begin + base_index_offset, base_vertex_offset);
                            }
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

            std::unique_ptr< submitable >   render_world_1::do_render_depth(render_context* ctx)
            {

                auto resources = ctx->m_resources;
                //now start new ones
                auto graphics = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));
                graphics->pix_begin_event(L"do_render_depth");
                begin_render_depth(ctx, graphics.get());

                
                {
                    set_view_port(ctx, graphics.get());
                    graphics->set_descriptor_heaps();
                }

                //Per many draw calls  -> frequency 1
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                //robots
                {
                    auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"Robots");

                    graphics->set_pso(m_skinned_depth);
                    {
                        graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, m_constants_frame);
                    }

                    graphics->set_pso(m_skinned_depth);

                    //geometry
                    graphics->set_vertex_buffer(0, ctx->m_geometry->skinned_mesh_position_view());
                    graphics->set_vertex_buffer(1, ctx->m_geometry->skinned_mesh_blend_weight_view());
                    graphics->set_vertex_buffer(2, ctx->m_geometry->skinned_mesh_blend_index_view());
                    graphics->set_index_buffer(ctx->m_geometry->indices_view());

                    for (auto i = 0U; i < m_animations.size(); ++i)
                    {
                        auto profile_event0 = uc::gx::dx12::make_profile_event(graphics.get(), L"Robot");
                        //draw
                        auto&& draw = m_draw_constants[i];

                        graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);
                        //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                        graphics->draw_indexed(m_robot->m_indices->index_count(), m_robot->m_indices->index_offset(), m_robot->m_geometry->draw_offset());
                    }
                }

                //plane
                {
                    auto profile_event0 = uc::gx::dx12::make_profile_event(graphics.get(), L"Plane");

                    math::float4x4 m = math::identity_matrix();

                    graphics->set_pso(m_plane_depth);
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, m);

                    gx::dx12::graphics_reset_vertex_buffer(graphics, 0);
                    gx::dx12::graphics_reset_index_buffer(graphics);
                    graphics->draw(6);
                }

                end_render_depth(ctx, graphics.get());

                graphics->pix_end_event();
                return std::make_unique<graphics_submitable>(std::move(graphics));
            }

            std::unique_ptr< submitable > render_world_1::do_render_shadows(shadow_render_context* ctx)
            {
                //now start new ones
                auto resources = ctx->m_resources;
                auto graphics = create_graphics_compute_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));

                {
                    auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"Shadows Pass");

                    graphics->clear_depth(ctx->m_shadow_depth_buffer, 0.0f);
                    graphics->set_render_target(ctx->m_shadow_depth_buffer);
                    set_view_port(ctx, graphics.get());
                    

                    //Per many draw calls  -> frequency 1
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    graphics->set_pso(m_skinned_shadows);
                    graphics->set_descriptor_heaps();

                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, m_constants_frame_shadows);

                    //robots
                    {
                        auto profile_event1 = uc::gx::dx12::make_profile_event(graphics.get(), L"Robots");

                        //geometry
                        graphics->set_vertex_buffer(0, ctx->m_geometry->skinned_mesh_position_view());
                        graphics->set_vertex_buffer(1, ctx->m_geometry->skinned_mesh_blend_weight_view());
                        graphics->set_vertex_buffer(2, ctx->m_geometry->skinned_mesh_blend_index_view());
                        graphics->set_index_buffer(ctx->m_geometry->indices_view());

                        for (auto i = 0U; i < m_animations.size(); ++i)
                        {
                            auto profile_event2 = uc::gx::dx12::make_profile_event(graphics.get(), L"Robot");
                            //draw
                            auto&& draw = m_draw_constants[i];

                            graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);
                            //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                            graphics->draw_indexed(m_robot->m_indices->index_count(), m_robot->m_indices->index_offset(), m_robot->m_geometry->draw_offset());
                        }
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
                
                //Resolve
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

            shadow_buffers_descriptor render_world_1::on_shadow_map_descriptor()
            {
                shadow_buffers_descriptor r = {};

                r.m_shadow_flags = shadow_buffers_descriptor::shadow_flags::shadow_buffer_used | shadow_buffers_descriptor::shadow_flags::shadow_map_used;
                r.m_shadow_buffer.m_width = 2048;
                r.m_shadow_buffer.m_height = 2048;
                r.m_shadow_buffer.m_initial_value = 0.0f;
                r.m_shadow_buffer.m_msaa = 4;
                r.m_shadow_buffer.m_format = DXGI_FORMAT_D32_FLOAT;


                r.m_shadow_map.m_width = 2048;
                r.m_shadow_map.m_height = 2048;
                r.m_shadow_map.m_format = DXGI_FORMAT_R16G16B16A16_UNORM;
                r.m_shadow_map.m_initial_state = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

                return r;
            }

        }
    }
}