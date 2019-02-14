#include "pch.h"

#include <uc_public/graphics/back_buffer.h>

#include <uc_dev/gx/dx12/dx12.h>

#include "gpu_virtual_resource_impl.h"
#include "render_target_view_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class BackBuffer::Impl
        {
            using Base = GpuVirtualResource::Impl;

            public:

            BackBuffer::Impl()
            {

            }

            BackBuffer::Impl(std::unique_ptr<uc::gx::dx12::gpu_back_buffer>&& b ) : 
                m_buffer(std::move(b))
                , m_renderTargetView(m_buffer->rtv())
            {

            }

            uc::gx::dx12::gpu_back_buffer* buffer() const
            {
                return m_buffer.get();
            }

            const RenderTargetView* GetRenderTargetView() const
            {
                return &m_renderTargetView;
            }

            private:

            std::unique_ptr< uc::gx::dx12::gpu_back_buffer > m_buffer;
            RenderTargetViewInternal                         m_renderTargetView;
        };
    }
}


