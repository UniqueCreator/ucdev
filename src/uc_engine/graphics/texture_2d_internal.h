#pragma once

#include <uc_public/graphics/texture_2d.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class Texture2DInternal : public Texture2D
        {
            public:

            Texture2DInternal
            (
                uc::gx::dx12::managed_gpu_texture_2d && b
            );
        };
    }
}


