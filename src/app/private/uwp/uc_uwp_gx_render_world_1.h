#pragma once

#include <memory>
#include <vector>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/geo/indexed_geometry.h>
#include <uc_dev/gx/anm/animation_instance.h>
#include <uc_dev/gx/structs.h>
#include <uc_dev/gx/blue_noise/moment_shadow_maps_blue_noise.h>

#include "uc_uwp_gx_render_world.h"

#include <malloc.h>

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_1 : public render_world
            {

                using base = render_world;

                struct skinned_draw_constants
                {
                    math::float4x4                  m_world;
                    std::array<math::float4x4, 127> m_joints_palette;

                    skinned_draw_constants()
                    {
                        reset();
                    }

                    void reset()
                    {
                        m_world = math::identity_matrix();
                        for (auto&& i : m_joints_palette)
                        {
                            i = math::identity_matrix();
                        }
                    }
                };

            public:

                render_world_1(initialize_context* ctx);
                virtual ~render_world_1();

            private:

                void do_update(update_context* ctx) override;
                std::unique_ptr< submitable >   do_render(render_context* ctx) override;
                std::unique_ptr< submitable >   do_render_depth(render_context* ctx) override;
                std::unique_ptr< submitable >   do_render_shadows(shadow_render_context* ctx) override;
                shadow_buffers_descriptor render_world_1::on_shadow_map_descriptor() override;

                std::unique_ptr<gxu::skinned_render_object>                     m_robot;

                gx::dx12::graphics_pipeline_state*                              m_skinned;
                gx::dx12::graphics_pipeline_state*                              m_skinned_depth;
                gx::dx12::graphics_pipeline_state*                              m_skinned_shadows;

                gx::dx12::graphics_pipeline_state*                              m_plane;
                gx::dx12::graphics_pipeline_state*                              m_plane_depth;
                gx::dx12::graphics_pipeline_state*                              m_plane_shadows;


                std::unique_ptr<gx::blue_noise::ldr_rg01_64x64>                 m_blue_noise;

                lip::unique_lip_pointer<lip::skeleton>                          m_skeleton;
                std::vector< lip::unique_lip_pointer<lip::joint_animations> >   m_animations;

                std::vector < std::unique_ptr< gx::anm::skeleton_instance > >   m_skeleton_instance;
                std::vector < std::unique_ptr< gx::anm::animation_instance> >   m_animation_instance;

                std::vector < skinned_draw_constants >                          m_draw_constants;

                //update state
                math::managed_float4x4                                          m_robot_transform = math::make_float4x4();

                //shadows
                gx::dx12::compute_pipeline_state*                               m_shadows_resolve;
                mem::aligned_unique_ptr<gx::orthographic_camera>                m_shadow_camera = mem::make_aligned_unique_ptr<gx::orthographic_camera>();

                //per frame and per pass constants
                frame_constants        m_constants_frame;
                frame_constants        m_constants_frame_shadows;
                skinned_draw_constants m_constants_pass;

                math::float4           m_light_direction;

            };
        }
    }
}

