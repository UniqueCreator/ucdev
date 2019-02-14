#include "pch.h"

#include <uc_public/graphics/color_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "color_buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<ColorBuffer::Impl>;

        ColorBuffer::ColorBuffer()
        {

        }

        ColorBuffer::~ColorBuffer() = default;
        ColorBuffer& ColorBuffer::operator=(ColorBuffer&&) = default;

        ColorBuffer::Impl*   ColorBuffer::GetImpl()
        {
            return m_impl.get();
        }

        const ColorBuffer::Impl*   ColorBuffer::GetImpl() const
        {
            return m_impl.get();
        }



        template details::pimpl<ViewColorBuffer::Impl>;

        ViewColorBuffer::ViewColorBuffer()
        {

        }

        ViewColorBuffer::~ViewColorBuffer() = default;
        ViewColorBuffer& ViewColorBuffer::operator=(ViewColorBuffer&&) = default;

        ViewColorBuffer::Impl*   ViewColorBuffer::GetImpl()
        {
            return m_impl.get();
        }

        const ViewColorBuffer::Impl*   ViewColorBuffer::GetImpl() const
        {
            return m_impl.get();
        }
    }
}


