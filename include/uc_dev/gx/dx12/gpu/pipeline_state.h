#pragma once

#include <d3d12.h>

#include <uc_dev/gx/dx12/cmd/root_signature_meta_data.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct graphics_pipeline_state
            {
                ID3D12PipelineState*        m_state = nullptr;
                ID3D12RootSignature*        m_root_signature = nullptr;
                root_signature_meta_data*   m_root_signature_meta_data = nullptr;
            };

            struct compute_pipeline_state
            {
                ID3D12PipelineState*        m_state = nullptr;
                ID3D12RootSignature*        m_root_signature = nullptr;
                root_signature_meta_data*   m_root_signature_meta_data = nullptr;
            };

        }
    }
}
