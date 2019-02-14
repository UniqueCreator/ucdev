#pragma once

#include <uc_public/graphics/geometry/geometry_factory.h>
#include <uc_public/graphics/resource_create_context.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        class GeometryFactory::Impl
        {
            public:

            GeometryFactory::Impl() {}
            GeometryFactory::Impl(ResourceCreateContext* rc) : m_rc(rc) {}
            
            ResourceCreateContext* rc() const
            {
                return m_rc;
            }

            private:
            ResourceCreateContext* m_rc;
        };
    }
}


