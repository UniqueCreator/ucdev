#include "pch.h"

#include "upload_queue_impl.h"


#include <uc_dev/mem/align.h>
#include <uc_dev/gx/dx12/cmd/memcpy.h>
#include <uc_dev/gx/dx12/cmd/upload_buffer_handle.h>

#include <uc_dev/gx/dx12/gpu/descriptions.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/gx/dx12/gpu/map_guard.h>
#include <uc_dev/gx/dx12/gpu/descriptions.h>
#include <uc_dev/gx/dx12/api/resource_utils.h>
#include <uc_dev/gx/dx12/gpu/texture_2d.h>
#include <uc_dev/gx/dx12/gpu/texture_2d_array.h>

#include <iterator>
#include <ppltasks.h>
#include <ppl.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace details
            {
                
                static inline upload_buffer_handle create_upload_buffer(gpu_resource_create_context* rc, uint64_t size)
                {
                    return new ref_counted_upload_buffer(rc->create_upload_buffer_resource(size).Get());
                }

                static inline upload_buffer_handle create_upload_buffer(ID3D12Device* d, uint64_t size)
                {
                    auto desc = describe_buffer(size);
                    auto heap = upload_properties();
                    auto r = create_committed_resource(d, &heap, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
                    return new ref_counted_upload_buffer(r.Get());
                }

                template <typename it, typename t> static void apply_visistor(it b, it e, t f)
                {
                    std::for_each(b, e, [&f](const auto& v)
                    {
                        boost::apply_visitor(f, v);
                    });
                }

                static inline void* offset_pointer(void* p, uint32_t offset)
                {
                    return reinterpret_cast<void*> (reinterpret_cast<intptr_t>(p) + offset);
                }

                static inline void* offset_pointer(void* p, uint64_t offset)
                {
                    return reinterpret_cast<void*> (reinterpret_cast<intptr_t>(p) + offset);
                }
            }

            gpu_upload_queue_impl::gpu_upload_queue_impl(ID3D12Device* d, gpu_resource_create_context* rc, gpu_command_context_allocator* allocator, gpu_command_queue* q):
                m_d(d)
                , m_rc(rc)
                , m_allocator(allocator)
                , m_queue(q)
                , m_context( create_command_context( allocator ) )
            {

            }

            gpu_upload_queue_impl::~gpu_upload_queue_impl()
            {
                flush();
            }

            void gpu_upload_queue_impl::sync()
            {
                std::lock_guard<std::mutex> lock(m_executing_tasks_lock);

                m_buffer_index = ( m_buffer_index + 1 ) % 3;

                auto& v = m_executing_tasks[m_buffer_index];

                for (auto& i : v)
                {
                    if (i.m_value != 0)
                    {
                        if (i.m_queue->is_fence_complete(i.m_value))
                        {
                            i.m_queue->wait_for_fence(i.m_value);
                        }
                    }
                }

                m_executing_tasks[m_buffer_index].resize(0);
            }

            //this must be flushed once per frame before the rendering
            gpu_fence gpu_upload_queue_impl::flush()
            {
                std::lock_guard<std::mutex> lock(m_executing_tasks_lock);

                //submit tasks so far
                m_executing_tasks[m_buffer_index].push_back(submit());
                {
                    for ( auto index = 2; index < 3; ++index )
                    {
                        auto iv = (index + m_buffer_index) % 3;
                        auto& v = m_executing_tasks[ iv ];

                        for ( auto& i : v )
                        {
                            if (i.m_value != 0)
                            {
                                if ( i.m_queue->is_fence_complete( i.m_value) )
                                {
                                    //i.m_resources.clear();
                                }
                            }
                        }
                    }
                }

                return m_queue->signal_fence();
            }

            gpu_upload_queue_impl::task_result gpu_upload_queue_impl::submit()
            {
                concurrency::concurrent_vector<task_handle> r;
                r.swap(m_tasks);

                task_result d; 

                if (!r.empty())
                {
                    managed_gpu_command_context ctx = create_command_context(m_allocator);
                    gpu_command_context* c = ctx.get();

                    details::apply_visistor(r.begin(), r.end(), [c](const auto& v)
                    {
                        upload_queue::begin_upload(c, v);
                    });

                    ctx->flush_resource_barriers();

                    details::apply_visistor(r.begin(), r.end(), [c](const auto& v)
                    {
                        upload_queue::execute_upload(c, v);
                    });

                    details::apply_visistor(r.begin(), r.end(), [c](const auto& v)
                    {
                        upload_queue::end_upload(c, v);
                    });

                    ctx->flush_resource_barriers();

                    std::vector< upload_buffer_handle > submitted_resources;
                    submitted_resources.reserve(r.size());

                    for (auto&& i : r)
                    {
                        boost::apply_visitor( [&submitted_resources] (auto& v)
                        {
                            submitted_resources.push_back(v.m_upload_buffer);
                        }, i);
                    }

                    auto s = ctx->submit(gpu_command_context::flush_operation::do_not_wait_to_execute);

                    task_result d0 = { std::move(s.m_value), m_queue ,std::move(submitted_resources) };

                    return d0;
                }
                else
                {
                    return d;
                }
            }

            void gpu_upload_queue_impl::copy_buffer( gpu_upload_buffer* b, const void* initial_data, uint64_t size)
            {
                gpu_map_guard< gpu_upload_buffer> guard(b);
                mem_copy(guard.cpu_address(), initial_data, size);
            }

            void gpu_upload_queue_impl::copy_buffer(gpu_upload_buffer* b, const void* initial_data, uint64_t size, uint64_t offset )
            {
                gpu_map_guard< gpu_upload_buffer> guard(b);
                auto destination = reinterpret_cast<uint64_t> (guard.cpu_address()) + offset;
                mem_copy(reinterpret_cast<void*> ( destination ), initial_data, size);
            }

            upload_buffer_handle gpu_upload_queue_impl::create_upload_buffer(uint64_t size)
            {
                if (size < 65536 )
                {
                    return details::create_upload_buffer(m_rc, size);
                }
                else
                {
                    return details::create_upload_buffer(m_d, size);
                }
            }

            void gpu_upload_queue_impl::upload_buffer( gpu_virtual_resource* destination, const void* initial_data, uint64_t size, uint64_t offset )
            {
                upload_buffer_handle buffer = create_upload_buffer( size );
                copy_buffer(buffer.get(), initial_data, size);
                upload_buffer(destination, std::move(buffer), offset, size);
            }

            gpu_upload_buffer* gpu_upload_queue_impl::upload_buffer(const void* initial_data, uint64_t size)
            {
                upload_buffer_handle buffer = create_upload_buffer(mem::align(size, 256));
                copy_buffer( buffer.get(), initial_data, size);

                upload_queue::upload_buffer_resource_task r = { std::move(buffer) };

                auto result = r.m_upload_buffer.get();
                m_tasks.push_back(std::move(r));

                return  result;
            }

            void gpu_upload_queue_impl::upload_buffer_gather( gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, uint64_t offset )
            {
                //some gpus have troubles if index buffers are not aligned on a boundary. that is why the aligned sizes
                upload_buffer_handle buffer = create_upload_buffer( size0_aligned + size1_aligned );
                copy_buffer(buffer.get(), initial_data0, size0);
                copy_buffer(buffer.get(), initial_data1, size1, size0_aligned);
                upload_buffer(destination, std::move(buffer), offset);
            }

            void gpu_upload_queue_impl::upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1,  const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, uint64_t offset )
            {
                //some gpus have troubles if index buffers are not aligned on a boundary. that is why the aligned sizes
                upload_buffer_handle buffer = create_upload_buffer(size0_aligned + size1_aligned + size2_aligned );

                copy_buffer(buffer.get(), initial_data0, size0);
                copy_buffer(buffer.get(), initial_data1, size1, size0_aligned);
                copy_buffer(buffer.get(), initial_data2, size2, size0_aligned + size1_aligned);

                upload_buffer(destination, std::move(buffer), offset);
            }

            void gpu_upload_queue_impl::upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, const void* __restrict initial_data3, uint64_t size3_aligned, uint64_t size3, const void* __restrict initial_data4, uint64_t size4_aligned, uint64_t size4, uint64_t offset)
            {
                //some gpus have troubles if index buffers are not aligned on a boundary. that is why the aligned sizes
                upload_buffer_handle buffer = create_upload_buffer(size0_aligned + size1_aligned + size2_aligned + size3_aligned + size4_aligned);

                copy_buffer(buffer.get(), initial_data0, size0);
                copy_buffer(buffer.get(), initial_data1, size1, size0_aligned);
                copy_buffer(buffer.get(), initial_data2, size2, size0_aligned + size1_aligned);
                copy_buffer(buffer.get(), initial_data3, size3, size0_aligned + size1_aligned + size2_aligned);
                copy_buffer(buffer.get(), initial_data4, size4, size0_aligned + size1_aligned + size2_aligned+ size3_aligned);

                upload_buffer(destination, std::move(buffer), offset);
            }

            void gpu_upload_queue_impl::upload_buffer( gpu_virtual_resource* destination, upload_buffer_handle&& source, uint64_t offset, uint64_t size)
            {
                if ( offset > 0 )
                {
                    auto  desc = source->desc();
                    auto  info = m_d->GetResourceAllocationInfo(1, 1, &desc);

                    upload_queue::copy_buffer_region_task r = {};
                        
                    r.m_source_offset = 0;
                    r.m_destination_offset = offset;
                    r.m_destination = destination;
                    r.m_upload_buffer = std::move(source);
                    r.m_byte_count = size == 0 ? info.SizeInBytes : size;

                    m_tasks.push_back(std::move(r));
                }
                else
                {
                    upload_queue::copy_resource_task r = { destination, std::move(source) };
                    m_tasks.push_back(std::move(r));
                }
            }

            void gpu_upload_queue_impl::upload_texture_2d(gpu_texture_2d* r, uint32_t first_sub_resource, uint32_t sub_resource_count, D3D12_SUBRESOURCE_DATA sub_resource_data[] )
            {
                uint64_t upload_size = 0;

                auto sb = sub_resource_count - first_sub_resource;

                std::vector<uint32_t>                               row_count(sb);
                std::vector<uint64_t>                               row_sizes_in_bytes(sb);
                std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>     layouts(sb);
                auto d = r->desc();
                m_d->GetCopyableFootprints(&d, first_sub_resource, sub_resource_count, 0, &layouts[0], &row_count[0], &row_sizes_in_bytes[0], &upload_size);

                upload_buffer_handle buffer = create_upload_buffer(upload_size);
                gpu_map_guard< gpu_upload_buffer> guard(buffer.get());

                for (auto i = 0U; i < sub_resource_count; ++i)
                {
                    D3D12_MEMCPY_DEST dest = { details::offset_pointer( guard.cpu_address(), layouts[i].Offset ) , layouts[i].Footprint.RowPitch, layouts[i].Footprint.RowPitch * row_count[i] };
                    dx12::memcpy_subresource(&dest, &sub_resource_data[i], row_sizes_in_bytes[i], row_count[i], layouts[i].Footprint.Depth);
                }

                {
                    upload_queue::copy_texture_region_task task;
                    task.m_destination = r;
                    task.m_upload_buffer = std::move(buffer);
                    task.m_d = this->m_d;
                    task.m_first_sub_resource = first_sub_resource;
                    task.m_sub_resource_count = sub_resource_count;
                    task.m_base_offset = 0;
                    m_tasks.push_back(std::move(task));
                }
            }

            void gpu_upload_queue_impl::upload_texture_2d_array(gpu_texture_2d_array* r, uint32_t first_slice, uint32_t slice_count, D3D12_SUBRESOURCE_DATA slice_data[])
            {
                uint64_t upload_size = 0;

                auto sb = slice_count - first_slice;

                std::vector<uint32_t>                               row_count(sb);
                std::vector<uint64_t>                               row_sizes_in_bytes(sb);
                std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>     layouts(sb);
                auto d = r->desc();
                m_d->GetCopyableFootprints(&d, first_slice, slice_count, 0, &layouts[0], &row_count[0], &row_sizes_in_bytes[0], &upload_size);

                upload_buffer_handle buffer = create_upload_buffer(upload_size);
                gpu_map_guard< gpu_upload_buffer> guard(buffer.get());

                for (auto i = 0U; i < slice_count; ++i)
                {
                    D3D12_MEMCPY_DEST dest = { details::offset_pointer(guard.cpu_address(), layouts[i].Offset) , layouts[i].Footprint.RowPitch, layouts[i].Footprint.RowPitch * row_count[i] };
                    dx12::memcpy_subresource(&dest, &slice_data[i], row_sizes_in_bytes[i], row_count[i], layouts[i].Footprint.Depth);
                }

                {
                    upload_queue::copy_texture_region_task task;
                    task.m_destination = r;
                    task.m_upload_buffer = std::move(buffer);
                    task.m_d = this->m_d;
                    task.m_first_sub_resource = first_slice;
                    task.m_sub_resource_count = slice_count;
                    task.m_base_offset = 0;
                    m_tasks.push_back(std::move(task));
                }
            }
        }
    }
}

