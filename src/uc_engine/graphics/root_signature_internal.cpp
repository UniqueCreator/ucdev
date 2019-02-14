#include "pch.h"

#include "root_signature_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "root_signature_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        RootSignatureInternal::RootSignatureInternal
        (
            const Microsoft::WRL::ComPtr<ID3D12RootSignature>& cached_root_signature,
            const uc::gx::dx12::root_signature_meta_data&      root_signature_meta_data
        )
        {
            m_impl = details::pimpl<Impl>( cached_root_signature, root_signature_meta_data );
        }
    }
}


