#include "pch.h"

#include <uc_dev/gx/dx12/cmd/upload_queue.h>
#include <uc_dev/util/pimpl_impl.h>

#include "upload_queue_impl.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            template uc::util::details::pimpl<gpu_upload_queue_impl>;

            gpu_upload_queue::gpu_upload_queue(ID3D12Device* d
                , gpu_resource_create_context* rc
                , gpu_command_context_allocator* allocator
                , gpu_command_queue* queue) : m_impl(d,rc, allocator, queue)
            {

            }

            gpu_upload_queue::~gpu_upload_queue() = default;

            gpu_fence   gpu_upload_queue::flush()
            {
                return m_impl->flush();
            }

            void gpu_upload_queue::upload_buffer(gpu_virtual_resource* destination, const void* initial_data, uint64_t size, uint64_t offset)
            {
                m_impl->upload_buffer(destination, initial_data, size, offset);
            }

            void gpu_upload_queue::upload_buffer(gpu_virtual_resource* destination, upload_buffer_handle&& source, uint64_t offset)
            {
                m_impl->upload_buffer(destination, std::move(source), offset);
            }

            void gpu_upload_queue::upload_texture_2d(gpu_texture_2d* r, uint32_t first_sub_resource, uint32_t sub_resource_count, D3D12_SUBRESOURCE_DATA sub_resource_data[])
            {
                m_impl->upload_texture_2d(r, first_sub_resource, sub_resource_count, sub_resource_data);
            }

            void gpu_upload_queue::upload_texture_2d_array(gpu_texture_2d_array* r, uint32_t first_slice, uint32_t slice_count, D3D12_SUBRESOURCE_DATA slice_data[])
            {
                m_impl->upload_texture_2d_array(r, first_slice, slice_count, slice_data );
            }

            void gpu_upload_queue::upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, uint64_t offset)
            {
                m_impl->upload_buffer_gather(destination, initial_data0, size0_aligned, size0, initial_data1, size1_aligned, size1, offset);
            }

            void gpu_upload_queue::upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, uint64_t offset)
            {
                m_impl->upload_buffer_gather(destination, initial_data0, size0_aligned, size0, initial_data1, size1_aligned, size1, initial_data2, size2_aligned, size2, offset);
            }

            void gpu_upload_queue::upload_buffer_gather(gpu_virtual_resource* destination, const void* __restrict initial_data0, uint64_t size0_aligned, uint64_t size0, const void* __restrict initial_data1, uint64_t size1_aligned, uint64_t size1, const void* __restrict initial_data2, uint64_t size2_aligned, uint64_t size2, const void* __restrict initial_data3, uint64_t size3_aligned, uint64_t size3, const void* __restrict initial_data4, uint64_t size4_aligned, uint64_t size4, uint64_t offset)
            {
                m_impl->upload_buffer_gather(destination, initial_data0, size0_aligned, size0, initial_data1, size1_aligned, size1, initial_data2, size2_aligned, size2, initial_data3, size3_aligned, size3, initial_data4, size4_aligned,size4, offset);
            }

            gpu_upload_buffer* gpu_upload_queue::upload_buffer(const void* initial_data, uint64_t size)
            {
                return m_impl->upload_buffer(initial_data, size);
            }

            void gpu_upload_queue::sync()
            {
                m_impl->sync();
            }
        }
    }
}


