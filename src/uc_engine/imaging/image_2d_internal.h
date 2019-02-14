#pragma once

#include <uc_public/imaging/image_2d.h>
#include <uc_dev/gx/img/img.h>


namespace UniqueCreator
{
    namespace Imaging
    {
        class Image2DInternal : public Image2D
        {
        public:

            Image2DInternal(uc::gx::imaging::cpu_texture&& p);
        };
    }
}


