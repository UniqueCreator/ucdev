#include "pch.h"
#include "uc_uwp_gx_render_world_5.h"

#include <gsl/gsl>
#include <ppl.h>

#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include <uc_dev/gx/img_utils.h>
#include <uc_dev/gx/anm/anm.h>

#include <autogen/shaders/skeleton_graphics.h>
#include <autogen/shaders/depth_prepass_lines_graphics.h>

#include "uc_uwp_device_resources.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            namespace details
            {

                inline math::float4x4 skeleton_transform(double frame_time )
                {
                    frame_time;
                    return math::identity_matrix();

                    /*
                    //todo: move this into the update phase
                    const float total = 3.14159265358979323846f * 2.0f;
                    const  uint32_t steps = 144*2;

                    const float step_rotation = total / steps;
                    static uint32_t step = 0;
                    static float angle = 0.0f;

                    const float rotation_angle = static_cast<float>(angle + 100.0f * frame_time * step_rotation);
                    angle = rotation_angle;
                    return math::rotation_y(rotation_angle);
                    */
                }

                
            }

            render_world_5::render_world_5(initialize_context* c) : base(c)
            {
                concurrency::task_group g;

                g.run([this, c]
                {
                    auto resources = c->m_resources;
                    m_skeleton_pso = gx::dx12::create_pso( resources->device_d2d12(), resources->resource_create_context(), gx::dx12::skeleton_graphics::create_pso );
                });

                /*
                g.run([this]()
                {
                    m_skeleton = lip::create_from_compressed_lip_file<lip::skeleton>(L"appdata/skeletons/robot.skeleton");
                });

                
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
                
                
                g.run([this]()
                {
                    m_skeleton = lip::create_from_compressed_lip_file<lip::skeleton>(L"appdata/skeletons/military_mechanic.skeleton");
                });

                g.run([this]()
                {
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/military_mechanic.animation"));
                });
                */
                
                
                g.run([this]()
                {
                    m_skeleton = lip::create_from_compressed_lip_file<lip::skeleton>(L"appdata/skeletons/robot.skeleton");
                });

                g.run([this]()
                {
                    m_animations.push_back(lip::create_from_compressed_lip_file<lip::joint_animations>(L"appdata/animations/lifting.animation"));
                });
                
                

                g.run([this, c]
                {
                    auto resources      = c->m_resources;
                    m_depth_prepass_pso = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::depth_prepass_lines_graphics::create_pso);
                });

                m_camera->set_view_position(math::set(0.0, 0.0f, -25.5f, 0.0f));
                m_camera->set_far(1200.0f);

                g.wait();

                m_skeleton_positions.resize(m_animations.size());

                for (auto i = 0U; i < m_animations.size(); ++i  )
                {
                    m_skeleton_instance.push_back(std::make_unique<gx::anm::skeleton_instance>(m_skeleton.get()));
                    m_animation_instance.push_back(std::make_unique<gx::anm::animation_instance>(m_animations[i].get(), m_skeleton.get() ) );
                }
            }

            render_world_5::~render_world_5()
            {

            }

            void render_world_5::do_update( update_context* ctx )
            {
                concurrency::parallel_for(size_t(0), m_animations.size(), [this, ctx](size_t i) -> void
                {
                    m_animation_instance[i]->accumulate(m_skeleton_instance[i].get(), ctx->m_frame_time);
                    math::float4x4 t = math::translation_x(1.5f * i);
                    m_skeleton_positions[i] = gx::anm::skeleton_positions(m_skeleton.get(), m_skeleton_instance[i]->local_transforms(), t);
                });


                *m_skeleton_positions_transform  = details::skeleton_transform( ctx->m_frame_time );
             }

            std::unique_ptr< submitable >render_world_5::do_render(render_context* ctx)
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
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
                graphics->set_pso(m_skeleton_pso);
                graphics->set_descriptor_heaps();

                {
                    frame_constants frame;
                    frame.m_view = uc::math::transpose(uc::gx::view_matrix(camera()));
                    frame.m_perspective = uc::math::transpose(uc::gx::perspective_matrix(camera()));
                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                }

                for ( auto i = 0U; i < m_animations.size(); ++i )
                {
                    //draw
                    draw_constants draw;

                    draw.m_world = uc::math::translation_x( 0 );

                    //todo: move this into a big buffer for the whole scene
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);

                    auto&& positions = m_skeleton_positions[i];

                    //gx::anm::skeleton_instance instance(m_skeleton.get());
                    //auto&& positions =gx::anm::skeleton_positions(m_skeleton.get(), instance.local_transforms());

                    graphics->set_dynamic_vertex_buffer(0, sizeof(gx::position_3d), resources->upload_queue()->upload_buffer(&positions[0], positions.size() * sizeof(gx::position_3d)));
                    graphics->draw(static_cast<uint32_t>(positions.size()));
                }

                end_render(ctx, graphics.get());

                return std::make_unique<graphics_submitable>(std::move(graphics));
            }
        }
    }
}