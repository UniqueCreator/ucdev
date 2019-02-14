#pragma once

#include <memory>
#include <uc_dev/util/noncopyable.h>

#include "uc_uwp_renderer_overlay_pageid.h"

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
            class  page;
            struct initialize_context;

            class page_manager_factory : public util::noncopyable
            {
                public:

                std::unique_ptr<page> create_page(pageid id, initialize_context* ctx );
                std::unique_ptr<page> create_options_page( initialize_context* ctx, gxu::render_world_manager* m );
            };
        }
    }

}