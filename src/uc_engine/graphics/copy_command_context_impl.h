#include "pch.h"

#include <uc_public/graphics/copy_command_context.h>

#include <uc_dev/gx/dx12/dx12.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class CopyCommandContext::Impl
        {
            public:

            Impl() = default;
            Impl(uc::gx::dx12::managed_copy_command_context&& context);

            uc::gx::dx12::managed_copy_command_context m_context;
        };
    }
}


