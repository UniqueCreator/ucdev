#pragma once

#include <memory>
#include <vector>

#include <uc_dev/gx/dx12/dx12.h>

#include "uc_uwp_gx_render_world.h"



namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_default : public render_world
            {
                using base = render_world;

                public:

                render_world_default(initialize_context* ctx);
                virtual ~render_world_default();

                private:
                void do_update(update_context* ctx) override;
                std::unique_ptr< submitable >do_render(render_context* ctx) override;
                std::unique_ptr< submitable >do_render_depth(render_context* ctx) override;
            };
        }
    }
}

