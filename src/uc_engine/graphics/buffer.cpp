#include "pch.h"

#include <uc_public/graphics/buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<Buffer::Impl>;

        Buffer::Buffer()
        {

        }

        Buffer::~Buffer() = default;
        Buffer& Buffer::operator=(Buffer&&) = default;

        Buffer::Impl*   Buffer::GetImpl()
        {
            return m_impl.get();
        }

        const Buffer::Impl*   Buffer::GetImpl() const
        {
            return m_impl.get();
        }

        UC_ENGINE_API GpuVirtualAddress Buffer::GetVirtualAddress() const
        {
            return m_impl->buffer()->virtual_address();
        }

        UC_ENGINE_API uint32_t  Buffer::GetSize() const
        {
            return static_cast<uint32_t>(uc::gx::dx12::size(m_impl->buffer()));
        }
    }
}


