#pragma once

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class Buffer;
        class ResourceCreateContext;
        class SkinnedGeometryAllocation;

        class SkinnedGeometryFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API SkinnedGeometryFactory(ResourceCreateContext* c, uint32_t vertexCount);

            UC_ENGINE_API ~SkinnedGeometryFactory();

            UC_ENGINE_API SkinnedGeometryFactory(SkinnedGeometryFactory&&) = default;
            UC_ENGINE_API SkinnedGeometryFactory& operator=(SkinnedGeometryFactory&&);

            UC_ENGINE_API std::unique_ptr<SkinnedGeometryAllocation>    CreateSkinnedGeometry(uint32_t vertexCount);

            UC_ENGINE_API const Buffer*  GetPositions() const;
            UC_ENGINE_API const Buffer*  GetUV() const;
            UC_ENGINE_API const Buffer*  GetBlendWeights() const;
            UC_ENGINE_API const Buffer*  GetBlendIndices() const;

            UC_ENGINE_API const VertexBufferView GetPositionsView() const;
            UC_ENGINE_API const VertexBufferView GetUVView() const;
            UC_ENGINE_API const VertexBufferView GetBlendWeightsView() const;
            UC_ENGINE_API const VertexBufferView GetBlendIndicesView() const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            UC_ENGINE_API void Sync();

        protected:
            SkinnedGeometryFactory();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedGeometryFactory::Impl>;
    }
}


