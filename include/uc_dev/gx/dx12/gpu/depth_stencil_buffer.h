#pragma once

#include "pixel_buffer.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_depth_stencil_buffer : public gpu_pixel_buffer
            {

            private:

                using base = gpu_pixel_buffer;

            public:

                gpu_depth_stencil_buffer() : base(nullptr)
                {

                }

                gpu_depth_stencil_buffer(ID3D12Resource* resource, descriptor_handle srv, descriptor_handle dsv[2]) : base(resource)
                {
                    m_SRV = srv;

                    m_DSV[0] = dsv[0];
                    m_DSV[1] = dsv[1];
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

                bool is_shader_visible() const { return false; }

            private:

                descriptor_handle   m_DSV[2];
                descriptor_handle   m_SRV;
            };
        }
    }
}
