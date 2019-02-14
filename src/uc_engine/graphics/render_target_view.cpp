#include "pch.h"

#include <uc_public/graphics/render_target_view.h>
#include <uc_public/uc_pimpl_impl.h>
#include "descriptor_handle_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        RenderTargetView::RenderTargetView()
        {

        }

        RenderTargetView::~RenderTargetView() = default;
        RenderTargetView& RenderTargetView::operator=(RenderTargetView&&) = default;
    }
}


