#pragma once

#include <uc_public/graphics/root_signature.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RootSignature::Impl
        {

            Microsoft::WRL::ComPtr<ID3D12RootSignature> m_cached_root_signature;
            uc::gx::dx12::root_signature_meta_data      m_root_signature_meta_data;

            public:

            Impl() {}

            Impl( const Microsoft::WRL::ComPtr<ID3D12RootSignature>& cached_root_signature,
                  const uc::gx::dx12::root_signature_meta_data&      root_signature_meta_data
            ) : m_cached_root_signature(cached_root_signature)
                , m_root_signature_meta_data(m_root_signature_meta_data)
            {
                m_root_signature_meta_data = root_signature_meta_data;
            }
        };
    }
}


