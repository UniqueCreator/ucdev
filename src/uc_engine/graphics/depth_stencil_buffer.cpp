#include "pch.h"

#include <uc_public/graphics/depth_stencil_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "depth_stencil_buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<DepthStencilBuffer::Impl>;

        DepthStencilBuffer::DepthStencilBuffer()
        {

        }

        DepthStencilBuffer::~DepthStencilBuffer() = default;
        DepthStencilBuffer& DepthStencilBuffer::operator=(DepthStencilBuffer&&) = default;

        DepthStencilBuffer::Impl*   DepthStencilBuffer::GetImpl()
        {
            return m_impl.get();
        }

        const DepthStencilBuffer::Impl*   DepthStencilBuffer::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


