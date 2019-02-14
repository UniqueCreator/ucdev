#include "pch.h"

#include <uc_public/graphics/direct_command_context.h>
#include <uc_public/uc_pimpl_impl.h>

#include "direct_command_context_impl.h"
#include "fence_handle_internal.h"
#include "back_buffer_impl.h"
#include "depth_buffer_impl.h"
#include "depth_stencil_buffer_impl.h"
#include "color_buffer_impl.h"
#include "gpu_virtual_resource_impl.h"
#include "graphics_pipeline_state_impl.h"
#include "descriptor_handle_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        template details::pimpl<DirectCommandContext::Impl>;

        DirectCommandContext::DirectCommandContext() : m_impl()
        {

        }
        
        DirectCommandContext::~DirectCommandContext() = default;
        DirectCommandContext& DirectCommandContext::operator=(DirectCommandContext&&) = default;

        void DirectCommandContext::Clear(const BackBuffer* b)
        {
            auto b0 = b->GetImpl()->buffer();
            m_impl->m_context->clear(b0);
        }

        void DirectCommandContext::Clear(const ColorBuffer* b)
        {
            auto b0 = b->GetImpl()->buffer();
            m_impl->m_context->clear(b0);
        }

        void DirectCommandContext::Clear(const DepthBuffer* b)
        {
            auto b0 = b->GetImpl()->buffer();
            m_impl->m_context->clear(b0);
        }

        void DirectCommandContext::Clear(const DepthBuffer* b, float depth)
        {
            auto b0 = b->GetImpl()->buffer();
            m_impl->m_context->clear_depth(b0, depth);
        }

        void DirectCommandContext::Clear(const DepthStencilBuffer* b, float depth, uint8_t stencil)
        {
            auto b0 = b->GetImpl()->buffer();
            m_impl->m_context->clear_depth_stencil(b0, depth, stencil);
        }

        void DirectCommandContext::SetPipelineStateObject(const GraphicsPipelineState* state)
        {
            m_impl->m_context->set_pso(state->GetImpl()->pipeline_state());
        }

        void DirectCommandContext::Draw(uint32_t vertexCount, uint32_t vertexStartOffset)
        {
            m_impl->m_context->draw(vertexCount, vertexStartOffset);
        }

        void DirectCommandContext::DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startIndexLocation)
        {
            m_impl->m_context->draw_instanced(vertexCountPerInstance, instanceCount, startVertexLocation, startIndexLocation);
        }

        void DirectCommandContext::DrawIndexed(uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVectorLocation)
        {
            m_impl->m_context->draw_indexed(indexCount, startIndexLocation, baseVectorLocation);
        }

        void DirectCommandContext::DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndexLocation, int32_t baseVertexLocation, uint32_t startInstanceLocation)
        {
            m_impl->m_context->draw_indexed_instanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
        }

        void DirectCommandContext::SetVertexBuffer(VertexSlot slot, const VertexBufferView view)
        {
            D3D12_VERTEX_BUFFER_VIEW v = { view.m_bufferLocation, view.m_sizeInBytes, view.m_strideInBytes };
            m_impl->m_context->set_vertex_buffer( static_cast<uint32_t>(slot), v );
        }

        void DirectCommandContext::SetIndexBuffer(const IndexBufferView view)
        {
            D3D12_INDEX_BUFFER_VIEW v = { view.m_bufferLocation, view.m_sizeInBytes, static_cast<DXGI_FORMAT>( view.m_format ) };
            m_impl->m_context->set_index_buffer(v);
        }

        void DirectCommandContext::SetDescriptorHeaps()
        {
            m_impl->m_context->set_descriptor_heaps();
        }

        void DirectCommandContext::SetDynamicDescriptor(RootIndex rootIndex, const DescriptorHandle* h, uint32_t offset)
        {
            m_impl->m_context->set_graphics_dynamic_descriptor(rootIndex, h->GetImpl()->handle(), offset);
        }

        void DirectCommandContext::SetDynamicConstantBuffer(RootIndex rootIndex, uint32_t offset, const void* buffer, size_t byte_count)
        {
            m_impl->m_context->set_graphics_dynamic_constant_buffer(static_cast<uint32_t>(rootIndex), offset, buffer, byte_count);
        }

        void DirectCommandContext::SetPrimitiveTopology( PrimitiveTopology p )
        {
            m_impl->m_context->set_primitive_topology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(p));
        }

        void DirectCommandContext::SetViewPort(ViewPort p)
        {
            D3D12_VIEWPORT v;
            v.TopLeftX  = p.m_topLeftX;
            v.TopLeftY  = p.m_topLeftY;
            v.Width     = p.m_width;
            v.Height    = p.m_height;
            v.MaxDepth  = p.m_maxDepth;
            v.MinDepth  = p.m_minDepth;
            m_impl->m_context->set_view_port(v);
        }

        void DirectCommandContext::SetScissorRectangle( Rectangle2D r )
        {
            D3D12_RECT v;
            v.left  = static_cast<LONG>(r.m_left);
            v.right = static_cast<LONG>(r.m_right);
            v.top   = static_cast<LONG>(r.m_top);
            v.bottom = static_cast<LONG>(r.m_bottom);
            m_impl->m_context->set_scissor_rectangle(v);
        }

        void DirectCommandContext::SetRenderTarget(const BackBuffer* b)
        {
            auto impl = b->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl);
        }

        void DirectCommandContext::SetRenderTarget(const BackBuffer* b, const DepthBuffer* d)
        {
            auto impl0 = b->GetImpl()->buffer();
            auto impl1 = d->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl0, impl1);
        }

        void DirectCommandContext::SetRenderTarget(const BackBuffer* b, const DepthStencilBuffer* d)
        {
            auto impl0 = b->GetImpl()->buffer();
            auto impl1 = d->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl0, impl1);
        }

        void DirectCommandContext::SetRenderTarget(const DepthBuffer* d)
        {
            auto impl1 = d->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl1);
        }

        void DirectCommandContext::SetRenderTarget(const DepthStencilBuffer* d)
        {
            auto impl1 = d->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl1);
        }

        void DirectCommandContext::SetRenderTarget(const ColorBuffer* b, const DepthBuffer* d)
        {
            auto impl0 = b->GetImpl()->buffer();
            auto impl1 = d->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl0, impl1);
        }

        void DirectCommandContext::SetRenderTarget(const ColorBuffer* b, const DepthStencilBuffer* d)
        {
            auto impl0 = b->GetImpl()->buffer();
            auto impl1 = d->GetImpl()->buffer();
            m_impl->m_context->set_render_target(impl0, impl1);
        }

        void DirectCommandContext::SetConstantBufferView(RootIndex rootIndex, GpuVirtualAddress a)
        {
            m_impl->m_context->set_graphics_constant_buffer(static_cast<uint32_t>(rootIndex), a);
        }

        void DirectCommandContext::SetConstantBuffer(RootIndex rootIndex, const void* buffer, size_t buffer_size)
        {
            m_impl->m_context->set_graphics_constant_buffer(static_cast<uint32_t>(rootIndex), buffer, buffer_size);
        }

        void DirectCommandContext::SetShaderResourceViewBuffer(RootIndex rootIndex, const GpuVirtualResource* r)
        {
            auto impl0 = r->GetImpl()->resource();
            m_impl->m_context->set_graphics_srv_buffer(static_cast<uint32_t>(rootIndex), impl0);
        }

        void DirectCommandContext::SetUnorderedAccessViewBuffer(RootIndex rootIndex, const GpuVirtualResource* r)
        {
            auto impl0 = r->GetImpl()->resource();
            m_impl->m_context->set_graphics_uav_buffer(static_cast<uint32_t>(rootIndex), impl0);
        }
    }
}


