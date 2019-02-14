#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/pixel_buffer.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DescriptorHandle;

        class ColorBuffer : public PixelBuffer
        {
            public:

            class Impl;

            UC_ENGINE_API ColorBuffer(ColorBuffer&&) = default;
            UC_ENGINE_API ColorBuffer& operator=(ColorBuffer&&);

            UC_ENGINE_API const DescriptorHandle* GetRenderTargetView() const;
            UC_ENGINE_API const DescriptorHandle* GetShaderResourceView() const;
            UC_ENGINE_API const DescriptorHandle* GetUnorderedAccessView() const;

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            ColorBuffer();
            ~ColorBuffer();
            details::pimpl<Impl> m_impl;

        };

        class ViewColorBuffer : public ColorBuffer
        {
            public:

            class Impl;

            UC_ENGINE_API ~ViewColorBuffer();

            UC_ENGINE_API ViewColorBuffer(ViewColorBuffer&&) = default;
            UC_ENGINE_API ViewColorBuffer& operator=(ViewColorBuffer&&);

            Impl*   GetImpl();
            const Impl*   GetImpl() const;

            protected:
            ViewColorBuffer();
            details::pimpl<Impl> m_impl;

        };


        //private data, todo: fix visual assist warning
        extern template details::pimpl<ColorBuffer::Impl>;
        extern template details::pimpl<ViewColorBuffer::Impl>;
    }
}


