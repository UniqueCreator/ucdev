#include "pch.h"

#include <uc_public/graphics/pixel_buffer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "pixel_buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<PixelBuffer::Impl>;

        PixelBuffer::PixelBuffer()
        {

        }

        PixelBuffer::~PixelBuffer() = default;
        PixelBuffer& PixelBuffer::operator=(PixelBuffer&&) = default;

        UC_ENGINE_API Size2D PixelBuffer::GetSize2D() const
        {
            auto r = Base::GetImpl()->resource()->desc();
            return Size2D{ static_cast<float>(r.Width), static_cast<float>(r.Height) };
        }

        UC_ENGINE_API Size3D PixelBuffer::GetSize3D() const
        {
            auto r = Base::GetImpl()->resource()->desc();
            return Size3D{ static_cast<float>(r.Width), static_cast<float>(r.Height), static_cast<float>(r.DepthOrArraySize) };
        }

    }
}


