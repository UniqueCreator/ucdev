#include "pch.h"
#include "uc_uwp_renderer_overlay_page_factory.h"

#include "uc_uwp_renderer_overlay_page.h"

#include "uc_uwp_renderer_debug_overlay_page.h"
#include "uc_uwp_renderer_options_overlay_page.h"
#include "uc_uwp_renderer_hud_page.h"


namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            std::unique_ptr<page> page_manager_factory::create_page(pageid id, initialize_context* ctx)
            {
                switch (id)
                {
                    case pageid::hud            : return std::unique_ptr<page>(new hud_page(ctx));
                    case pageid::debug_page     : return std::unique_ptr<page>(new debug_page(ctx));
                    //case pageid::options_page   : return std::unique_ptr<page>(new options_page(ctx));
                    default: return nullptr;
                }
            }

            std::unique_ptr<page> page_manager_factory::create_options_page(initialize_context* ctx, gxu::render_world_manager* m)
            {
                return std::unique_ptr<page>(new options_page(ctx, m));
            }
         }
    }
}