#include "pch.h"

#include <uc_public/graphics/geometry/indexed_geometry_factory.h>
#include <uc_public/graphics/geometry/indexed_geometry_allocation.h>
#include <uc_public/graphics/graphics_types.h>
#include <uc_public/uc_pimpl_impl.h>


#include "indexed_geometry_factory_impl.h"
#include "indexed_geometry_allocation_impl.h"
#include "../resource_create_context_impl.h"


namespace UniqueCreator
{
    namespace Graphics
    {
        class IndexedGeometryAllocationInternal : public IndexedGeometryAllocation
        {
            public:

            IndexedGeometryAllocationInternal(uc::gx::geo::managed_indexed_allocation&& mesh)
            {
                m_impl = std::move(mesh);
            }
        };

        template details::pimpl<IndexedGeometryFactory::Impl>;

        IndexedGeometryFactory::IndexedGeometryFactory(ResourceCreateContext* c, uint32_t vertexCount) : m_impl(c, vertexCount)
        {

        }

        IndexedGeometryFactory::IndexedGeometryFactory()
        {

        }

        IndexedGeometryFactory::~IndexedGeometryFactory() = default;
        IndexedGeometryFactory& IndexedGeometryFactory::operator=(IndexedGeometryFactory&&) = default;

        IndexedGeometryFactory::Impl*   IndexedGeometryFactory::GetImpl()
        {
            return m_impl.get();
        }

        const IndexedGeometryFactory::Impl*   IndexedGeometryFactory::GetImpl() const
        {
            return m_impl.get();
        }

        void IndexedGeometryFactory::Sync()
        {
            m_impl->allocator()->sync();
        }

        const IndexBufferView IndexedGeometryFactory::GetIndicesView() const
        {
            IndexBufferView r;
            auto v = m_impl->allocator()->indices_view();
            r.m_bufferLocation = v.BufferLocation;
            r.m_sizeInBytes = v.SizeInBytes;
            r.m_format = static_cast<IndexBufferFormat>(v.Format);
            return r;
        }
        
        const Buffer*  IndexedGeometryFactory::GetIndices() const
        {
            return m_impl->indices();
        }

        std::unique_ptr<IndexedGeometryAllocation> IndexedGeometryFactory::CreateIndexedGeometry(uint32_t indexCount)
        {
            return std::make_unique<IndexedGeometryAllocationInternal>(uc::gx::geo::make_indexed_allocation(m_impl->allocator(),indexCount));
        }
    }
}


