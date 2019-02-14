#include "pch.h"

#include "compute_pipeline_state_internal.h"

#include <uc_public/uc_pimpl_impl.h>

#include "compute_pipeline_state_impl.h"

namespace UniqueCreator
{
    namespace Graphics
    {
        ComputePipelineStateInternal::ComputePipelineStateInternal
        (
            const Microsoft::WRL::ComPtr<ID3D12PipelineState>& cached_pipeline_state,
            const Microsoft::WRL::ComPtr<ID3D12RootSignature>& cached_root_signature,
            const uc::gx::dx12::root_signature_meta_data&      root_signature_meta_data
        )
        {
            m_impl = details::pimpl<Impl>( cached_pipeline_state, cached_root_signature, root_signature_meta_data );
        }
    }
}


