#include "pch.h"

#include "resource_create_context_impl.h"

#include <autogen/shaders/default_graphics_signature.h>


namespace UniqueCreator
{
    namespace Graphics
    {
        void ResourceCreateContext::Impl::CreateD3D12Device()
        {
            using namespace uc::gx::dx12;

#if defined(_DEBUG)
            {
                auto hresult = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug));
                if (SUCCEEDED(hresult))
                {
                    m_debug->EnableDebugLayer();

                    //todo: check intel driver, which failes with the gpu validation
                    m_debug->SetEnableGPUBasedValidation(TRUE);
                    m_debug->SetEnableSynchronizedCommandQueueValidation(TRUE);
                }
            }
#endif
            std::vector< Microsoft::WRL::ComPtr<IDXGIAdapter1> > adapters;
            adapters = get_adapters();
            m_device = create_device_always(adapters[0].Get());                    //put here if you have other adapters to test
        }

        ResourceCreateContext::Impl::Impl()
        {
            CreateD3D12Device();
            m_rc = std::make_unique<uc::gx::dx12::gpu_resource_create_context>( Device() );
        }

        namespace
        {
            static std::unique_ptr<uc::gx::dx12::gpu_command_queue> create_command_queue(ID3D12Device* d, D3D12_COMMAND_LIST_TYPE type)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandQueue>                      present_queue;
                {
                    auto desc = D3D12_COMMAND_QUEUE_DESC();
                    desc.Type = type;
                    present_queue = uc::gx::dx12::create_command_queue(d, &desc);
                }
                return std::make_unique< uc::gx::dx12::gpu_command_queue>(d, present_queue.Get());
            }
        }

        std::unique_ptr<DirectQueue> ResourceCreateContext::Impl::CreateDirectQueue()
        {
            return std::make_unique<DirectQueueInternal>( create_command_queue(m_device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT) );
        }

        std::unique_ptr<ComputeQueue> ResourceCreateContext::Impl::CreateComputeQueue()
        {
            return std::make_unique<ComputeQueueInternal>(create_command_queue(m_device.Get(), D3D12_COMMAND_LIST_TYPE_COMPUTE));
        }

        std::unique_ptr<CopyQueue>    ResourceCreateContext::Impl::CreateCopyQueue()
        {
            return std::make_unique<CopyQueueInternal>(create_command_queue(m_device.Get(), D3D12_COMMAND_LIST_TYPE_COPY));
        }

        std::unique_ptr<ViewColorBuffer> ResourceCreateContext::Impl::CreateViewColorBuffer( Size2D s, ColorBufferFormat format )
        {
            return std::make_unique<ViewColorBufferInternal>(std::unique_ptr<uc::gx::dx12::gpu_view_color_buffer>(m_rc->create_view_color_buffer(static_cast<uint32_t>(s.m_width), static_cast<uint32_t>(s.m_height), static_cast<DXGI_FORMAT>(format))));
        }

        std::unique_ptr<ViewDepthBuffer> ResourceCreateContext::Impl::CreateViewDepthBuffer(Size2D s, DepthBufferFormat format)
        {
            return std::make_unique<ViewDepthBufferInternal>(std::unique_ptr<uc::gx::dx12::gpu_view_depth_buffer>(m_rc->create_view_depth_buffer(static_cast<uint32_t>(s.m_width), static_cast<uint32_t>(s.m_height), static_cast<DXGI_FORMAT>(format))));
        }

        namespace
        {
            static D3D12_SHADER_BYTECODE create_shader_byte_code(const ShaderByteCode* s)
            {
                D3D12_SHADER_BYTECODE r = {};
                r.pShaderBytecode = s->m_code;
                r.BytecodeLength = s->m_code_size;
                return r;
            }

            static D3D12_COMPUTE_PIPELINE_STATE_DESC create_compute_pipeline_state_internal(ID3D12RootSignature* s, const ComputePipelineStateDescription* d)
            {
                D3D12_COMPUTE_PIPELINE_STATE_DESC r = {};
                r.CS = create_shader_byte_code(&d->m_cs);
                r.pRootSignature = s;
                return r;
            }

            struct StreamOutputDescriptionManaged : public D3D12_STREAM_OUTPUT_DESC
            {
                using Base = D3D12_STREAM_OUTPUT_DESC;
                std::vector< D3D12_SO_DECLARATION_ENTRY> m_declaration;
                std::vector<uint32_t  >                  m_bufferStrides;

                void PatchBuffers()
                {
                    if (!(m_bufferStrides.empty() || m_declaration.empty()))
                    {
                        Base::pBufferStrides = &m_bufferStrides[0];
                        Base::NumStrides = static_cast<uint32_t>(m_bufferStrides.size());

                        Base::pSODeclaration = &m_declaration[0];
                        Base::NumEntries = static_cast<uint32_t>(m_declaration.size());
                    }
                    else
                    {
                        Base::pBufferStrides = nullptr;
                        Base::NumStrides = 0;

                        Base::pSODeclaration = nullptr;
                        Base::NumEntries = 0;
                    }
                }
            };

            struct InputLayoutDescriptionManaged : public D3D12_INPUT_LAYOUT_DESC
            {
                using Base = D3D12_INPUT_LAYOUT_DESC;

                std::vector<D3D12_INPUT_ELEMENT_DESC> m_descrpitions;

                void PatchBuffers()
                {
                    if (!m_descrpitions.empty())
                    {
                        Base::NumElements = static_cast<uint32_t> (m_descrpitions.size());
                        Base::pInputElementDescs = &m_descrpitions[0];
                    }
                    else
                    {
                        Base::NumElements = 0;
                        Base::pInputElementDescs = nullptr;
                    }
                }
            };

            static StreamOutputDescriptionManaged create_stream_output_description(const StreamOutputDescription* d)
            {
                StreamOutputDescriptionManaged r = {};

                r.RasterizedStream  = d->m_rasterizedStream;

                for (auto i = 0U; i < d->m_numEntries; ++i)
                {
                    auto de = &d->m_soDeclaration[i];

                    D3D12_SO_DECLARATION_ENTRY e;
                    e.ComponentCount    = de->m_componentCount;
                    e.OutputSlot        = de->m_outputSlot;
                    e.SemanticIndex     = de->m_semanticIndex;
                    e.SemanticName      = de->m_semanticName;
                    e.StartComponent    = de->m_startComponent;
                    e.Stream            = de->m_stream;

                    r.m_declaration.push_back(e);
                }

                for (auto i = 0U; i < d->m_numStrides; ++i)
                {
                    auto stride = d->m_bufferStrides[i];
                    r.m_bufferStrides.push_back(stride);
                }

                r.PatchBuffers();
                return r;
            }

            static InputLayoutDescriptionManaged create_input_layout_description(const InputLayoutDescription* d)
            {
                InputLayoutDescriptionManaged r = {};

                for (auto i = 0U; i < d->m_numElements; ++i)
                {
                    auto de = &d->m_inputElementDescs[i];

                    D3D12_INPUT_ELEMENT_DESC e;
                    e.AlignedByteOffset = de->m_alignedByteOffset;
                    e.Format = static_cast<DXGI_FORMAT>(de->m_format);
                    e.InputSlot = de->m_inputSlot;
                    e.InputSlotClass = static_cast<D3D12_INPUT_CLASSIFICATION> (de->m_inputSlotClass);
                    e.InstanceDataStepRate = de->m_instanceDataStepRate;
                    e.SemanticIndex = de->m_semanticIndex;
                    e.SemanticName = de->m_semanticName;

                    r.m_descrpitions.push_back(e);
                }

                r.PatchBuffers();
                return r;
            }

            static D3D12_BLEND_DESC create_blend_desc(const BlendDescription* d)
            {
                D3D12_BLEND_DESC r = {};

                r.AlphaToCoverageEnable = d->m_alphaToCoverageEnable ? TRUE     : FALSE;
                r.IndependentBlendEnable = d->m_independentBlendEnable ? TRUE   : FALSE;

                for (auto i = 0U; i < 8; ++i)
                {
                    r.RenderTarget[i].BlendEnable           = d->m_renderTarget[i].m_blendEnable;
                    r.RenderTarget[i].BlendOp               = static_cast<D3D12_BLEND_OP>(d->m_renderTarget[i].m_blendOperation);
                    r.RenderTarget[i].BlendOpAlpha          = static_cast<D3D12_BLEND_OP>(d->m_renderTarget[i].m_blendOperationAlpha);
                    r.RenderTarget[i].DestBlend             = static_cast<D3D12_BLEND>(d->m_renderTarget[i].m_destinationBlend);
                    r.RenderTarget[i].DestBlendAlpha        = static_cast<D3D12_BLEND>(d->m_renderTarget[i].m_destinationBlendAlpha);
                    r.RenderTarget[i].LogicOp               = static_cast<D3D12_LOGIC_OP>(d->m_renderTarget[i].m_logicOperation);
                    r.RenderTarget[i].LogicOpEnable         = d->m_renderTarget[i].m_logicOperationEnable ? TRUE : FALSE;
                    r.RenderTarget[i].RenderTargetWriteMask = d->m_renderTarget[i].m_renderTargetWriteMask;
                    r.RenderTarget[i].SrcBlend              = static_cast<D3D12_BLEND>(d->m_renderTarget[i].m_sourceBlend);
                    r.RenderTarget[i].SrcBlendAlpha         = static_cast<D3D12_BLEND>(d->m_renderTarget[i].m_sourceBlendAlpha);
                }

                return r;
            }

            static D3D12_RASTERIZER_DESC create_rasterizer_desc(const RasterizerDescription* d)
            {
                D3D12_RASTERIZER_DESC r = {};

                r.AntialiasedLineEnable = d->m_antialiasedLineEnable ? TRUE : FALSE;
                r.ConservativeRaster    = static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(d->m_conservativeRaster);
                r.CullMode              = static_cast<D3D12_CULL_MODE>(d->m_cullMode);
                r.DepthBias             = d->m_depthBias;
                r.DepthBiasClamp        = d->m_depthBiasClamp;
                r.DepthClipEnable       = d->m_depthClipEnable ? TRUE : FALSE;
                r.FillMode              = static_cast<D3D12_FILL_MODE> (d->m_fillMode);
                r.ForcedSampleCount     = d->m_forcedSampleCount;
                r.FrontCounterClockwise = d->m_frontCounterClockwise ? TRUE : FALSE;
                r.MultisampleEnable     = d->m_multisampleEnable ? TRUE : FALSE;
                r.SlopeScaledDepthBias  = d->m_slopeScaledDepthBias;

                return r;
            }

            static D3D12_DEPTH_STENCILOP_DESC create_depth_stencil_op_desc(const DepthStencilOperationDescription* d)
            {

                D3D12_DEPTH_STENCILOP_DESC r = {};

                r.StencilDepthFailOp = static_cast<D3D12_STENCIL_OP>(d->m_stencilDepthFailOperation);
                r.StencilFailOp      = static_cast<D3D12_STENCIL_OP>(d->m_stencilFailOperation);
                r.StencilFunc        = static_cast<D3D12_COMPARISON_FUNC>(d->m_stencilFunction);
                r.StencilPassOp      = static_cast<D3D12_STENCIL_OP>(d->m_stencilPassOperation);

                return r;
            }

            static D3D12_DEPTH_STENCIL_DESC create_depth_stencil_desc(const DepthStencilDescription* d)
            {
                D3D12_DEPTH_STENCIL_DESC r = {};
                r.BackFace  = create_depth_stencil_op_desc(&d->m_backFace);
                r.DepthEnable = d->m_depthEnable ? TRUE : FALSE;
                r.DepthFunc = static_cast<D3D12_COMPARISON_FUNC> (d->m_depthFunc);
                r.DepthWriteMask = static_cast<D3D12_DEPTH_WRITE_MASK> (d->m_depthWriteMask);
                r.FrontFace = create_depth_stencil_op_desc(&d->m_frontFace);
                r.StencilEnable = d->m_stencilEnable ? TRUE : FALSE;
                r.StencilReadMask = d->m_stencilReadMask;
                r.StencilWriteMask = d->m_stencilWriteMask;

                return r;
            }

            struct D3D12GraphicsPipelineStateDescManaged : public D3D12_GRAPHICS_PIPELINE_STATE_DESC
            {

                InputLayoutDescriptionManaged   m_managed_input_layout;
                StreamOutputDescriptionManaged  m_managed_stream_out;
            };

            static D3D12GraphicsPipelineStateDescManaged create_graphics_pipeline_state_internal( ID3D12RootSignature* s, const GraphicsPipelineStateDescription* d )
            {
                D3D12GraphicsPipelineStateDescManaged r = {};

                r.pRootSignature = s;
                r.VS = create_shader_byte_code(&d->m_vs);
                r.PS = create_shader_byte_code(&d->m_ps);
                r.DS = create_shader_byte_code(&d->m_ds);
                r.HS = create_shader_byte_code(&d->m_hs);
                r.GS = create_shader_byte_code(&d->m_gs);

                r.m_managed_stream_out   = create_stream_output_description(&d->m_streamOutput);
                r.StreamOutput           = r.m_managed_stream_out;
                r.BlendState             = create_blend_desc(&d->m_blendState);
                r.SampleMask             = d->m_sampleMask;
                r.RasterizerState        = create_rasterizer_desc(&d->m_rasterizerState);
                r.DepthStencilState      = create_depth_stencil_desc(&d->m_depthStencilState);

                r.m_managed_input_layout = create_input_layout_description(&d->m_inputLayout);
                r.InputLayout            = r.m_managed_input_layout;
                r.IBStripCutValue        = static_cast<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>(d->m_ibStripCutValue);
                r.PrimitiveTopologyType  = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(d->m_primitiveTopologyType);
                r.NumRenderTargets       = d->m_numRenderTargets;

                for (auto i = 0U; i < 8; ++i)
                {
                    r.RTVFormats[i] = static_cast<DXGI_FORMAT>( d->m_rtvFormats[i] );
                }
                
                r.DSVFormat = static_cast<DXGI_FORMAT> (d->m_dsvFormat);
                r.SampleDesc.Count = d->m_sampleDescription.m_count;
                r.SampleDesc.Quality = d->m_sampleDescription.m_quality;

                return r;
            }
        }

        std::unique_ptr<GraphicsPipelineState>   ResourceCreateContext::Impl::CreateGraphicsPipelineState( const GraphicsPipelineStateDescription* desc)
        {
            auto r = rc();
            auto d = m_device.Get();

            using namespace uc::gx::dx12;

            auto sig = create_default_graphics_signature(d);
            auto blob = default_graphics_signature();
            auto description = create_graphics_pipeline_state_internal(sig, desc);

            root_signature_meta_data                meta_data = parse_root_signature(blob.code(), blob.code_size(), r->null_cbv(), r->null_srv(), r->null_uav(), r->null_sampler());
            Microsoft::WRL::ComPtr<ID3D12PipelineState> state = create_graphics_pipeline_state(d, &description);

            return std::make_unique<GraphicsPipelineStateInternal>(state, sig, meta_data);
        }

        std::unique_ptr<ComputePipelineState>   ResourceCreateContext::Impl::CreateComputePipelineState(const ComputePipelineStateDescription* desc )
        {
            auto r = rc();
            auto d = m_device.Get();

            using namespace uc::gx::dx12;

            auto sig                                          = create_default_graphics_signature(d);
            auto blob                                         = default_graphics_signature();
            auto description                                  = create_compute_pipeline_state_internal(sig, desc);

            root_signature_meta_data                meta_data = parse_root_signature(blob.code(), blob.code_size(), r->null_cbv(), r->null_srv(), r->null_uav(), r->null_sampler() );
            Microsoft::WRL::ComPtr<ID3D12PipelineState> state = create_compute_pipeline_state(d, &description);

            return std::make_unique<ComputePipelineStateInternal>(state, sig, meta_data);
        }

        std::unique_ptr<RootSignature> ResourceCreateContext::Impl::CreateRootSignature(const RootSignatureByteCode* code)
        {
            auto r = rc();
            auto d = m_device.Get();

            using namespace uc::gx::dx12;
            root_signature_meta_data meta_data = parse_root_signature(code->m_code, code->m_code_size, r->null_cbv(), r->null_srv(), r->null_uav(), r->null_sampler());
            Microsoft::WRL::ComPtr<ID3D12RootSignature> state = create_root_signature(d, 0, code->m_code, code->m_code_size);
            return std::make_unique<RootSignatureInternal>(state, meta_data);
        }

        std::unique_ptr<Texture2D> ResourceCreateContext::Impl::CreateTexture2D(Size2D s, GraphicsFormat format)
        {
            return std::make_unique<Texture2DInternal>(uc::gx::dx12::create_texture_2d(rc(), static_cast<uint32_t>(s.m_width), static_cast<uint32_t>(s.m_height), static_cast<DXGI_FORMAT>(format)));
        }

        std::unique_ptr<Texture2D> ResourceCreateContext::Impl::CreateTexture2D(Size2D s, GraphicsFormat format, uint32_t mipCount)
        {
            return std::make_unique<Texture2DInternal>(uc::gx::dx12::create_texture_2d(rc(), static_cast<uint32_t>(s.m_width), static_cast<uint32_t>(s.m_height), static_cast<DXGI_FORMAT>(format), mipCount));
        }

        std::unique_ptr<Buffer> ResourceCreateContext::Impl::CreateBuffer(uint64_t size)
        {
            return std::make_unique<BufferInternal>(uc::gx::dx12::create_buffer(rc(), static_cast<uint32_t>(size) ) );
        }

        void ResourceCreateContext::Impl::Sync()
        {
            m_rc->sync();
        }

        void ResourceCreateContext::Impl::ResetViewDependentResources()
        {
            m_rc->reset_view_dependent_resources();
        }
    }
}


