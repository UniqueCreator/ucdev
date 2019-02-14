#include "pch.h"

#include "render_target_view_internal.h"
#include <uc_public/uc_pimpl_impl.h>
#include "descriptor_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        RenderTargetViewInternal::RenderTargetViewInternal()
        {

        }

        RenderTargetViewInternal::RenderTargetViewInternal(uc::gx::dx12::descriptor_handle handle)
        {
            m_impl = DescriptorHandle::Impl(handle);
        }
    }
}


