#pragma once

#include <memory>

#include <uc_dev/util/noncopyable.h>

#include "uc_uwp_gx_render_world_id.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class  render_world;
            struct initialize_context;

            class render_world_factory : public util::noncopyable
            {

                public:

                std::unique_ptr<render_world> create_world( world_id id, initialize_context* ctx );
            };
        }
    }

}