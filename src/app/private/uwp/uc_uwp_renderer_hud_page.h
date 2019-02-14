#pragma once

#include <uc_dev/sys/profile_timer.h>

#include "uc_uwp_renderer_overlay_page.h"

#include <uc_dev/gx/dx12/dx12.h>

#include "librocket/uc_uwp_librocket_initializer.h"
#include "librocket/uc_uwp_librocket_system_interface.h"
#include "librocket/uc_uwp_librocket_file_interface.h"
#include "librocket/uc_uwp_librocket_render_interface.h"

#include "librocket/uc_uwp_librocket_intrusive_ptr.h"


namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            class hud_page : public page
            {
                using base = page;

            public:

                hud_page(initialize_context* ctx);
                ~hud_page();

            private:

                void do_update( update_context* ctx) override;
                std::unique_ptr< submitable > do_render( render_context* ctx) override;

                librocket::system_interface          m_system_interface;
                librocket::file_interface            m_file_interface;
                librocket::render_interface          m_render_interface;
                librocket::initializer               m_initializer;

                mem::intrusive_ptr<Rocket::Core::Context>         m_context;
                mem::intrusive_ptr<Rocket::Core::ElementDocument> m_main_document;
                gx::dx12::graphics_pipeline_state*                m_pso;

                Rocket::Core::Input::KeyIdentifier                m_key_mapping[259];
                void                                              initialize_mapping();
            };
        }
    }
    
}