#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/pixel_buffer.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RenderTargetView;

        class BackBuffer : public PixelBuffer
        {
            public:

            class Impl;

            UC_ENGINE_API ~BackBuffer();

            UC_ENGINE_API BackBuffer(BackBuffer&&) = default;
            UC_ENGINE_API BackBuffer& operator=(BackBuffer&&);

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            const RenderTargetView* GetRenderTargetView() const;

            protected:
            BackBuffer();
            details::pimpl<Impl> m_impl;

        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<BackBuffer::Impl>;
    }
}


