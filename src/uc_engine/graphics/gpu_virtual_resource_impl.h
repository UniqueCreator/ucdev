#pragma once

#include <uc_public/graphics/gpu_virtual_resource.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class GpuVirtualResource::Impl
        {
            public:

            Impl()
            {

            }

            Impl( uc::gx::dx12::gpu_virtual_resource* b ) : m_resource(b)
            {

            }

            uc::gx::dx12::gpu_virtual_resource* resource() const
            {
                return m_resource;
            }

            protected:

            uc::gx::dx12::gpu_virtual_resource* m_resource = nullptr;
        };
    }
}


