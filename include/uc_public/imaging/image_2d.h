#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/imaging/image_types.h>

namespace UniqueCreator
{
    namespace Imaging
    {
        struct ImageDescription2D final
        {
            uint32_t    m_width;
            uint32_t    m_height;
            uint32_t    m_row_pitch;
            uint32_t    m_slice_pitch;
            ImageType   m_format;
        };

        class Image2D : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API ~Image2D();

            UC_ENGINE_API Image2D(Image2D&&) = default;
            UC_ENGINE_API Image2D& operator=(Image2D&&);

            UC_ENGINE_API ImageDescription2D GetDescription() const;
            UC_ENGINE_API const void* GetData() const;


            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            Image2D();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Image2D::Impl>;
    }
}

            

