#include "pch.h"
#include "uc_uwp_gx_render_world_7.h"

#include <gsl/gsl>
#include <ppl.h>

#include <autogen/shaders/full_screen_color_graphics.h>
#include "uc_uwp_device_resources.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {

            render_world_7::render_world_7(initialize_context* c) : base(c)
            {
                concurrency::task_group g;

                g.run([this, c]
                {
                    auto resources = c->m_resources;
					m_full_screen_color = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::full_screen_color_graphics::create_pso);
                });

				m_camera->set_view_position(math::set(0.0, 0.0f, -25.5f, 0.0f));
                m_camera->set_far(1200.0f);

				g.wait();
            }

            render_world_7::~render_world_7()
            {

            }

            void render_world_7::do_update( update_context* )
            {

            }

            std::unique_ptr< submitable >render_world_7::do_render( render_context* ctx )
            {
                //now start new ones
                auto resources      = ctx->m_resources;
                auto graphics       = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));

                begin_render(ctx, graphics.get() );

                {
                    set_view_port(ctx, graphics.get()) ;
                    graphics->set_descriptor_heaps();
                }

                //Per many draw calls  -> frequency 1
                graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                graphics->set_pso(m_full_screen_color);
                graphics->draw(3);


                end_render(ctx, graphics.get());

                return std::make_unique<graphics_submitable>(std::move(graphics));
            }
      
        }
    }
}