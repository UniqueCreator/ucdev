#pragma once

#include <d3d12.h>

#include <atomic>
#include <mutex>
#include <thread>
#include <ppltasks.h>
#include <concurrent_vector.h>
#include <tuple>

#include <boost/variant.hpp>

#include <uc_dev/gx/dx12/cmd/upload_buffer_handle.h>
#include <uc_dev/gx/dx12/gpu/upload_buffer.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/gx/dx12/cmd/command_queue.h>
#include <uc_dev/gx/dx12/cmd/command_context_allocator.h>
#include <uc_dev/gx/dx12/cmd/command_context_utils.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace upload_queue
            {
                struct copy_resource_task
                {
                    gpu_virtual_resource*                          m_destination;
                    upload_buffer_handle                           m_upload_buffer;
                };

                struct copy_buffer_region_task : public copy_resource_task
                {
                    uint64_t	m_source_offset;
                    uint64_t	m_destination_offset;
                    uint64_t	m_byte_count;
                };

                struct copy_texture_region_task : public copy_resource_task
                {
                    uint32_t      m_first_sub_resource;
                    uint32_t      m_sub_resource_count;
                    uint64_t      m_base_offset;
                    ID3D12Device* m_d;
                };

                //source and destination point to the same upload buffer memory region
                //useful for small index or geometry buffers
                struct upload_buffer_resource_task
                {
                    upload_buffer_handle    m_upload_buffer;
                };

                template <typename t> inline void begin_upload( gpu_command_context* ctx, const t& command )
                {
                    ctx->transition_resource( command.m_destination, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST );
                }

                template <> inline void begin_upload<upload_buffer_resource_task>(gpu_command_context*, const upload_buffer_resource_task&)
                {
                }

                template <typename t> inline void execute_upload(gpu_command_context* ctx, const t& command);

                template <> inline void execute_upload< copy_resource_task>(gpu_command_context* ctx, const copy_resource_task& command)
                {
                    auto d = command.m_destination->desc();
                    auto s = command.m_upload_buffer->desc();
                    assert(memcmp(&d, &s, sizeof(d)) == 0);
                    ctx->copy_resource(command.m_destination, command.m_upload_buffer.get());
                }

                template<> inline void execute_upload< copy_buffer_region_task>( gpu_command_context* ctx, const copy_buffer_region_task& command )
                {
                    ctx->copy_buffer_region(command.m_destination, command.m_destination_offset, command.m_upload_buffer.get(), command.m_source_offset, command.m_byte_count);
                }

                template<> inline void execute_upload< upload_buffer_resource_task>(gpu_command_context*, const upload_buffer_resource_task&)
                {
                    
                }

                template <> inline void execute_upload< copy_texture_region_task>( gpu_command_context* ctx, const copy_texture_region_task& command )
                {
                    auto first_sub_resource = command.m_first_sub_resource;
                    auto sub_resource_count = command.m_sub_resource_count;

                    auto sb = sub_resource_count - first_sub_resource;

                    std::vector<uint32_t>                               row_count(sb);
                    std::vector<uint64_t>                               row_sizes_in_bytes(sb);
                    std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT>     layouts(sb);

                    {
                        auto d = command.m_destination->desc();
                        command.m_d->GetCopyableFootprints(&d, static_cast<uint32_t>(first_sub_resource), static_cast<uint32_t>(sub_resource_count), command.m_base_offset, &layouts[0], &row_count[0], &row_sizes_in_bytes[0], nullptr);
                    }

                    auto&& d = command.m_destination;
                    auto&& s = command.m_upload_buffer.get();

                    for (auto i = 0U; i < sub_resource_count; ++i)
                    {
                        ctx->copy_texture_region(dx12::copy_sub_resource(d, i + first_sub_resource), dx12::copy_foot_print(s, layouts[i]));
                    }
                }

                template <typename t> inline void end_upload(gpu_command_context* ctx, const t& command)
                {
                    ctx->transition_resource(command.m_destination, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);
                }

                template<> inline void end_upload< upload_buffer_resource_task>(gpu_command_context*, const upload_buffer_resource_task&)
                {

                }
            }

            //holds resources and uploads them to the gpu in batches
            class gpu_upload_queue_impl : public util::noncopyable
            {
                using task = boost::variant< upload_queue::copy_resource_task, upload_queue::copy_buffer_region_task, upload_queue::copy_texture_region_task, upload_queue::upload_buffer_resource_task> ;

                public:
                gpu_upload_queue_impl() = default;
                gpu_upload_queue_impl( ID3D12Device* d, gpu_resource_create_context* rc, gpu_command_context_allocator* allocator, gpu_command_queue* queue);
                ~gpu_upload_queue_impl();

                gpu_fence   flush();
                                
                void upload_buffer(gpu_virtual_resource* destination, const void* initial_data, uint64_t size, uint64_t offset = 0 );
                void upload_buffer(gpu_virtual_resource* destination, upload_buffer_handle&& source, uint64_t offset = 0, uint64_t size = 0 );
                void upload_texture_2d(gpu_texture_2d* r, uint32_t first_sub_resource, uint32_t sub_resource_count, D3D12_SUBRESOURCE_DATA sub_resource_data[] );
                void upload_texture_2d_array(gpu_texture_2d_array* r, uint32_t first_slice, uint32_t slice_count, D3D12_SUBRESOURCE_DATA slice_data[]);

                void upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, uint64_t offset = 0 );
                void upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, uint64_t offset = 0);
                void upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, const void* __restrict initial_data3, uint64_t size3_aligned, uint64_t size3, const void* __restrict initial_data4, uint64_t size4_aligned, uint64_t size4, uint64_t offset = 0);

                gpu_upload_buffer* upload_buffer( const void* initial_data, uint64_t size);

                void sync();
                                
                protected:

                struct task_result
                {
                    gpu_fence_value                                m_value = 0;
                    gpu_command_queue*                             m_queue = nullptr;
                    std::vector< upload_buffer_handle >            m_resources;
                };

                using executing_task                               = concurrency::task< task_result >;

                ID3D12Device*                                      m_d;
                gpu_resource_create_context*                       m_rc;
                gpu_command_context_allocator*                     m_allocator;
                gpu_command_queue*                                 m_queue;

                managed_gpu_command_context                        m_context;
                uint64_t                                           m_buffer_index = 0;

                using task_handle = task; 
                concurrency::concurrent_vector < task_handle >      m_tasks;
                std::mutex                                          m_executing_tasks_lock;
                std::vector<task_result>                            m_executing_tasks[3];

                void copy_buffer(gpu_upload_buffer* b, const void* initial_data, uint64_t size);
                void copy_buffer(gpu_upload_buffer* b, const void* initial_data, uint64_t size, uint64_t offset);

                upload_buffer_handle create_upload_buffer(uint64_t size);
                task_result  submit();
            };
        }
    }
}
