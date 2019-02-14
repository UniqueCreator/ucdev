#pragma once

#include <uc_dev/gx/dx12/gpu/back_buffer.h>
#include <uc_dev/gx/dx12/gpu/color_buffer.h>
#include <uc_dev/gx/dx12/gpu/depth_buffer.h>
#include <uc_dev/gx/dx12/gpu/depth_stencil_buffer.h>
#include <uc_dev/gx/dx12/gpu/pipeline_state.h>
#include <uc_dev/gx/dx12/gpu/resource_util.h>

#include "command_context.h"

#include <uc_dev/mem/align.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct gpu_graphics_command_context : public gpu_command_context
            {
                using base = gpu_command_context;

                public:

                void clear(gpu_back_buffer* r)
                {
                    flush_resource_barriers();
                    float c[] = { 0.0f, 0.0f, 0.0f, 0.0f };

                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->rtv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_rtv();
                        m_device->CopyDescriptorsSimple(1, h, r->rtv(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                    }

                    list()->ClearRenderTargetView(h, c, 0, nullptr);
                }


                void clear(gpu_color_buffer* r)
                {
                    flush_resource_barriers();
                    float c[] = { 0.0f, 0.0f, 0.0f, 0.0f };

                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->rtv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_rtv();
                        m_device->CopyDescriptorsSimple(1, h, r->rtv(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                    }

                    list()->ClearRenderTargetView(h, c, 0, nullptr);
                }

                void clear(gpu_depth_buffer* r)
                {
                    clear_depth(r, 1.0f);
                }

                void clear(gpu_depth_stencil_buffer* r)
                {
                    clear_depth_stencil(r, 1.0f, 0 );
                }

                void clear_depth( gpu_depth_buffer* r, float depth )
                {
                    flush_resource_barriers();

                    uint32_t stencil = 0;

                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->dsv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_dsv();
                        m_device->CopyDescriptorsSimple(1, h, r->dsv(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                    }

                    list()->ClearDepthStencilView(h, D3D12_CLEAR_FLAG_DEPTH, depth, static_cast<uint8_t>(stencil), 0, nullptr);
                }

                void clear_depth(gpu_depth_stencil_buffer* r, float depth)
                {
                    flush_resource_barriers();

                    uint32_t stencil = 0;

                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->dsv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_dsv();
                        m_device->CopyDescriptorsSimple(1, h, r->dsv(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                    }

                    list()->ClearDepthStencilView(h, D3D12_CLEAR_FLAG_DEPTH, depth, static_cast<uint8_t>(stencil), 0, nullptr);
                }

                void clear_stencil(gpu_depth_stencil_buffer* r, uint32_t stencil )
                {
                    float depth = 1.0f;
                    flush_resource_barriers();
                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->dsv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_dsv();
                        m_device->CopyDescriptorsSimple(1, h, r->dsv(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                    }

                    list()->ClearDepthStencilView(h, D3D12_CLEAR_FLAG_STENCIL, depth, static_cast<uint8_t>(stencil), 0, nullptr);
                }

                void clear_depth_stencil(gpu_depth_stencil_buffer* r, float depth = 1.0f, uint32_t stencil = 0)
                {
                    flush_resource_barriers();

                    D3D12_CPU_DESCRIPTOR_HANDLE h;
                    if (r->is_shader_visible())
                    {
                        h = r->dsv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_dsv();
                        m_device->CopyDescriptorsSimple(1, h, r->dsv(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                    }

                    list()->ClearDepthStencilView(h, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, static_cast<uint8_t>(stencil), 0, nullptr);
                }

                void set_render_target( gpu_back_buffer* r )
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->rtv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_rtv();
                        m_device->CopyDescriptorsSimple(1, h, r->rtv(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                    }

                    D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { h };

                    list()->OMSetRenderTargets(1, handles, FALSE, nullptr);
                }

                void set_render_target(gpu_color_buffer* r)
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE h;

                    if (r->is_shader_visible())
                    {
                        h = r->rtv();
                    }
                    else
                    {
                        h = m_descriptor_cache.allocate_rtv();
                        m_device->CopyDescriptorsSimple(1, h, r->rtv(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                    }
                    D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { h };
                    list()->OMSetRenderTargets(1, handles, FALSE, nullptr);
                }

                void set_render_target( gpu_back_buffer* r, gpu_depth_buffer* b )
                {
                    set_render_target(r->rtv(), b->dsv(), r->is_shader_visible(), b->is_shader_visible());
                }

                void set_render_target(gpu_back_buffer* r, gpu_depth_stencil_buffer* b)
                {
                    set_render_target(r->rtv(), b->dsv(), r->is_shader_visible(), b->is_shader_visible());
                }

                void set_render_target(gpu_depth_buffer* b)
                {
                    set_render_target_dsv(b->dsv(), b->is_shader_visible());
                }

                void set_render_target(gpu_depth_stencil_buffer* b)
                {
                    set_render_target_dsv(b->dsv(), b->is_shader_visible());
                }

                void set_render_target( gpu_color_buffer* r, gpu_depth_buffer* b )
                {
                    set_render_target(r->rtv(), b->dsv(), r->is_shader_visible(), b->is_shader_visible());
                }

                void set_render_target(gpu_color_buffer* r, gpu_depth_stencil_buffer* b)
                {
                    set_render_target(r->rtv(), b->dsv(), r->is_shader_visible(), b->is_shader_visible());
                }

                void draw(uint32_t vertex_count, uint32_t vertex_start_offset = 0)
                {
                    draw_instanced(vertex_count, 1, vertex_start_offset);
                }

                void draw_instanced(uint32_t vertex_count_per_instance, uint32_t instance_count, uint32_t start_vertex_location = 0, uint32_t start_instance_location = 0)
                {
                    flush_resource_barriers();
                    commit_graphics_root_descriptor_tables();
                    list()->DrawInstanced(vertex_count_per_instance, instance_count, start_vertex_location, start_instance_location);
                }

                void draw_indexed(uint32_t index_count, uint32_t start_index_location = 0, int32_t base_vertex_location = 0)
                {
                    draw_indexed_instanced(index_count, 1, start_index_location, base_vertex_location, 0);
                }

                void draw_indexed_instanced(uint32_t index_count_per_instance, uint32_t instance_count, uint32_t start_index_location, int32_t base_vertex_location, uint32_t start_instance_location)
                {
                    flush_resource_barriers();
                    commit_graphics_root_descriptor_tables();
                    list()->DrawIndexedInstanced(index_count_per_instance, instance_count, start_index_location, base_vertex_location, start_instance_location);
                }

                void set_primitive_topology(D3D12_PRIMITIVE_TOPOLOGY topology)
                {
                    list()->IASetPrimitiveTopology(topology);
                }

                void set_view_port(const D3D12_VIEWPORT& vp)
                {
                    list()->RSSetViewports(1, &vp);
                }

                void set_scissor_rectangle(const D3D12_RECT& r)
                {
                    list()->RSSetScissorRects(1, &r);
                }

                void set_pso(const graphics_pipeline_state* p )
                {
                    if (m_graphics_pipeline_state != p )
                    {
                        //upload and reset cache for the descriptor tables
                        if (m_graphics_pipeline_state)
                        {
                            if (m_graphics_pipeline_state->m_root_signature != p->m_root_signature)
                            {
                                m_descriptor_handle_cache0.set_root_signature_meta_data(*p->m_root_signature_meta_data);
                                list()->SetGraphicsRootSignature(p->m_root_signature);
                                m_graphics_pipeline_state = p;
                            }
                        }
                        else
                        {
                            m_descriptor_handle_cache0.set_root_signature_meta_data(*p->m_root_signature_meta_data);
                            list()->SetGraphicsRootSignature(p->m_root_signature);
                            m_graphics_pipeline_state = p;
                        }

                        list()->SetPipelineState(p->m_state);
                    }
                }

                void set_descriptor_heaps( uint32_t heap_count, ID3D12DescriptorHeap* heaps[] )
                {
                    list()->SetDescriptorHeaps(heap_count, heaps);
                }

                void set_descriptor_heaps()
                {
                    ID3D12DescriptorHeap* heaps[] = { m_rc->frame_gpu_srv_heap()->resource(), m_rc->frame_gpu_sampler_heap()->resource() };
                    set_descriptor_heaps(sizeof(heaps) / sizeof(heaps[0]), heaps);
                }

                void set_index_buffer( D3D12_INDEX_BUFFER_VIEW view)
                {
                    list()->IASetIndexBuffer(&view);
                }

                void set_vertex_buffer(uint32_t slot, D3D12_VERTEX_BUFFER_VIEW view)
                {
                    list()->IASetVertexBuffers(slot, 1, &view);
                }

                void set_vertex_buffers( uint32_t slot, uint32_t count, const D3D12_VERTEX_BUFFER_VIEW views[])
                {
                    list()->IASetVertexBuffers(slot, count, views);
                }

                void set_graphics_srv_buffer(uint32_t root_index, gpu_virtual_resource* r)
                {
                    list()->SetGraphicsRootShaderResourceView(root_index, r->virtual_address());
                }

                void set_graphics_srv_buffer(uint32_t root_index, nullptr_t )
                {
                    list()->SetGraphicsRootShaderResourceView(root_index, 0);
                }

                void set_graphics_uav_buffer(uint32_t root_index, gpu_virtual_resource* r)
                {
                    list()->SetGraphicsRootUnorderedAccessView(root_index, r->virtual_address());
                }

                void set_graphics_uav_buffer(uint32_t root_index, nullptr_t)
                {
                    list()->SetGraphicsRootUnorderedAccessView(root_index, 0);
                }

                void set_graphics_constant_buffer(uint32_t root_index, D3D12_GPU_VIRTUAL_ADDRESS address)
                {
                    list()->SetGraphicsRootConstantBufferView(root_index, address);
                }

                void set_graphics_constant_buffer(uint32_t root_index, const void* buffer, size_t byte_count )
                {
                    auto allocation = m_upload_allocator.allocate(byte_count, 256);
                    mem_copy(allocation.cpu_address(), buffer, byte_count);
                    list()->SetGraphicsRootConstantBufferView(root_index, allocation.gpu_address());
                }

                template< typename t >
                void set_graphics_constant_buffer(uint32_t root_index, const t* buffer)
                {
                    set_graphics_constant_buffer(root_index, &buffer, sizeof(t));
                }

                template<typename t>
                void set_graphics_constant_buffer(uint32_t root_index, const t& buffer)
                {
                    set_graphics_constant_buffer(root_index, &buffer, sizeof(t));
                }

                void set_graphics_dynamic_constant_buffer(uint32_t root_index, uint32_t offset, const void* buffer, size_t byte_count)
                {
                    auto allocation = m_upload_allocator.allocate(byte_count, 256);
                    mem_copy(allocation.cpu_address(), buffer, byte_count);

                    D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc    = {};
                    cbv_desc.BufferLocation                     = allocation.gpu_address();
                    cbv_desc.SizeInBytes                        = static_cast<uint32_t>(mem::align(byte_count, 256));
                    D3D12_CPU_DESCRIPTOR_HANDLE h2              = m_descriptor_cache.allocate_srv();

                    m_command_manager->device()->CreateConstantBufferView(&cbv_desc, h2);

                    set_graphics_dynamic_descriptor(root_index, h2, offset);
                }

                //use for small buffers, generated every frame
                //todo: check if better to use upload_buffer_handle
                void set_dynamic_vertex_buffer( uint32_t slot, uint32_t stride, gpu_upload_buffer* r )
                {

                    D3D12_VERTEX_BUFFER_VIEW view   = {};
                    view.BufferLocation             = r->virtual_address();
                    view.SizeInBytes = static_cast<uint32_t>(mem::align(r->desc().Width, 256));
                    view.StrideInBytes              = stride;
                    set_vertex_buffer(slot, view);
                }

                //use for small buffers, generated every frame
                void set_dynamic_index_buffer_32( const gpu_upload_buffer* r )
                {
                    D3D12_INDEX_BUFFER_VIEW view = {};
                    view.BufferLocation = r->virtual_address();
                    auto d = r->desc();
                    view.SizeInBytes    = static_cast<uint32_t>(mem::align(d.Width, 256));
                    view.Format         = DXGI_FORMAT_R32_UINT;
                    set_index_buffer(view);
                }

                //use for small buffers, generated every frame
                void set_dynamic_index_buffer_16( const gpu_upload_buffer* r )
                {
                    D3D12_INDEX_BUFFER_VIEW view = {};
                    view.BufferLocation = r->virtual_address();
                    view.SizeInBytes    = static_cast<uint32_t>(mem::align(r->desc().Width, 256));
                    view.Format         = DXGI_FORMAT_R16_UINT;
                    set_index_buffer(view);
                }


                template <typename t>
                void set_graphics_dynamic_constant_buffer(uint32_t root_index, uint32_t offset, const t* buffer)
                {
                    set_graphics_dynamic_constant_buffer(root_index, offset, buffer, sizeof(t));
                }

                template <typename t>
                void set_graphics_dynamic_constant_buffer(uint32_t root_index, uint32_t offset, const t& buffer)
                {
                    set_graphics_dynamic_constant_buffer(root_index, offset, &buffer, sizeof(t));
                }

                void set_graphics_root_descriptor_table(uint32_t root_index, D3D12_GPU_DESCRIPTOR_HANDLE h)
                {
                    list()->SetGraphicsRootDescriptorTable(root_index, h);
                }

                void set_graphics_dynamic_descriptors( uint32_t root_index, const D3D12_CPU_DESCRIPTOR_HANDLE handles[], uint32_t count , uint32_t offset = 0 )
                {
                    m_descriptor_handle_cache0.set_descriptor_handles(root_index, offset, handles, count);
                }

                void set_graphics_dynamic_descriptor( uint32_t root_index, D3D12_CPU_DESCRIPTOR_HANDLE Handle, uint32_t offset = 0 )
                {
                    set_graphics_dynamic_descriptors(root_index, &Handle, 1, offset);
                }

                void set_name(const wchar_t* name)
                {
                    list()->SetName(name);
                }

                private:

                void commit_graphics_root_descriptor_tables()
                {
                    if (m_descriptor_handle_cache0.dirty())
                    {
                        auto size    = m_descriptor_handle_cache0.size();
                        auto handles = this->m_rc->frame_gpu_srv_heap()->allocate_incrementable(size);
                        m_descriptor_handle_cache0.flush(handles, [this](uint32_t root_index, D3D12_GPU_DESCRIPTOR_HANDLE h )
                        {
                            set_graphics_root_descriptor_table(root_index, h);
                        }
                        );
                    }
                }

                void set_render_target(D3D12_CPU_DESCRIPTOR_HANDLE rtv, D3D12_CPU_DESCRIPTOR_HANDLE dsv, bool copy_rtv, bool copy_dsv)
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE h0 = rtv;
                    D3D12_CPU_DESCRIPTOR_HANDLE h1 = dsv;

                    if (copy_rtv)
                    {
                        h0 = m_descriptor_cache.allocate_rtv();
                        m_device->CopyDescriptorsSimple(1, h0, rtv, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                    }

                    if (copy_dsv)
                    {
                        h1 = m_descriptor_cache.allocate_dsv();
                        m_device->CopyDescriptorsSimple(1, h1, dsv, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                    }
                    
                    D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { h0 };
                    list()->OMSetRenderTargets(1, handles, FALSE, &h1);
                }

                void set_render_target_dsv(D3D12_CPU_DESCRIPTOR_HANDLE dsv, bool copy_dsv )
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE h1 = dsv;

                    if (copy_dsv)
                    {
                        h1 = m_descriptor_cache.allocate_dsv();
                        m_device->CopyDescriptorsSimple(1, h1, dsv, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                    }

                    list()->OMSetRenderTargets(0, nullptr, FALSE, &h1);
                }
            };

            inline void graphics_reset_vertex_buffer(gpu_graphics_command_context* ctx, uint32_t slot)
            {
                ctx->set_vertex_buffer(slot, {});
            }

            inline void graphics_reset_index_buffer(gpu_graphics_command_context* ctx)
            {
                ctx->set_index_buffer({});
            }
        }
    }
}