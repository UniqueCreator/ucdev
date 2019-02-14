#pragma once

#include "virtual_resource.h"
#include "pixel_buffer.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_color_buffer : public gpu_pixel_buffer
            {
                private:

                using base = gpu_pixel_buffer;

                public:
                gpu_color_buffer(ID3D12Resource* resource, descriptor_handle rtv, descriptor_handle srv, descriptor_handle uav, bool is_shader_visible) : base(resource)
                    , m_RTV(rtv)
                    , m_SRV(srv)
                    , m_UAV(uav)
                    , m_is_shader_visible(is_shader_visible)
                {

                }

                descriptor_handle   rtv() const
                {
                    return m_RTV;
                }

                descriptor_handle   srv() const
                {
                    return m_SRV;
                }

                descriptor_handle   uav() const
                {
                    return m_UAV;
                }

                bool is_shader_visible() const
                {
                    return m_is_shader_visible;
                }

                private:
                descriptor_handle   m_RTV;
                descriptor_handle   m_SRV;
                descriptor_handle   m_UAV;
                bool                m_is_shader_visible;
            };

            class gpu_frame_color_buffer : public gpu_color_buffer
            {
                private:

                using base = gpu_color_buffer;

                public:
                gpu_frame_color_buffer(ID3D12Resource* resource, descriptor_handle rtv, descriptor_handle srv, descriptor_handle uav) : base(resource, rtv, srv, uav, true)
                {

                }
            };

            class gpu_view_color_buffer : public gpu_color_buffer
            {
                private:

                using base = gpu_color_buffer;

                public:
                gpu_view_color_buffer(ID3D12Resource* resource, descriptor_handle rtv, descriptor_handle srv, descriptor_handle uav) : base(resource, rtv, srv, uav, false)
                {

                }
            };
        }
    }
}


