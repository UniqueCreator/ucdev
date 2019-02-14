#include "pch.h"

#include <uc_public/graphics/upload_queue.h>
#include <uc_public/uc_pimpl_impl.h>

#include "upload_queue_impl.h"
#include "fence_handle_internal.h"
#include "gpu_virtual_resource_impl.h"
#include "texture_2d_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<UploadQueue::Impl>;

        
        UploadQueue::UploadQueue() : m_impl()
        {

        }
      
        UploadQueue::~UploadQueue() = default;
        UploadQueue& UploadQueue::operator=(UploadQueue&&) = default;

        std::unique_ptr<FenceHandle> UploadQueue::Flush()
        {
            auto fence = m_impl->queue()->flush();
            return std::make_unique<FenceHandleInternal>(fence);
        }

        void UploadQueue::Sync()
        {
            m_impl->queue()->sync();
        }

        void UploadQueue::UploadBuffer(const GpuVirtualResource* destination, const void* initial_data, uint64_t size, uint64_t offset)
        {
            auto d = destination->GetImpl()->resource();
            auto des = d->desc();
            assert(d->desc().Width > 0);
            m_impl->queue()->upload_buffer(d, initial_data, size, offset );
        }

        void UploadQueue::UploadTexture(const Texture2D* texture, const SubresourceData* subresoureData, uint32_t firstSubResource, uint32_t subResourceCount)
        {
            if (subResourceCount == 1)
            {
                D3D12_SUBRESOURCE_DATA r[1];

                r[0].pData      = subresoureData->m_data;
                r[0].RowPitch   = static_cast<LONG_PTR>(subresoureData->m_rowPitch);
                r[0].SlicePitch = static_cast<LONG_PTR>(subresoureData->m_slicePitch);

                auto d          = texture->GetImpl()->texture();
                m_impl->queue()->upload_texture_2d(d, firstSubResource, subResourceCount, r);
            }
            else
            {
                std::vector<D3D12_SUBRESOURCE_DATA> r;
                r.resize(subResourceCount);

                for (auto i = 0U; i < subResourceCount; ++i)
                {
                    r[i].pData = subresoureData->m_data;
                    r[i].RowPitch = static_cast<LONG_PTR>(subresoureData->m_rowPitch);
                    r[i].SlicePitch = static_cast<LONG_PTR>(subresoureData->m_slicePitch);
                }

                auto d          = texture->GetImpl()->texture();
                m_impl->queue()->upload_texture_2d(d, firstSubResource, subResourceCount, &r[0]);
                
            }
        }
    }
}


