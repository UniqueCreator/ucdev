#pragma once


#include <uc_dev/gx/dx12/cmd/base_command_context.h>
#include <uc_dev/gx/dx12/cmd/memcpy.h>
#include <pix3.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct fill_value
            {
                fill_value(float f) : m_f(f)
                {

                }

                fill_value(uint32_t u) : m_u(u)
                {

                }

                fill_value(int32_t i) : m_i(i)
                {

                }

                static fill_value zero()
                {
                    return fill_value(0U);
                }

                static fill_value one()
                {
                    return fill_value(1.0f);
                }

                union
                {
                    float    m_f;
                    uint32_t m_u;
                    int32_t  m_i;
                };
            };

            struct copy_sub_resource : D3D12_TEXTURE_COPY_LOCATION
            {
                using base           = D3D12_TEXTURE_COPY_LOCATION;
                copy_sub_resource( gpu_virtual_resource* r, uint32_t sub_resource_index)
                {
                    base::pResource = r->resource();
                    base::Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                    base::SubresourceIndex = sub_resource_index;
                }
            };

            struct copy_foot_print : D3D12_TEXTURE_COPY_LOCATION
            {
                using base = D3D12_TEXTURE_COPY_LOCATION;

                copy_foot_print(gpu_virtual_resource* r, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& foot_print )
                {
                    base::pResource = r->resource();
                    base::Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                    base::PlacedFootprint = foot_print;
                }
            };

            struct gpu_command_context : public gpu_base_command_context
            {
                using base = gpu_base_command_context;

                public:

                enum flush_operation
                {
                    wait_to_execute,
                    do_not_wait_to_execute
                };


                gpu_fence submit(flush_operation op = flush_operation::do_not_wait_to_execute)
                {
                    auto fence = finish();

                    if (op == wait_to_execute)
                    {
                        m_command_queue->wait_for_fence(fence.m_value);
                    }

                    return fence;
                }

                gpu_fence finish()
                {
                    flush_resource_barriers();

                    throw_if_failed(list()->Close());

                    auto v = m_command_queue->execute_command_list(m_command_list.m_list.Get());
                    m_command_manager->free_command_list(v.m_value, m_command_list);
                    m_command_list.reset();

                    return v;
                }

                void flush_resource_barriers()
                {
                    if (m_resource_bariers_count > 0)
                    {
                        list()->ResourceBarrier(m_resource_bariers_count, &m_resource_bariers[0]);
                        m_resource_bariers_count = 0;
                    }
                }

                void transition_resource(const gpu_virtual_resource* r, D3D12_RESOURCE_STATES old_state, D3D12_RESOURCE_STATES new_state)
                {
                    //todo: check the same states, before flush

                    if (m_resource_bariers_count == barrier_count)
                    {
                        flush_resource_barriers();
                    }

                    D3D12_RESOURCE_BARRIER  d = {};

                    d.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                    d.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                    d.Transition.pResource = r->resource();
                    d.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                    d.Transition.StateBefore = old_state;
                    d.Transition.StateAfter = new_state;

                    bool add = true;

                    for (int32_t i = m_resource_bariers_count - 1; i >= 0; --i)
                    {
                        if ( m_resource_bariers[i].Transition.pResource == d.Transition.pResource )
                        {
                            if (memcmp(&m_resource_bariers[i],&d, sizeof(d)) == 0)
                            {
                                add = false;
                                break;
                            }
                        }
                    }

                    if (add)
                    {
                        D3D12_RESOURCE_BARRIER& desc = m_resource_bariers[m_resource_bariers_count++];
                        desc = d;
                    }
                }

                void copy_buffer_region( gpu_virtual_resource* destination, uint64_t destination_offset, gpu_virtual_resource* source, uint64_t source_offset, uint64_t byte_count)
                {
                    flush_resource_barriers();
                    list()->CopyBufferRegion(destination->resource(), destination_offset, source->resource(), source_offset, byte_count );
                }

                void copy_resource( gpu_virtual_resource* destination, gpu_virtual_resource* source )
                {
                    flush_resource_barriers();
                    list()->CopyResource(destination->resource(), source->resource());
                }

                void copy_texture_region(const copy_sub_resource& destination, const copy_foot_print& source)
                {
                    flush_resource_barriers();
                    list()->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);
                }

                void upload_buffer( gpu_virtual_resource* destination, size_t destination_offset, const void* buffer, size_t byte_count )
                {
                    auto allocation = m_upload_allocator.allocate(byte_count, 512);
                    mem_copy(allocation.cpu_address(), buffer, byte_count);
                    copy_buffer_region(destination, destination_offset, allocation.resource(), allocation.offset(), byte_count);
                }

                gpu_upload_allocation upload_buffer(const void* buffer, size_t byte_count)
                {
                    auto allocation = m_upload_allocator.allocate(byte_count, 512);
                    mem_copy(allocation.cpu_address(), buffer, byte_count);
                    return allocation;
                }

                void pix_begin_event(const wchar_t* label)
                {
                    PIXBeginEvent(list(), 0, label);
                }

                void pix_end_event(void)
                {
                    PIXEndEvent(list());
                }

                void pix_set_marker(const wchar_t* label)
                {
                    PIXSetMarker(list(), 0, label);
                }

                descriptor_handle srv()
                {
                    return m_rc->frame_cpu_srv_heap()->allocate();
                }

                descriptor_handle uav()
                {
                    return m_rc->frame_cpu_srv_heap()->allocate();
                }

                descriptor_handle cbv()
                {
                    return m_rc->frame_cpu_srv_heap()->allocate();
                }

                protected:
                ID3D12GraphicsCommandList*  list()
                {
                    return m_command_list.m_list.Get();
                }
            };
        }
    }
}