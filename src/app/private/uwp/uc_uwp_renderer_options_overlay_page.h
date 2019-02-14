#pragma once

#include <uc_dev/sys/profile_timer.h>
#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/io/console.h>

#include "uc_uwp_renderer_overlay_page.h"




namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_manager;
        }

        namespace overlay
        {
            class options_page : public page, public io::console::notifier, public std::enable_shared_from_this<io::console::notifier>
            {
                using base = page;

            public:

                options_page(initialize_context* ctx, gxu::render_world_manager* m);
                ~options_page();

            private:

                void do_update( update_context* ctx) override;
                std::unique_ptr< submitable > do_render( render_context* ctx) override;


                gx::dx12::managed_gpu_texture_2d        m_font;
                gx::dx12::graphics_pipeline_state*      m_imgui_pso;

                std::vector< io::console::message>      m_message;

                void do_handle_messages(const io::console::message* message_begin, const io::console::message* message_end) override;

                gxu::render_world_manager*              m_render_world_manager;
                
            };
        }
    }
    
}