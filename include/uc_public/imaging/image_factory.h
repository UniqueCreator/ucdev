#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/imaging/image_types.h>

namespace UniqueCreator
{
    namespace Imaging
    {
        class Image2D;

        class ImageFactory : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API ImageFactory();
            UC_ENGINE_API ~ImageFactory();

            UC_ENGINE_API ImageFactory(ImageFactory&&) = default;
            UC_ENGINE_API ImageFactory& operator=(ImageFactory&&);

            UC_ENGINE_API std::unique_ptr<Image2D> CreateFromFile(const wchar_t* fileName);
            UC_ENGINE_API std::unique_ptr<Image2D> CreateCheckerBoardImage(uint32_t width, uint32_t height);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            protected:
            
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<ImageFactory::Impl>;
    }
}

            

