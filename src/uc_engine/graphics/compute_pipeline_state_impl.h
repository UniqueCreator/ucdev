#pragma once

#include <uc_public/graphics/compute_pipeline_state.h>

#include <uc_dev/gx/dx12/dx12.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class ComputePipelineState::Impl
        {
            Microsoft::WRL::ComPtr<ID3D12PipelineState> m_cached_pipeline_state;
            Microsoft::WRL::ComPtr<ID3D12RootSignature> m_cached_root_signature;
            uc::gx::dx12::root_signature_meta_data      m_root_signature_meta_data;
            uc::gx::dx12::compute_pipeline_state        m_pipeline_state;

            public:
            Impl() {}

            Impl( const Microsoft::WRL::ComPtr<ID3D12PipelineState>& cached_pipeline_state,
                  const Microsoft::WRL::ComPtr<ID3D12RootSignature>& cached_root_signature,
                  const uc::gx::dx12::root_signature_meta_data&      root_signature_meta_data
            ) : m_cached_pipeline_state(cached_pipeline_state)
                , m_cached_root_signature(cached_root_signature)
                , m_root_signature_meta_data(m_root_signature_meta_data)
            {
                m_root_signature_meta_data = root_signature_meta_data;
                m_pipeline_state.m_root_signature_meta_data = &m_root_signature_meta_data;
                m_pipeline_state.m_root_signature = m_cached_root_signature.Get();
                m_pipeline_state.m_state = m_cached_pipeline_state.Get();
            }

            const uc::gx::dx12::compute_pipeline_state* pipeline_state() const
            {
                return &m_pipeline_state;
            }
        };
    }
}


