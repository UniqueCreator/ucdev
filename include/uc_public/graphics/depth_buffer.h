#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/pixel_buffer.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DepthBuffer : public PixelBuffer
        {
            public:

            class Impl;

            UC_ENGINE_API DepthBuffer(DepthBuffer&&) = default;
            UC_ENGINE_API DepthBuffer& operator=(DepthBuffer&&);

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            DepthBuffer();
            ~DepthBuffer();
            details::pimpl<Impl> m_impl;
        };

        class ViewDepthBuffer : public DepthBuffer
        {
            public:

            class Impl;

            UC_ENGINE_API ~ViewDepthBuffer();

            UC_ENGINE_API ViewDepthBuffer(ViewDepthBuffer&&) = default;
            UC_ENGINE_API ViewDepthBuffer& operator=(ViewDepthBuffer&&);

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            ViewDepthBuffer();
            details::pimpl<Impl> m_impl;
        };


        //private data, todo: fix visual assist warning
        extern template details::pimpl<DepthBuffer::Impl>;
        extern template details::pimpl<ViewDepthBuffer::Impl>;
    }
}


