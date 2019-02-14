#pragma once

#include "virtual_resource.h"

#include <cstdint>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_pixel_buffer : public gpu_virtual_resource
            {

            private:
                using base = gpu_virtual_resource;

            public:

                explicit gpu_pixel_buffer(ID3D12Resource* resource) : base(resource)
                {

                }

                uint32_t width() const
                {
                    auto d = desc();
                    return static_cast<uint32_t>(d.Width);
                }

                uint32_t height() const
                {
                    auto d = desc();
                    return static_cast<uint32_t>(d.Height);
                }

                uint32_t array_size() const
                {
                    auto d = desc();
                    return d.DepthOrArraySize;
                }

                UINT depth() const
                {
                    auto d = desc();
                    return static_cast<uint32_t>(d.DepthOrArraySize);
                }

                DXGI_FORMAT format() const
                {
                    auto d = desc();
                    return (d.Format);
                }
            };
        }
    }
}
