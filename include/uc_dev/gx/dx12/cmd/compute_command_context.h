#pragma once

#include <uc_dev/gx/dx12/gpu/pipeline_state.h>
#include "command_context.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct gpu_compute_command_context : public gpu_command_context
            {
                using base = gpu_command_context;

                public:

                void dispatch( uint32_t thread_group_count_x, uint32_t thread_group_count_y = 1, uint32_t thread_group_count_z = 1)
                {
                    flush_resource_barriers();
                    commit_compute_root_descriptor_tables();
                    list()->Dispatch(thread_group_count_x, thread_group_count_y, thread_group_count_z);
                }

                void set_pso(const compute_pipeline_state* p )
                {
                    if (m_compute_pipeline_state != p)
                    {
                        //upload and reset cache for the descriptor tables
                        if (m_compute_pipeline_state)
                        {
                            if (m_compute_pipeline_state->m_root_signature != p->m_root_signature)
                            {
                                m_descriptor_handle_cache0.set_root_signature_meta_data(*p->m_root_signature_meta_data);
                                list()->SetComputeRootSignature(p->m_root_signature);
                                m_compute_pipeline_state = p;
                            }
                        }
                        else
                        {
                            m_descriptor_handle_cache0.set_root_signature_meta_data(*p->m_root_signature_meta_data);
                            list()->SetComputeRootSignature(p->m_root_signature);
                            m_compute_pipeline_state = p;
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

                void set_compute_srv_buffer(uint32_t root_index, gpu_virtual_resource* r)
                {
                    list()->SetComputeRootShaderResourceView(root_index, r->virtual_address());
                }

                void set_compute_srv_buffer(uint32_t root_index, nullptr_t )
                {
                    list()->SetComputeRootShaderResourceView(root_index, 0);
                }

                void set_compute_uav_buffer(uint32_t root_index, gpu_virtual_resource* r)
                {
                    list()->SetComputeRootUnorderedAccessView(root_index, r->virtual_address());
                }

                void set_compute_uav_buffer(uint32_t root_index, nullptr_t)
                {
                    list()->SetComputeRootUnorderedAccessView(root_index, 0);
                }

                void set_compute_constant_buffer(uint32_t root_index, D3D12_GPU_VIRTUAL_ADDRESS address)
                {
                    list()->SetComputeRootConstantBufferView(root_index, address);
                }

                void set_compute_constant_buffer(uint32_t root_index, const void* buffer, size_t byte_count )
                {
                    auto allocation = m_upload_allocator.allocate(byte_count, 256);
                    mem_copy(allocation.cpu_address(), buffer, byte_count);
                    list()->SetComputeRootConstantBufferView(root_index, allocation.gpu_address());
                }

                template <typename t>
                void set_compute_constant_buffer(uint32_t root_index, const t* buffer)
                {
                    set_compute_constant_buffer(root_index, buffer, sizeof(t));
                }

                void set_compute_dynamic_constant_buffer(uint32_t root_index, uint32_t offset, const void* buffer, size_t byte_count)
                {
                    auto allocation = m_upload_allocator.allocate(byte_count, 256);
                    mem_copy(allocation.cpu_address(), buffer, byte_count);

                    D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc    = {};
                    cbv_desc.BufferLocation                     = allocation.gpu_address();
                    cbv_desc.SizeInBytes                        = static_cast<uint32_t>(mem::align(byte_count, 256));
                    D3D12_CPU_DESCRIPTOR_HANDLE h2              = m_descriptor_cache.allocate_srv();

                    m_command_manager->device()->CreateConstantBufferView(&cbv_desc, h2);

                    set_compute_dynamic_descriptor(root_index, h2, offset);
                }

                template <typename t>
                void set_compute_dynamic_constant_buffer(uint32_t root_index, uint32_t offset, const t* buffer)
                {
                    set_dynamic_constant_buffer(root_index, offset, buffer, sizeof(t));
                }

                void set_compute_root_descriptor_table(uint32_t root_index, D3D12_GPU_DESCRIPTOR_HANDLE h)
                {
                    list()->SetComputeRootDescriptorTable(root_index, h);
                }

                void set_compute_dynamic_descriptors( uint32_t root_index, const D3D12_CPU_DESCRIPTOR_HANDLE handles[], uint32_t count , uint32_t offset = 0 )
                {
                    m_descriptor_handle_cache0.set_descriptor_handles(root_index, offset, handles, count);
                }

                void set_compute_dynamic_descriptor( uint32_t root_index, D3D12_CPU_DESCRIPTOR_HANDLE Handle, uint32_t offset = 0 )
                {
                    set_compute_dynamic_descriptors(root_index, &Handle, 1, offset);
                }

                private:

                void commit_compute_root_descriptor_tables()
                {
                    if (m_descriptor_handle_cache0.dirty())
                    {
                        auto size    = m_descriptor_handle_cache0.size();
                        auto handles = this->m_rc->frame_gpu_srv_heap()->allocate_incrementable(size);
                        m_descriptor_handle_cache0.flush(handles, [this](uint32_t root_index, D3D12_GPU_DESCRIPTOR_HANDLE h )
                        {
                            set_compute_root_descriptor_table(root_index, h);
                        }
                        );
                    }
                }
            };
        }
    }
}