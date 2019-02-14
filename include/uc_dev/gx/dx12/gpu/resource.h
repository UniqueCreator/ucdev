#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource : public util::noncopyable
            {
                public: 

                explicit gpu_resource( ID3D12Resource* resource ) : m_resource(resource)
                {

                }


                D3D12_RESOURCE_DESC desc() const
                {
                    return m_resource->GetDesc();
                }

                ID3D12Resource* resource() const
                {
                    return m_resource.Get();
                }

                protected:
                Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
            };
        }
    }
}
