#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/descriptor_handle.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ShaderResourceView : public DescriptorHandle
        {
            public:

            UC_ENGINE_API ~ShaderResourceView();

            UC_ENGINE_API ShaderResourceView(ShaderResourceView&&) = default;
            UC_ENGINE_API ShaderResourceView& operator=(ShaderResourceView&&);

            protected:
            ShaderResourceView();
        };
    }
}


