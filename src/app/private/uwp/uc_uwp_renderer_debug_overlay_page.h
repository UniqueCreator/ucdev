#pragma once

#include <uc_dev/sys/profile_timer.h>

#include "uc_uwp_renderer_overlay_page.h"

#include <wrl/client.h>
#include <d2d1_3.h>

namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            class debug_page : public page
            {
                using base = page;
            public:

                debug_page(initialize_context* ctx);
                ~debug_page();

            private:

                void do_update(update_context* ctx) override;
                std::unique_ptr< submitable > do_render( render_context* ctx) override;


                sys::profile_timer  m_profile_timer;
                //
                Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>                    m_brush;
                Microsoft::WRL::ComPtr<IDWriteTextFormat>                       m_text_format;
            };
        }
    }
    
}