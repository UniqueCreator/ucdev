#pragma once

#include <uc_public/graphics/graphics_pipeline_state.h>
#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class GraphicsPipelineStateInternal : public GraphicsPipelineState
        {
            public:
                GraphicsPipelineStateInternal
                (
                    const Microsoft::WRL::ComPtr<ID3D12PipelineState>& cached_pipeline_state,
                    const Microsoft::WRL::ComPtr<ID3D12RootSignature>& cached_root_signature,
                    const uc::gx::dx12::root_signature_meta_data&      root_signature_meta_data
                );
        };
    }
}


