#pragma once

#include <d3d12.h>

#include <uc_dev/util/noncopyable.h>
#include <uc_dev/mem/ref_counter.h>
#include <uc_dev/mem/intrusive_ptr.h>
#include <uc_dev/util/pimpl.h>

#include <uc_dev/gx/dx12/gpu/fence_value.h>

#include "upload_buffer_handle.h"


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_command_context_allocator;
            class gpu_command_queue;
            class gpu_resource_create_context;
            class gpu_virtual_resource;
            class gpu_texture_2d;
            class gpu_texture_2d_array;

            class gpu_upload_queue_impl;

            class gpu_upload_queue : private util::noncopyable
            {
            public:

                gpu_upload_queue(ID3D12Device* d, gpu_resource_create_context* rc, gpu_command_context_allocator* allocator, gpu_command_queue* queue);
                ~gpu_upload_queue();

                gpu_fence   flush();

                void upload_buffer(gpu_virtual_resource* destination, const void* initial_data, uint64_t size, uint64_t offset = 0);
                void upload_buffer(gpu_virtual_resource* destination, upload_buffer_handle&& source, uint64_t offset = 0);
                void upload_texture_2d(gpu_texture_2d* r, uint32_t first_sub_resource, uint32_t sub_resource_count, D3D12_SUBRESOURCE_DATA sub_resource_data[]);
                void upload_texture_2d_array(gpu_texture_2d_array* r, uint32_t first_slice, uint32_t slice_count, D3D12_SUBRESOURCE_DATA slice_data[]);

                void upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, uint64_t offset = 0);
                void upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, uint64_t offset = 0);
                void upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, const void* __restrict initial_data3, uint64_t size3_aligned, uint64_t size3, const void* __restrict initial_data4, uint64_t size4_aligned, uint64_t size4, uint64_t offset = 0);

                gpu_upload_buffer* upload_buffer(const void* initial_data, uint64_t size);

                void sync();

                util::details::pimpl<gpu_upload_queue_impl> m_impl;
            };

            //private data, todo: fix visual assist warning
            extern template util::details::pimpl<gpu_upload_queue_impl>;
        }
    }
}


