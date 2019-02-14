#include "pch.h"

#include "indexed_geometry_factory_impl.h"
#include "../resource_create_context_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        IndexedGeometryFactory::Impl::Impl(ResourceCreateContext* rc, uint32_t vertexCount)  : m_index_allocator( std::make_unique<uc::gx::geo::indexed_geometry_allocator>( rc->GetImpl()->rc(), vertexCount)),
            m_indices(m_index_allocator->indices())
        {
            
        }

        uc::gx::geo::indexed_geometry_allocator*  IndexedGeometryFactory::Impl::allocator() const
        {
            return m_index_allocator.get();
        }

        const Buffer* IndexedGeometryFactory::Impl::indices() const
        {
            return &m_indices;
        }
    }
}


