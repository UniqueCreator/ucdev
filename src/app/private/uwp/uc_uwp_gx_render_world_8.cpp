#include "pch.h"
#include "uc_uwp_gx_render_world_8.h"

#include <gsl/gsl>
#include <ppl.h>
#include <array>

#include <uc_dev/gx/dx12/gpu/texture_2d.h>

#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include <uc_dev/gx/img_utils.h>
#include <uc_dev/gx/anm/anm.h>

#include <autogen/shaders/textured_skinned_solid_graphics.h>
#include <autogen/shaders/textured_skinned_depth_only_graphics.h>
#include <autogen/shaders/skeleton_graphics.h>

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
                inline math::float4x4 robot_world_transform(double frame_time)
                {
                    //todo: move this in the update pahse
                    const float total = 3.14159265358979323846f * 2.0f;
                    const  uint32_t steps = 144;

                    const float step_rotation = total / steps;
                    static uint32_t step = 0;

                    static float angle = 0.0f;

                    const float rotation_angle = static_cast<float>(angle + 100.0f * frame_time * step_rotation);
                    angle = rotation_angle;

                    math::float4x4 t0 = math::translation(-8.0f, 15.0f, 0.0f);
                    math::float4x4 t1 = math::rotation_y(rotation_angle);
                    math::float4x4 t2 = math::translation(-30.0f, -15.0f, 0.0f);

                    return t1;// math::mul(t1, t0);
                }
            }

            render_world_8::render_world_8(initialize_context* c) : base(c)//, current_animation(animations::idle)
            {
                concurrency::task_group g;

                g.run([this, c]
                {
                    m_robot = gxu::make_render_object_from_file<skinned_render_object>(L"appdata/meshes/robot.derivatives_skinned_model.model", c->m_resources, c->m_geometry);
                });

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_textured_skinned = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_skinned_solid_graphics::create_pso);
                });

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_textured_skinned_depth = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::textured_skinned_depth_only_graphics::create_pso);
                });

                //load preprocessed textured model
                g.run([this]()
                {
                    m_skeleton    = lip::create_from_compressed_lip_file<lip::skeleton>(L"appdata/skeletons/robot.skeleton");
                });

                //load preprocessed textured model
                g.run([this]()
                {
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/breathing_idle.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/climbing.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/closing.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/closing_a_lid.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/crouch_death.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/drawing_gun.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/gunplay.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/hanging_idle.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/hit_reaction.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/inspecting.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/jump_away.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/jumping.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/lifting.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/picking_up_object.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/rifle_jump.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/rifle_run.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/running.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/running_jump.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/shooting.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/smash.animation"));
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/swagger_walk.animation"));
                });

                //m_camera->set_view_position(math::set(15.0, 0.0f, -25.5f, 0.0f));
                m_camera->set_view_position(math::set(0.0, 0.0f, -25.5f, 0.0f));
                m_camera->set_far(1200.0f);

                g.wait();

                for (auto i = 0U; i < m_animations.size(); ++i)
                {
                    m_skeleton_instance.push_back(std::make_unique<gx::anm::skeleton_instance>(m_skeleton.get()));
                    m_animation_instance.push_back(std::make_unique<gx::anm::animation_instance>(m_animations[i].get(), m_skeleton.get()));
                }

                m_draw_constants.resize(m_animations.size());
            }

            render_world_8::~render_world_8()
            {

            }

            void render_world_8::do_update(update_context* ctx)
            {
                concurrency::parallel_for(size_t(0), m_animations.size(), [this, ctx](size_t i) -> void
                {
                    m_animation_instance[i]->accumulate(m_skeleton_instance[i].get(), ctx->m_frame_time);
                });


                concurrency::parallel_for(size_t(0), m_animations.size(), [this](size_t i) ->void
                {
                    auto skeleton = m_skeleton.get();

                    //draw
                    auto&& draw = m_draw_constants[i];
                    //draw.m_world = uc::math::transpose(*m_robot_transform);
                    draw.m_world = math::identity_matrix();  //uc::math::transpose(m_robot_transform);
                    {
                        math::float4x4 t = math::translation_x(/*1.5f * i*/0);

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
                current_animation = animations::idle;
                auto gamepad = ctx->m_pad_state;
                if (gamepad.m_state.m_thumb_left_y != 0)
                {
                    current_animation = animations::running;
                }
                else if (gamepad.m_state.m_left_trigger != 0)
                {
                    current_animation = animations::drawing_gun;
                }
            }

            std::unique_ptr< submitable >render_world_8::do_render(render_context* ctx)
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
                graphics->set_pso(m_textured_skinned);

                {
                    frame_constants frame;

                    frame.m_view        = uc::math::transpose(uc::gx::view_matrix( camera() ));
                    frame.m_perspective = uc::math::transpose(uc::gx::perspective_matrix( camera() ));
                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                }

                //robot
                {
                    graphics->set_pso(m_textured_skinned);
                    graphics->set_descriptor_heaps();

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

                    size_t start = 0;
                    size_t size = m_robot->m_primitive_ranges.size();

                    for (auto j = start; j < start + size; ++j)
                    {
                        {
                            auto& t = m_robot->m_opaque_textures[j];
                            //material
                            graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, t->srv());
                        }

                        {
                            auto& r = m_robot->m_primitive_ranges[j];
                            auto  base_index_offset     = m_robot->m_indices->index_offset();
                            auto  base_vertex_offset    = m_robot->m_geometry->draw_offset();

                            //Draw call -> frequency 2 ( nvidia take care these should be on a sub 1 ms granularity)
                            graphics->draw_indexed(r.size(), r.m_begin + base_index_offset, base_vertex_offset);
                        }
                    }
                }

                end_render(ctx, graphics.get());

                return std::make_unique<graphics_submitable>(std::move(graphics));
            }

            std::unique_ptr< submitable >render_world_8::do_render_depth(render_context* ctx)
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
                graphics->set_pso(m_textured_skinned_depth);

                {
                    frame_constants frame;
                    frame.m_view = uc::math::transpose(uc::gx::view_matrix(camera()));
                    frame.m_perspective = uc::math::transpose(uc::gx::perspective_matrix(camera()));
                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                }

                //geometry
                graphics->set_vertex_buffer(0, ctx->m_geometry->skinned_mesh_position_view());
                graphics->set_vertex_buffer(1, ctx->m_geometry->skinned_mesh_blend_weight_view());
                graphics->set_vertex_buffer(2, ctx->m_geometry->skinned_mesh_blend_index_view());
                graphics->set_index_buffer(ctx->m_geometry->indices_view());

                auto  base_index_offset = m_robot->m_indices->index_offset();
                auto  base_vertex_offset = m_robot->m_geometry->draw_offset();
                auto  index_count = m_robot->m_indices->index_count();

                graphics->draw_indexed(index_count, base_index_offset, base_vertex_offset);

                end_render_depth(ctx, graphics.get());
                return std::make_unique<graphics_submitable>(std::move(graphics));

            }
        }
    }
}