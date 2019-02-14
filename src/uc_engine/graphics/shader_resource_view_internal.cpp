#include "pch.h"

#include "shader_resource_view_internal.h"
#include <uc_public/uc_pimpl_impl.h>
#include "descriptor_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        ShaderResourceViewInternal::ShaderResourceViewInternal()
        {

        }

        ShaderResourceViewInternal::ShaderResourceViewInternal(uc::gx::dx12::descriptor_handle handle)
        {
            m_impl                      = DescriptorHandle::Impl(handle);
        }
    }
}


