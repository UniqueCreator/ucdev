#include "pch.h"

#include "indexed_geometry_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "indexed_geometry_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        IndexedGeometryInternal::IndexedGeometryInternal()
        {

        }

        IndexedGeometryInternal::IndexedGeometryInternal(uc::gx::dx12::managed_gpu_buffer&& indices)
        {
            m_impl = details::pimpl<Impl>(std::move(indices));
        }
    }
}


