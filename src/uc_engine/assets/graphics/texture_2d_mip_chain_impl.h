#pragma once

#include <uc_public/assets/graphics/texture_2d_mip_chain.h>
#include <uc_dev/gx/lip/model.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Texture2DMipChain::Impl
        {
            public:

            Impl() {}

            Impl(uc::lip::texture2d_mip_chain* texture) : m_texture(texture)
            {

            }

            uc::lip::texture2d_mip_chain* m_texture = nullptr;
        };
    }
}


