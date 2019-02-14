#include "pch.h"

#include <uc_public/graphics/copy_command_context.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class CopyCommandContextInternal final : public CopyCommandContext
        {
            using Base = CopyCommandContext;
            
            public:

            CopyCommandContextInternal(uc::gx::dx12::managed_copy_command_context&& context );
        };
    }
}


