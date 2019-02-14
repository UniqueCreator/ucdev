#include "pch.h"

#include <uc_public/graphics/unordered_access_view.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class UnorderedAccessViewInternal final : public UnorderedAccessView
        {
            using Base = UnorderedAccessView;

            public:
            UnorderedAccessViewInternal();
            UnorderedAccessViewInternal(uc::gx::dx12::descriptor_handle handle );
        };
    }
}


