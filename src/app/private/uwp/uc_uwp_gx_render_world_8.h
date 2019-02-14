#pragma once

#include <memory>
#include <vector>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/geo/indexed_geometry.h>
#include <uc_dev/gx/anm/animation_instance.h>
#include <uc_dev/gx/structs.h>

#include "uc_uwp_gx_render_world.h"

#include <malloc.h>

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
			

            class render_world_8 : public render_world
            {

                using base = render_world;

				enum class animations
				{
					idle			=0,
					climbing,
					closing,
					closing_a_lid,
					crouch_death,
					drawing_gun,
					gunplay,
					hanging_idle,
					hit_reaction,
					inspecting,
					jump_away,
					jumping,
					lifting,
					picking_up_object,
					rifle_jump,
					rifle_run,
					running,
					running_jump,
					shooting,
					smash,
					swagger_walk,
				};

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

                render_world_8(initialize_context* ctx);
                virtual ~render_world_8();

            private:

				animations current_animation;

                void do_update(update_context* ctx) override;
                std::unique_ptr< submitable >do_render(render_context* ctx) override;
                std::unique_ptr< submitable >do_render_depth(render_context* ctx) override;

                std::unique_ptr<gxu::skinned_render_object>                     m_robot;

                gx::dx12::graphics_pipeline_state*                              m_textured_skinned;
                gx::dx12::graphics_pipeline_state*                              m_textured_skinned_depth;

				lip::unique_lip_pointer<lip::skeleton>                          m_skeleton;
				std::vector< lip::unique_lip_pointer<lip::joint_animations> >	m_animations;

				std::vector < std::unique_ptr< gx::anm::skeleton_instance > >   m_skeleton_instance;
				std::vector < std::unique_ptr< gx::anm::animation_instance> >   m_animation_instance;

				std::vector < skinned_draw_constants >							m_draw_constants;

                //update state
				math::managed_float4x4                                          m_robot_transform = math::make_float4x4();
            };
        }
    }
}

