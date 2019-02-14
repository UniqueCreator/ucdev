#pragma once

#include <memory>
#include <vector>

#include <uc_dev/gx/dx12/dx12.h>

#include "uc_uwp_gx_render_world_default.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_7 : public render_world_default
            {
                using base = render_world_default;

                public:

                render_world_7(initialize_context* ctx);
                virtual ~render_world_7();

            private:

                void do_update(update_context* ctx) override;

                std::unique_ptr< submitable >do_render(render_context* ctx) override;

                gx::dx12::graphics_pipeline_state*	m_full_screen_color;
            };
        }
    }
}

