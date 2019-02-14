#pragma once

#include <memory>
#include <vector>

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/geo/indexed_geometry.h>

#include "uc_uwp_gx_render_world.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_4 : public render_world
            {
                using base = render_world;

            public:

                render_world_4(initialize_context* ctx);
                virtual ~render_world_4();

            private:

                void do_update(update_context* ctx) override;
                std::unique_ptr< submitable >do_render(render_context* ctx) override;

                std::unique_ptr<gxu::static_render_object>                      m_deer;
                gx::dx12::graphics_pipeline_state*                              m_textured;
                gx::dx12::graphics_pipeline_state*                              m_textured_depth;

                math::managed_float4x4                                          m_deer_transform = math::make_float4x4();
            };
        }
    }
}

