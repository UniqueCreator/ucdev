#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/pixel_buffer.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DepthStencilBuffer : public PixelBuffer
        {
            public:

            class Impl;

            UC_ENGINE_API ~DepthStencilBuffer();

            UC_ENGINE_API DepthStencilBuffer(DepthStencilBuffer&&) = default;
            UC_ENGINE_API DepthStencilBuffer& operator=(DepthStencilBuffer&&);

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            DepthStencilBuffer();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<DepthStencilBuffer::Impl>;
    }
}


