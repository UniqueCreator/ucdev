#pragma once

#include <uc_public/imaging/image_2d.h>

#include <uc_dev/gx/img/img.h>

namespace UniqueCreator
{
    namespace Imaging
    {
        class Image2D::Impl
        {
            public:

            Image2D::Impl() {}

            Image2D::Impl(uc::gx::imaging::cpu_texture&& t) : m_texture(std::move(t)) {}

            private:
            uc::gx::imaging::cpu_texture m_texture;

            public:

            const uc::gx::imaging::cpu_texture* texture() const
            {
                return &m_texture;
            }
        };
    }
}


