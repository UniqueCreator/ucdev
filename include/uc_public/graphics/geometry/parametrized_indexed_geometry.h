#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/geometry/position_indexed_geometry.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle;

        class ParametrizedIndexedGeometry : public PositionIndexedGeometry
        {
        public:

            class Impl;

            UC_ENGINE_API ~ParametrizedIndexedGeometry();

            UC_ENGINE_API ParametrizedIndexedGeometry(ParametrizedIndexedGeometry&&) = default;
            UC_ENGINE_API ParametrizedIndexedGeometry& operator=(ParametrizedIndexedGeometry&&);

            UC_ENGINE_API const Buffer* GetParametersBuffer() const;

            UC_ENGINE_API const VertexBufferView GetParametersView() const;
            UC_ENGINE_API const VertexBufferView GetParametersView(uint32_t offset, uint32_t size) const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            ParametrizedIndexedGeometry();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<ParametrizedIndexedGeometry::Impl>;
    }
}




