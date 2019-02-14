#pragma once

#include <uc_public/graphics/root_signature.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class RootSignatureInternal : public RootSignature
        {
        public:
            RootSignatureInternal
            (
                const Microsoft::WRL::ComPtr<ID3D12RootSignature>& cached_root_signature,
                const uc::gx::dx12::root_signature_meta_data&      root_signature_meta_data
            );
        };
    }
}


