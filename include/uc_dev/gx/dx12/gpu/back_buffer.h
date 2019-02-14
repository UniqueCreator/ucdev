#pragma once

#include "virtual_resource.h"
#include "pixel_buffer.h"
#include "descriptor_heap.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_back_buffer : public gpu_pixel_buffer
            {

            private:
                using base = gpu_pixel_buffer;

            public:
                gpu_back_buffer() : base(nullptr)
                {

                }

                explicit gpu_back_buffer(ID3D12Resource* resource) : base(resource)
                {

                }

                gpu_back_buffer(ID3D12Resource* resource, descriptor_handle rtv) : base(resource)
                    , m_RTV(rtv)
                {

                }

                descriptor_handle   rtv() const
                {
                    return m_RTV;
                }

                bool is_shader_visible() const { return false; }

                private:

                descriptor_handle   m_RTV;
            };
        }
    }
}


