#pragma once

#include <uc_public/uc_implementation.h>
#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle;
        class GpuVirtualResource;
        class Texture2D;

        class UploadQueue : private NonCopyable
        {
            public:
            class Impl;
            UC_ENGINE_API ~UploadQueue();

            UC_ENGINE_API UploadQueue(UploadQueue&&) = default;
            UC_ENGINE_API UploadQueue& operator=(UploadQueue&&);

           
            UC_ENGINE_API std::unique_ptr<FenceHandle> Flush();
            UC_ENGINE_API void Sync();

            UC_ENGINE_API void UploadBuffer(const GpuVirtualResource* destination, const void* initialData, uint64_t size, uint64_t offset = 0 );
            UC_ENGINE_API void UploadTexture(const Texture2D* texture, const SubresourceData*, uint32_t firstSubResource, uint32_t subResourceCount);

            protected:
            UploadQueue();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<UploadQueue::Impl>;
    }
}


