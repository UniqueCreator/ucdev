#include "pch.h"

#include <uc_public/graphics/back_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "back_buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<BackBuffer::Impl>;

        BackBuffer::BackBuffer()
        {

        }

        BackBuffer::~BackBuffer() = default;
        BackBuffer& BackBuffer::operator=(BackBuffer&&) = default;

        BackBuffer::Impl*   BackBuffer::GetImpl()
        {
            return m_impl.get();
        }

        const BackBuffer::Impl*   BackBuffer::GetImpl() const
        {
            return m_impl.get();
        }

        const RenderTargetView* BackBuffer::GetRenderTargetView() const
        {
            return m_impl->GetRenderTargetView();
        }
    }
}


