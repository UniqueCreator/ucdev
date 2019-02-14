#include "pch.h"

#include <uc_public/graphics/descriptor_handle.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DescriptorHandleIntenral final : public DescriptorHandle
        {
            using Base = DescriptorHandle;

            public:

            DescriptorHandleIntenral(uc::gx::dx12::descriptor_handle handle );
        };
    }
}


