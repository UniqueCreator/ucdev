#include "pch.h"

#include "uc_uwp_renderer_overlay_page_manager.h"
#include "uc_uwp_renderer_overlay_page_factory.h"



#include <ppl.h>

namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            page_manager::page_manager(initialize_context* c, gxu::render_world_manager* m )
            {
                //test: for loading two assets concurrently mesh and a texture
                concurrency::task_group g;

                g.run([this, c]
                {
                    page_manager_factory factory;
                    m_pages[0] = factory.create_page(pageid::hud, c);
                });

                g.run([this, c]
                {
                    page_manager_factory factory;
                    m_pages[1] = factory.create_page(pageid::debug_page, c);
                });

                g.run([this, c, m]
                {
                    page_manager_factory factory;
                    m_pages[2] = factory.create_options_page(c, m);
                });
                
                g.wait();

                m_active_page_index = 0xffff;
                //m_active_page_index = 0;
            }

            page_manager::~page_manager()
            {

            }

            void page_manager::update(update_context* ctx)
            {
                do_update(ctx);
            }

            std::unique_ptr< submitable >  page_manager::render( render_context* ctx )
            {
                return do_render(ctx);
            }

            void page_manager::show_page( pageid page )
            {
                m_active_page_index = static_cast<uint16_t>(page);
            }

            void page_manager::hide_page()
            {
                m_active_page_index = static_cast<uint16_t>(pageid::none);
            }

            pageid page_manager::get_active_page_id() const
            {
                return static_cast<pageid>(m_active_page_index);
            }

            std::unique_ptr< submitable >  page_manager::do_render(render_context* ctx)
            {
                if (m_active_page_index != 0xffff)
                {
                    return m_pages[m_active_page_index]->render(ctx);
                }

                return nullptr;
            }

            void page_manager::do_update(update_context* ctx)
            {
                if (m_active_page_index != 0xffff)
                {
                    m_pages[m_active_page_index]->update(ctx);
                }
            }
        }
    }
}