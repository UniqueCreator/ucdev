#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class IndexedGeometryAllocation;
        class SkinnedGeometryAllocation;
        class Texture2D;
    }

    namespace Render
    {
        class SkinnedRenderObject : private NonCopyable
        {
        public:

            struct TexturesDescription final
            {
                Graphics::Texture2D**   m_textures;
                size_t                  m_textures_size;
            };

            class Impl;

            UC_ENGINE_API ~SkinnedRenderObject();

            UC_ENGINE_API SkinnedRenderObject(SkinnedRenderObject&&) = default;
            UC_ENGINE_API SkinnedRenderObject& operator=(SkinnedRenderObject&&);

            UC_ENGINE_API const Graphics::IndexedGeometryAllocation* GetPrimitives() const;
            UC_ENGINE_API const Graphics::SkinnedGeometryAllocation* GetGeometry() const;

            UC_ENGINE_API const Graphics::PrimitiveRangeDescription GetPrimitiveRanges() const;
            UC_ENGINE_API const TexturesDescription GetAlbedoTextures() const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            SkinnedRenderObject();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedRenderObject::Impl>;
    }
}


