#pragma once

#include <uc_public/assets/graphics/texture_2d.h>
#include <uc_dev/gx/lip/model.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Texture2D::Impl
        {
            public:

            Impl() {}

            Impl(uc::lip::texture2d* texture) : m_texture(texture)
            {

            }

            uc::lip::texture2d* m_texture = nullptr;
        };
    }
}


