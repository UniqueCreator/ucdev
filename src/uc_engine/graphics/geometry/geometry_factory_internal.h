#pragma once

#include <uc_public/graphics/geometry/geometry_factory.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ResourceCreateContext;

        class GeometryFactoryInternal : public GeometryFactory
        {
            public:

            GeometryFactoryInternal( ResourceCreateContext* ctx );
        };
    }
}


