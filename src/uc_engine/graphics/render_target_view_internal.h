#include "pch.h"

#include <uc_public/graphics/render_target_view.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RenderTargetViewInternal final : public RenderTargetView
        {
            using Base = RenderTargetView;

            public:
            RenderTargetViewInternal();
            RenderTargetViewInternal(uc::gx::dx12::descriptor_handle handle );
        };
    }
}


