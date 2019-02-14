#pragma once

#include <array>
#include <memory>

#include <uc_dev/util/noncopyable.h>
#include "uc_uwp_renderer_overlay_page.h"
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
            class page_manager : public util::noncopyable
            {
                public:

                page_manager(initialize_context* c, gxu::render_world_manager* m);
                virtual ~page_manager();

                void show_page( pageid page );
                void hide_page(  );

                void update(update_context* ctx);
                
                std::unique_ptr< submitable > render( render_context* ctx );

                pageid get_active_page_id() const;

            private:

                std::array< std::unique_ptr<page>, 3 > m_pages;
                uint16_t                               m_active_page_index = 0xffff;

                virtual void do_update(update_context* ctx);
                virtual std::unique_ptr< submitable >  do_render(render_context* ctx);
            };
        }
    }

}