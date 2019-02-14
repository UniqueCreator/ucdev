#include "pch.h"

#include "image_2d_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "image_2d_impl.h"

namespace UniqueCreator
{
    namespace Imaging
    {
        Image2DInternal::Image2DInternal(uc::gx::imaging::cpu_texture&& p)
        {
            m_impl = details::pimpl<Impl>(std::move(p));
        }
    }
}


