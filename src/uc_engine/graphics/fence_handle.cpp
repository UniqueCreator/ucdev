#include "pch.h"

#include <uc_public/graphics/fence_handle.h>
#include <uc_public/uc_pimpl_impl.h>

#include "fence_handle_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<FenceHandle::Impl>;

        FenceHandle::FenceHandle() : m_impl()
        {

        }
        
        FenceHandle::~FenceHandle() = default;
        FenceHandle& FenceHandle::operator=(FenceHandle&&) = default;

        FenceHandle::Impl* FenceHandle::GetImpl()
        {
            return m_impl.get();
        }

        const FenceHandle::Impl* FenceHandle::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


