#pragma once

#include <uc_public/graphics/resource_create_context.h>

#include <uc_dev/gx/dx12/dx12.h>

#include "buffer_internal.h"
#include "color_buffer_internal.h"
#include "compute_queue_internal.h"
#include "compute_pipeline_state_internal.h"
#include "copy_queue_internal.h"
#include "depth_buffer_internal.h"
#include "direct_queue_internal.h"
#include "graphics_pipeline_state_internal.h"
#include "root_signature_internal.h"
#include "texture_2d_internal.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        class ResourceCreateContext::Impl
        {
            private:

#if defined(_DEBUG)
            Microsoft::WRL::ComPtr<ID3D12Debug1>                        m_debug;
#endif
            Microsoft::WRL::ComPtr<ID3D12Device>                        m_device;

            std::unique_ptr<uc::gx::dx12::gpu_resource_create_context>  m_rc;

            public:

            void CreateD3D12Device();

            ID3D12Device* Device() const
            {
                return m_device.Get();
            }

            ResourceCreateContext::Impl();

            std::unique_ptr<DirectQueue>  CreateDirectQueue();
            std::unique_ptr<ComputeQueue> CreateComputeQueue();
            std::unique_ptr<CopyQueue>    CreateCopyQueue();

            uc::gx::dx12::gpu_resource_create_context* rc() const
            {
                return m_rc.get();
            }

            std::unique_ptr<ViewColorBuffer>            CreateViewColorBuffer(Size2D s, ColorBufferFormat format);
            std::unique_ptr<ViewDepthBuffer>            CreateViewDepthBuffer(Size2D s, DepthBufferFormat format);
            std::unique_ptr<GraphicsPipelineState>      CreateGraphicsPipelineState(const GraphicsPipelineStateDescription* descsription);
            std::unique_ptr<ComputePipelineState>       CreateComputePipelineState( const ComputePipelineStateDescription* description );
            std::unique_ptr<RootSignature>              CreateRootSignature(const RootSignatureByteCode* code);
            std::unique_ptr<Texture2D>                  CreateTexture2D(Size2D s, GraphicsFormat format);
            std::unique_ptr<Texture2D>                  CreateTexture2D(Size2D s, GraphicsFormat format, uint32_t mipCount);
            std::unique_ptr<Buffer>                     CreateBuffer(uint64_t size);

            void Sync();
            void ResetViewDependentResources();
        };
    }
}


