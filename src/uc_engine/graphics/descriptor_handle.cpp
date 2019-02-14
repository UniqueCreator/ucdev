#include "pch.h"

#include <uc_public/graphics/descriptor_handle.h>
#include <uc_public/uc_pimpl_impl.h>

#include "descriptor_handle_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<DescriptorHandle::Impl>;

        DescriptorHandle::DescriptorHandle()
        {

        }

        DescriptorHandle::~DescriptorHandle() = default;
        DescriptorHandle& DescriptorHandle::operator=(DescriptorHandle&&) = default;

        DescriptorHandle::Impl*   DescriptorHandle::GetImpl()
        {
            return m_impl.get();
        }

        const DescriptorHandle::Impl*   DescriptorHandle::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


