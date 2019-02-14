#pragma once

#include <memory>
#include <vector>

#include <uc_dev/mem/memory.h>
#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/lip/animation.h>
#include <uc_dev/gx/anm/animation_instance.h>
#include <uc_dev/gx/structs.h>

#include "uc_uwp_gx_render_world.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_5 : public render_world
            {
                using base = render_world;

                public:

                render_world_5(initialize_context* ctx);
                virtual ~render_world_5();

            private:

                void do_update(update_context* ctx) override;
                std::unique_ptr< submitable >do_render(render_context* ctx) override;


                //materials
                gx::dx12::graphics_pipeline_state*                              m_skeleton_pso;
                gx::dx12::graphics_pipeline_state*                              m_depth_prepass_pso;
                
                lip::unique_lip_pointer<lip::skeleton>                          m_skeleton;

                std::vector< lip::unique_lip_pointer<lip::joint_animations> >	m_animations;

                std::vector < std::unique_ptr< gx::anm::skeleton_instance > >   m_skeleton_instance;
                std::vector < std::unique_ptr< gx::anm::animation_instance> >   m_animation_instance;

                std::vector< std::vector<gx::position_3d> >                     m_skeleton_positions;

                math::managed_float4x4                                          m_skeleton_positions_transform = math::make_float4x4();
                //mem::aligned_unique_ptr<math::float4x4>                         m_skeleton_positions_transform = mem::make_aligned_unique_ptr<math::float4x4>();

            };
        }
    }
}

