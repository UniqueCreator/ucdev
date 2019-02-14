#include "pch.h"

#include <uc_public/graphics/shader_resource_view.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ShaderResourceViewInternal final : public ShaderResourceView
        {
            using Base = ShaderResourceView;

            public:
            ShaderResourceViewInternal();
            ShaderResourceViewInternal(uc::gx::dx12::descriptor_handle handle );
        };
    }
}


