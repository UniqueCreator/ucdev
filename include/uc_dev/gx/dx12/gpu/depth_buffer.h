#pragma once

#include "pixel_buffer.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_depth_buffer : public gpu_pixel_buffer
            {

            private:

                using base = gpu_pixel_buffer;

            public:

                gpu_depth_buffer() : base(nullptr)
                {

                }

                gpu_depth_buffer(ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2], bool is_shader_visible) : base(resource)
                {
                    m_SRV = srv;

                    m_DSV[0] = dsv[0];
                    m_DSV[1] = dsv[1];

                    m_is_shader_visible = is_shader_visible;
                }

                descriptor_handle dsv() const
                {
                    return m_DSV[0];
                }

                descriptor_handle dsv_read_depth() const
                {
                    return m_DSV[1];
                }

                descriptor_handle srv_depth() const
                {
                    return m_SRV;
                }

                bool is_shader_visible() const { return m_is_shader_visible; }

            private:

                descriptor_handle   m_DSV[2];
                descriptor_handle   m_SRV;

                bool m_is_shader_visible = false;
            };


            class gpu_msaa_depth_buffer : public gpu_depth_buffer
            {
                private:

                using base = gpu_depth_buffer;

                public:
                gpu_msaa_depth_buffer() 
                {
                }

                gpu_msaa_depth_buffer( ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2], bool is_shader_visible) : base(resource, srv, dsv, is_shader_visible)
                {

                }
            };

            class gpu_view_depth_buffer : public gpu_depth_buffer
            {
                using base = gpu_depth_buffer;

                public:
                gpu_view_depth_buffer() {}
                gpu_view_depth_buffer(ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2]) : base(resource, srv, dsv, false)
                {

                }
            };

            class gpu_frame_depth_buffer : public gpu_depth_buffer
            {
                using base = gpu_depth_buffer;

                public:
                gpu_frame_depth_buffer() {}
                gpu_frame_depth_buffer(ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2]) : base(resource, srv, dsv, true)
                {

                }
            };

            class gpu_view_msaa_depth_buffer : public gpu_msaa_depth_buffer
            {
                private:

                using base = gpu_msaa_depth_buffer;

                public:
                gpu_view_msaa_depth_buffer() {}
                gpu_view_msaa_depth_buffer(ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2]) : base(resource, srv, dsv, false)
                {

                }
            };

            class gpu_frame_msaa_depth_buffer : public gpu_msaa_depth_buffer
            {
                private:
                using base = gpu_msaa_depth_buffer;

                public:
                gpu_frame_msaa_depth_buffer() {}
                gpu_frame_msaa_depth_buffer(ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2]) : base(resource, srv, dsv, true)
                {

                }
            };
        }
    }
}
