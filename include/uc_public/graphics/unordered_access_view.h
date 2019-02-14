#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/descriptor_handle.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class UnorderedAccessView : public DescriptorHandle
        {
            public:

            class Impl;

            UC_ENGINE_API ~UnorderedAccessView();

            UC_ENGINE_API UnorderedAccessView(UnorderedAccessView&&) = default;
            UC_ENGINE_API UnorderedAccessView& operator=(UnorderedAccessView&&);

            protected:

            UnorderedAccessView();
        };
    }
}


