#pragma once

#include <uc_dev/util/pimpl.h>
#include <uc_dev/util/noncopyable.h>
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_back_buffer;
            class gpu_buffer;

            class gpu_frame_color_buffer;
            class gpu_frame_depth_buffer;
            class gpu_frame_msaa_depth_buffer;

            class gpu_read_back_buffer;
            class gpu_texture_2d;
            class gpu_texture_2d_array;
            class gpu_read_write_texture_2d;
            class gpu_upload_buffer;

            class gpu_view_color_buffer;
            class gpu_view_depth_buffer;
            class gpu_view_msaa_depth_buffer;

            class gpu_resource_create_context final : private util::noncopyable
            {
                public:

                class gpu_resource_create_context_impl;

                gpu_resource_create_context(ID3D12Device* device);

                //Read only assets
                gpu_texture_2d*            create_texture_2d(uint32_t width, uint32_t height, DXGI_FORMAT format);
                gpu_texture_2d*            create_texture_2d(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t mip_count);
                gpu_read_write_texture_2d* create_read_write_texture_2d(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t mip_count);
                gpu_texture_2d_array*      create_texture_2d_array(uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t elements);

                void                       free_texture_2d( gpu_texture_2d* texture );
                void                       free_texture_2d_array(gpu_texture_2d_array* texture);
                void                       free_read_write_texture_2d(gpu_read_write_texture_2d* texture);

                //Transfer accross the pci bus
                gpu_upload_buffer*         create_upload_buffer(uint64_t size);
                gpu_read_back_buffer*      create_read_back_buffer(uint64_t size);

                //Render Targets
                gpu_frame_color_buffer*    create_frame_color_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initial_state = D3D12_RESOURCE_STATE_COMMON);
                gpu_view_color_buffer*     create_view_color_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initial_state = D3D12_RESOURCE_STATE_COMMON);

                //Depth Buffers
                gpu_frame_depth_buffer*         create_frame_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value = 1.0f, uint8_t stencil = 0 );
                gpu_frame_msaa_depth_buffer*    create_frame_msaa_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value = 1.0f, uint8_t stencil = 0);
                
                gpu_view_depth_buffer*          create_view_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value = 1.0f, uint8_t stencil = 0);
                gpu_view_msaa_depth_buffer*     create_view_msaa_depth_buffer(uint32_t width, uint32_t height, DXGI_FORMAT format, float clear_value = 1.0f, uint8_t stencil = 0);

                gpu_back_buffer*                create_back_buffer(ID3D12Resource* r);

                gpu_buffer*                create_buffer(uint32_t elements, uint32_t element_size = 1);
                void                       free_buffer(gpu_buffer* buffer);

                //called on present to swap the allocators
                void sync();

                void reset_view_dependent_resources();

                gpu_srv_descriptor_heap* frame_gpu_srv_heap();
                cpu_srv_descriptor_heap* frame_cpu_srv_heap();
                gpu_rtv_descriptor_heap* frame_rtv_heap();
                gpu_dsv_descriptor_heap* frame_dsv_heap();
                gpu_sampler_descriptor_heap* frame_gpu_sampler_heap();
                cpu_sampler_descriptor_heap* frame_cpu_sampler_heap();
                
                Microsoft::WRL::ComPtr<ID3D12Resource> create_upload_buffer_resource(uint64_t size);

                descriptor_handle null_srv()        const;
                descriptor_handle null_uav()        const;
                descriptor_handle null_cbv()        const;
                descriptor_handle null_sampler()    const;

                descriptor_handle point_clamp_sampler()  const;
                descriptor_handle point_wrap_sampler()   const;
                descriptor_handle linear_clamp_sampler() const;
                descriptor_handle linear_wrap_sampler()  const;


            private:
               

                util::details::pimpl<gpu_resource_create_context_impl> m_impl;
            };

        }
    }
}
