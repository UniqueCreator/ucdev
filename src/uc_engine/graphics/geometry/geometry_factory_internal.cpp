#include "pch.h"

#include "geometry_factory_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "geometry_factory_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        GeometryFactoryInternal::GeometryFactoryInternal(ResourceCreateContext* ctx)
        {
            this->m_impl = ctx;
        }
    }
}


