#pragma once

#include <array>
#include <memory>
#include <ppltasks.h>

#include <uc_dev/util/noncopyable.h>
#include "uc_uwp_gx_render_world.h"
#include "uc_uwp_gx_render_world_id.h"


namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_manager : public util::noncopyable
            {
                public:

                render_world_manager( initialize_context* c );

                ~render_world_manager();

                void show_world( world_id page );
                void hide_world(  );

                void update( update_context* ctx );
                
                std::unique_ptr< submitable >render( render_context* ctx );
                std::unique_ptr< submitable >render_depth(render_context* ctx);
                std::unique_ptr< submitable >render_shadows(shadow_render_context* ctx);

                shadow_buffers_descriptor shadow_map_descriptor();

                void resize_buffers(device_resources* resources);

                gx::pinhole_camera* camera()
                {
                    return m_world ? m_world->camera() : nullptr;
                }

                const gx::pinhole_camera* camera() const
                {
                    return m_world ? m_world->camera() : nullptr;
                }

                private:

                std::shared_ptr< render_world > m_world;
                initialize_context              m_initialize_context;

                concurrency::task<void>         m_loading_world;
            };
        }
    }

}