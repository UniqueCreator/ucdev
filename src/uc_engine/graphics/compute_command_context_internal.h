#include "pch.h"

#include <uc_public/graphics/compute_command_context.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ComputeCommandContextInternal final : public ComputeCommandContext
        {
            using Base = ComputeCommandContext;
            public:

            ComputeCommandContextInternal(uc::gx::dx12::managed_compute_command_context&& context );
        };
    }
}


