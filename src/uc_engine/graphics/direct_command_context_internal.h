#include "pch.h"

#include <uc_public/graphics/direct_command_context.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DirectCommandContextInternal final : public DirectCommandContext
        {
            using Base = DirectCommandContext;
            public:

            DirectCommandContextInternal(uc::gx::dx12::managed_graphics_command_context&& context );
        };
    }
}


