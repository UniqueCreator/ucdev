#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>
#include <uc_public/graphics/gpu_command_context.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class BackBuffer;
        class ColorBuffer;
        class DepthBuffer;
        class DepthStencilBuffer;
        class DescriptorHandle;
        class FenceHandle;
        class GpuVirtualResource;
        class GraphicsPipelineState;

        class DirectCommandContext : public GpuCommandContext
        {
            using Base = GpuCommandContext;

            public:

            class Impl;
            UC_ENGINE_API ~DirectCommandContext();

            UC_ENGINE_API DirectCommandContext(DirectCommandContext&&) = default;
            UC_ENGINE_API DirectCommandContext& operator=(DirectCommandContext&&);

            UC_ENGINE_API void Clear(const BackBuffer* b);
            UC_ENGINE_API void Clear(const ColorBuffer* b);
            UC_ENGINE_API void Clear(const DepthBuffer* b);
            UC_ENGINE_API void Clear(const DepthBuffer* b, float depth);
            UC_ENGINE_API void Clear(const DepthStencilBuffer* b, float depth, uint8_t stencil);

            UC_ENGINE_API void Draw(uint32_t vertexCount, uint32_t vertexStartOffset);
            UC_ENGINE_API void DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVectorLocation);
            UC_ENGINE_API void DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation);
            UC_ENGINE_API void DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startIndexLocation);
            
            UC_ENGINE_API void SetConstantBufferView(RootIndex rootIndex, GpuVirtualAddress a);
            UC_ENGINE_API void SetConstantBuffer(RootIndex rootIndex, const void* buffer, size_t buffer_size);
            UC_ENGINE_API void SetDescriptorHeaps();

            UC_ENGINE_API void SetDynamicDescriptor(RootIndex rootIndex, const DescriptorHandle* h, uint32_t offset);
            UC_ENGINE_API void SetDynamicConstantBuffer(RootIndex rootIndex, uint32_t offset, const void* buffer, size_t byte_count);

            UC_ENGINE_API void SetIndexBuffer(const IndexBufferView view);
            UC_ENGINE_API void SetPipelineStateObject(const GraphicsPipelineState* state);
            UC_ENGINE_API void SetPrimitiveTopology(PrimitiveTopology p);
            UC_ENGINE_API void SetRenderTarget(const BackBuffer* b);
            UC_ENGINE_API void SetRenderTarget(const BackBuffer* b, const DepthBuffer* d);
            UC_ENGINE_API void SetRenderTarget(const BackBuffer* b, const DepthStencilBuffer* d);
            UC_ENGINE_API void SetRenderTarget(const DepthBuffer* d);
            UC_ENGINE_API void SetRenderTarget(const DepthStencilBuffer* d);
            UC_ENGINE_API void SetRenderTarget(const ColorBuffer* b, const DepthBuffer* d);
            UC_ENGINE_API void SetRenderTarget(const ColorBuffer* b, const DepthStencilBuffer* d);

            UC_ENGINE_API void SetScissorRectangle(Rectangle2D r);
            UC_ENGINE_API void SetShaderResourceViewBuffer(RootIndex rootIndex, const GpuVirtualResource* r);
            UC_ENGINE_API void SetVertexBuffer(VertexSlot slot, const VertexBufferView view);
            UC_ENGINE_API void SetViewPort(ViewPort p);
            UC_ENGINE_API void SetUnorderedAccessViewBuffer(RootIndex rootIndex, const GpuVirtualResource* r);


            protected:
            DirectCommandContext();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<DirectCommandContext::Impl>;
    }
}


