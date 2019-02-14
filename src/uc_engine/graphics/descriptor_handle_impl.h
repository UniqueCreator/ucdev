#pragma once

#include <uc_public/graphics/descriptor_handle.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DescriptorHandle::Impl
        {
            uc::gx::dx12::descriptor_handle             m_handle;

            public:
            Impl();

            Impl( uc::gx::dx12::descriptor_handle handle ) : m_handle(handle) {}

            uc::gx::dx12::descriptor_handle handle() const
            {
                return m_handle;
            }
        };
    }
}


