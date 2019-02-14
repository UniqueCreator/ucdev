#include "pch.h"

#include <uc_public/graphics/depth_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "depth_buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<DepthBuffer::Impl>;

        DepthBuffer::DepthBuffer()
        {

        }

        DepthBuffer::~DepthBuffer() = default;
        DepthBuffer& DepthBuffer::operator=(DepthBuffer&&) = default;

        DepthBuffer::Impl*   DepthBuffer::GetImpl()
        {
            return m_impl.get();
        }

        const DepthBuffer::Impl*   DepthBuffer::GetImpl() const
        {
            return m_impl.get();
        }


        template details::pimpl<ViewDepthBuffer::Impl>;

        ViewDepthBuffer::ViewDepthBuffer()
        {

        }

        ViewDepthBuffer::~ViewDepthBuffer() = default;
        ViewDepthBuffer& ViewDepthBuffer::operator=(ViewDepthBuffer&&) = default;

        ViewDepthBuffer::Impl*   ViewDepthBuffer::GetImpl()
        {
            return m_impl.get();
        }

        const ViewDepthBuffer::Impl*   ViewDepthBuffer::GetImpl() const
        {
            return m_impl.get();
        }

    }
}


