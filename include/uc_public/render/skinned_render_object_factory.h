#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class SkinnedModel;
    }

    namespace Graphics
    {
        class IndexedGeometryFactory;
        class SkinnedGeometryFactory;
        class UploadQueue;
        class ResourceCreateContext;
    }

    namespace Render
    {
        class SkinnedRenderObject;

        class SkinnedRenderObjectFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API SkinnedRenderObjectFactory();
            UC_ENGINE_API ~SkinnedRenderObjectFactory();

            UC_ENGINE_API SkinnedRenderObjectFactory(SkinnedRenderObjectFactory&&) = default;
            UC_ENGINE_API SkinnedRenderObjectFactory& operator=(SkinnedRenderObjectFactory&&);

            UC_ENGINE_API std::unique_ptr<SkinnedRenderObject> Create(const Assets::SkinnedModel* asset, Graphics::SkinnedGeometryFactory* geometry, Graphics::IndexedGeometryFactory* primitives, Graphics::UploadQueue* uploadQueue, Graphics::ResourceCreateContext* resourceCreateContext );

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<SkinnedRenderObjectFactory::Impl>;
    }
}


