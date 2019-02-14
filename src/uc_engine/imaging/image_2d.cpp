#include "pch.h"

#include <uc_public/imaging/image_2d.h>
#include <uc_public/uc_pimpl_impl.h>

#include "image_2d_impl.h"

namespace UniqueCreator
{
    namespace Imaging
    {
        template details::pimpl<Image2D::Impl>;

        Image2D::Image2D()
        {

        }

        Image2D::~Image2D() = default;
        Image2D& Image2D::operator=(Image2D&&) = default;

        Image2D::Impl*   Image2D::GetImpl()
        {
            return m_impl.get();
        }

        const Image2D::Impl*   Image2D::GetImpl() const
        {
            return m_impl.get();
        }

        ImageDescription2D Image2D::GetDescription() const
        {
            auto t = m_impl->texture();

            ImageDescription2D r = {};

            r.m_width = t->width();
            r.m_height = t->height();
            r.m_format = static_cast<ImageType>(t->type());
            r.m_row_pitch = t->row_pitch();
            r.m_slice_pitch = t->slice_pitch();
            
            return r;
        }

        const void* Image2D::GetData() const
        {
            return m_impl->texture()->pixels().get_pixels_cpu();
        }
    }
}


