#include "pch.h"

#include <uc_public/graphics/geometry/parametrized_indexed_geometry.h>
#include <uc_public/uc_pimpl_impl.h>

#include "indexed_geometry_impl.h"
#include "../buffer_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<IndexedGeometry::Impl>;

        IndexedGeometry::IndexedGeometry()
        {

        }

        IndexedGeometry::~IndexedGeometry() = default;
        IndexedGeometry& IndexedGeometry::operator=(IndexedGeometry&&) = default;

        IndexedGeometry::Impl*   IndexedGeometry::GetImpl()
        {
            return m_impl.get();
        }

        const IndexedGeometry::Impl*   IndexedGeometry::GetImpl() const
        {
            return m_impl.get();
        }

        const Buffer* IndexedGeometry::GetIndicesBuffer() const
        {
            return m_impl->indices();
        }

        const IndexBufferView IndexedGeometry::GetIndicesView() const
        {
            IndexBufferView r;
            r.m_bufferLocation  = m_impl->indices()->GetVirtualAddress();
            r.m_format          = IndexBufferFormat::Bytes4;
            r.m_sizeInBytes = static_cast<uint32_t>(uc::gx::dx12::size(m_impl->indices()->GetImpl()->buffer()));

            return r;
        }

        const IndexBufferView IndexedGeometry::GetIndicesView(uint32_t offset, uint32_t size) const
        {
            {
                assert(size < static_cast<uint32_t>(uc::gx::dx12::size(m_impl->indices()->GetImpl()->buffer())));
            }
            IndexBufferView r;
            r.m_bufferLocation  = m_impl->indices()->GetVirtualAddress() + offset;
            r.m_format          = IndexBufferFormat::Bytes4;
            r.m_sizeInBytes     = size;

            return r;
        }
    }
}


