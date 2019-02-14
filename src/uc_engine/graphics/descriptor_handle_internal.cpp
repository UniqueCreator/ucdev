#include "pch.h"

#include "descriptor_handle_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "descriptor_handle_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        DescriptorHandleIntenral::DescriptorHandleIntenral(uc::gx::dx12::descriptor_handle handle)
        {
            m_impl                    = DescriptorHandle::Impl(handle);
            DescriptorHandle::m_impl  = DescriptorHandle::Impl(handle);
        }
    }
}


