#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/descriptor_handle.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RenderTargetView : public DescriptorHandle
        {
            public:

            class Impl;

            UC_ENGINE_API ~RenderTargetView();

            UC_ENGINE_API RenderTargetView(RenderTargetView&&) = default;
            UC_ENGINE_API RenderTargetView& operator=(RenderTargetView&&);

            protected:

            RenderTargetView();
        };
    }
}


