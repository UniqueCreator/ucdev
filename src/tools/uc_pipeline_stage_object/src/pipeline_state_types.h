#pragma once

#if defined(UC_BACKEND_DEV)

    #include <d3d12.h>

#else

    #include <uc_public/graphics/graphics_types.h>

    using namespace UniqueCreator::Graphics;
    
    #define DXGI_FORMAT GraphicsFormat
    #define DXGI_FORMAT_UNKNOWN GraphicsFormat::UNKNOWN
    #define DXGI_FORMAT_R32G32B32A32_TYPELESS  GraphicsFormat::R32G32B32A32_TYPELESS
    #define DXGI_FORMAT_R32G32B32A32_FLOAT  GraphicsFormat::R32G32B32A32_FLOAT
    #define DXGI_FORMAT_R32G32B32A32_UINT  GraphicsFormat::R32G32B32A32_UINT
    #define DXGI_FORMAT_R32G32B32A32_SINT  GraphicsFormat::R32G32B32A32_SINT
    #define DXGI_FORMAT_R32G32B32_TYPELESS  GraphicsFormat::R32G32B32_TYPELESS
    #define DXGI_FORMAT_R32G32B32_FLOAT  GraphicsFormat::R32G32B32_FLOAT
    #define DXGI_FORMAT_R32G32B32_UINT  GraphicsFormat::R32G32B32_UINT
    #define DXGI_FORMAT_R32G32B32_SINT  GraphicsFormat::R32G32B32_SINT
    #define DXGI_FORMAT_R16G16B16A16_TYPELESS  GraphicsFormat::R16G16B16A16_TYPELESS
    #define DXGI_FORMAT_R16G16B16A16_FLOAT  GraphicsFormat::R16G16B16A16_FLOAT
    #define DXGI_FORMAT_R16G16B16A16_UNORM  GraphicsFormat::R16G16B16A16_UNORM
    #define DXGI_FORMAT_R16G16B16A16_UINT  GraphicsFormat::R16G16B16A16_UINT
    #define DXGI_FORMAT_R16G16B16A16_SNORM  GraphicsFormat::R16G16B16A16_SNORM
    #define DXGI_FORMAT_R16G16B16A16_SINT  GraphicsFormat::R16G16B16A16_SINT
    #define DXGI_FORMAT_R32G32_TYPELESS  GraphicsFormat::R32G32_TYPELESS
    #define DXGI_FORMAT_R32G32_FLOAT  GraphicsFormat::R32G32_FLOAT
    #define DXGI_FORMAT_R32G32_UINT  GraphicsFormat::R32G32_UINT
    #define DXGI_FORMAT_R32G32_SINT  GraphicsFormat::R32G32_SINT
    #define DXGI_FORMAT_R32G8X24_TYPELESS  GraphicsFormat::R32G8X24_TYPELESS
    #define DXGI_FORMAT_D32_FLOAT_S8X24_UINT  GraphicsFormat::D32_FLOAT_S8X24_UINT
    #define DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS GraphicsFormat::R32_FLOAT_X8X24_TYPELESS
    #define DXGI_FORMAT_X32_TYPELESS_G8X24_UINT  GraphicsFormat::X32_TYPELESS_G8X24_UINT
    #define DXGI_FORMAT_R10G10B10A2_TYPELESS  GraphicsFormat::R10G10B10A2_TYPELESS
    #define DXGI_FORMAT_R10G10B10A2_UNORM  GraphicsFormat::R10G10B10A2_UNORM
    #define DXGI_FORMAT_R10G10B10A2_UINT  GraphicsFormat::R10G10B10A2_UINT
    #define DXGI_FORMAT_R11G11B10_FLOAT  GraphicsFormat::R11G11B10_FLOAT
    #define DXGI_FORMAT_R8G8B8A8_TYPELESS  GraphicsFormat::R8G8B8A8_TYPELESS
    #define DXGI_FORMAT_R8G8B8A8_UNORM  GraphicsFormat::R8G8B8A8_UNORM
    #define DXGI_FORMAT_R8G8B8A8_UNORM_SRGB  GraphicsFormat::R8G8B8A8_UNORM_SRGB
    #define DXGI_FORMAT_R8G8B8A8_UINT  GraphicsFormat::R8G8B8A8_UINT
    #define DXGI_FORMAT_R8G8B8A8_SNORM  GraphicsFormat::R8G8B8A8_SNORM
    #define DXGI_FORMAT_R8G8B8A8_SINT  GraphicsFormat::R8G8B8A8_SINT
    #define DXGI_FORMAT_R16G16_TYPELESS  GraphicsFormat::R16G16_TYPELESS
    #define DXGI_FORMAT_R16G16_FLOAT  GraphicsFormat::R16G16_FLOAT
    #define DXGI_FORMAT_R16G16_UNORM  GraphicsFormat::R16G16_UNORM
    #define DXGI_FORMAT_R16G16_UINT  GraphicsFormat::R16G16_UINT
    #define DXGI_FORMAT_R16G16_SNORM  GraphicsFormat::R16G16_SNORM
    #define DXGI_FORMAT_R16G16_SINT  GraphicsFormat::R16G16_SINT
    #define DXGI_FORMAT_R32_TYPELESS  GraphicsFormat::R32_TYPELESS
    #define DXGI_FORMAT_D32_FLOAT  GraphicsFormat::D32_FLOAT
    #define DXGI_FORMAT_R32_FLOAT  GraphicsFormat::R32_FLOAT
    #define DXGI_FORMAT_R32_UINT  GraphicsFormat::R32_UINT
    #define DXGI_FORMAT_R32_SINT  GraphicsFormat::R32_SINT
    #define DXGI_FORMAT_R24G8_TYPELESS  GraphicsFormat::R24G8_TYPELESS
    #define DXGI_FORMAT_D24_UNORM_S8_UINT  GraphicsFormat::D24_UNORM_S8_UINT
    #define DXGI_FORMAT_R24_UNORM_X8_TYPELESS  GraphicsFormat::R24_UNORM_X8_TYPELESS
    #define DXGI_FORMAT_X24_TYPELESS_G8_UINT  GraphicsFormat::X24_TYPELESS_G8_UINT
    #define DXGI_FORMAT_R8G8_TYPELESS  GraphicsFormat::R8G8_TYPELESS
    #define DXGI_FORMAT_R8G8_UNORM  GraphicsFormat::R8G8_UNORM
    #define DXGI_FORMAT_R8G8_UINT  GraphicsFormat::R8G8_UINT
    #define DXGI_FORMAT_R8G8_SNORM GraphicsFormat::R8G8_SNORM
    #define DXGI_FORMAT_R8G8_SINT  GraphicsFormat::R8G8_SINT
    #define DXGI_FORMAT_R16_TYPELESS  GraphicsFormat::R16_TYPELESS
    #define DXGI_FORMAT_R16_FLOAT  GraphicsFormat::R16_FLOAT
    #define DXGI_FORMAT_D16_UNORM  GraphicsFormat::D16_UNORM
    #define DXGI_FORMAT_R16_UNORM  GraphicsFormat::R16_UNORM
    #define DXGI_FORMAT_R16_UINT  GraphicsFormat::R16_UINT
    #define DXGI_FORMAT_R16_SNORM  GraphicsFormat::R16_SNORM
    #define DXGI_FORMAT_R16_SINT  GraphicsFormat::R16_SINT
    #define DXGI_FORMAT_R8_TYPELESS  GraphicsFormat::R8_TYPELESS
    #define DXGI_FORMAT_R8_UNORM  GraphicsFormat::R8_UNORM
    #define DXGI_FORMAT_R8_UINT  GraphicsFormat::R8_UINT
    #define DXGI_FORMAT_R8_SNORM  GraphicsFormat::R8_SNORM
    #define DXGI_FORMAT_R8_SINT  GraphicsFormat::R8_SINT
    #define DXGI_FORMAT_A8_UNORM GraphicsFormat::A8_UNORM
    #define DXGI_FORMAT_R1_UNORM  GraphicsFormat::R1_UNORM
    #define DXGI_FORMAT_R9G9B9E5_SHAREDEXP  GraphicsFormat::R9G9B9E5_SHAREDEXP
    #define DXGI_FORMAT_R8G8_B8G8_UNORM  GraphicsFormat::R8G8_B8G8_UNORM
    #define DXGI_FORMAT_G8R8_G8B8_UNORM  GraphicsFormat::G8R8_G8B8_UNORM
    #define DXGI_FORMAT_BC1_TYPELESS  GraphicsFormat::BC1_TYPELESS
    #define DXGI_FORMAT_BC1_UNORM  GraphicsFormat::BC1_UNORM
    #define DXGI_FORMAT_BC1_UNORM_SRGB  GraphicsFormat::BC1_UNORM_SRGB
    #define DXGI_FORMAT_BC2_TYPELESS  GraphicsFormat::BC2_TYPELESS
    #define DXGI_FORMAT_BC2_UNORM  GraphicsFormat::BC2_UNORM
    #define DXGI_FORMAT_BC2_UNORM_SRGB  GraphicsFormat::BC2_UNORM_SRGB
    #define DXGI_FORMAT_BC3_TYPELESS  GraphicsFormat::BC3_TYPELESS
    #define DXGI_FORMAT_BC3_UNORM  GraphicsFormat::BC3_UNORM
    #define DXGI_FORMAT_BC3_UNORM_SRGB  GraphicsFormat::BC3_UNORM_SRGB
    #define DXGI_FORMAT_BC4_TYPELESS GraphicsFormat::BC4_TYPELESS
    #define DXGI_FORMAT_BC4_UNORM GraphicsFormat::BC4_UNORM
    #define DXGI_FORMAT_BC4_SNORM GraphicsFormat::BC4_SNORM
    #define DXGI_FORMAT_BC5_TYPELESS GraphicsFormat::BC5_TYPELESS
    #define DXGI_FORMAT_BC5_UNORM GraphicsFormat::BC5_UNORM
    #define DXGI_FORMAT_BC5_SNORM GraphicsFormat::BC5_SNORM
    #define DXGI_FORMAT_B5G6R5_UNORM GraphicsFormat::B5G6R5_UNORM
    #define DXGI_FORMAT_B5G5R5A1_UNORM GraphicsFormat::B5G5R5A1_UNORM
    #define DXGI_FORMAT_B8G8R8A8_UNORM GraphicsFormat::B8G8R8A8_UNORM
    #define DXGI_FORMAT_B8G8R8X8_UNORM GraphicsFormat::B8G8R8X8_UNORM
    #define DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM GraphicsFormat::R10G10B10_XR_BIAS_A2_UNORM
    #define DXGI_FORMAT_B8G8R8A8_TYPELESS GraphicsFormat::B8G8R8A8_TYPELESS
    #define DXGI_FORMAT_B8G8R8A8_UNORM_SRGB GraphicsFormat::B8G8R8A8_UNORM_SRGB
    #define DXGI_FORMAT_B8G8R8X8_TYPELESS GraphicsFormat::B8G8R8X8_TYPELESS
    #define DXGI_FORMAT_B8G8R8X8_UNORM_SRGB GraphicsFormat::B8G8R8X8_UNORM_SRGB
    #define DXGI_FORMAT_BC6H_TYPELESS GraphicsFormat::BC6H_TYPELESS
    #define DXGI_FORMAT_BC6H_UF16 GraphicsFormat::BC6H_UF16
    #define DXGI_FORMAT_BC6H_SF16 GraphicsFormat::BC6H_SF16
    #define DXGI_FORMAT_BC7_TYPELESS GraphicsFormat::BC7_TYPELESS
    #define DXGI_FORMAT_BC7_UNORM GraphicsFormat::BC7_UNORM
    #define DXGI_FORMAT_BC7_UNORM_SRGB GraphicsFormat::BC7_UNORM_SRGB
    #define DXGI_FORMAT_AYUV GraphicsFormat::AYUV
    #define DXGI_FORMAT_Y410 GraphicsFormat::Y410
    #define DXGI_FORMAT_Y416 GraphicsFormat::Y416
    #define DXGI_FORMAT_NV12 GraphicsFormat::NV12
    #define DXGI_FORMAT_P010 GraphicsFormat::P010
    #define DXGI_FORMAT_P016 GraphicsFormat::P016
    #define DXGI_FORMAT_420_OPAQUE GraphicsFormat::F420_OPAQUE
    #define DXGI_FORMAT_YUY2 GraphicsFormat::YUY2
    #define DXGI_FORMAT_Y210 GraphicsFormat::Y210
    #define DXGI_FORMAT_Y216 GraphicsFormat::Y216
    #define DXGI_FORMAT_NV11 GraphicsFormat::NV11
    #define DXGI_FORMAT_AI44 GraphicsFormat::AI44
    #define DXGI_FORMAT_IA44 GraphicsFormat::IA44
    #define DXGI_FORMAT_P8 GraphicsFormat::P8
    #define DXGI_FORMAT_A8P8 GraphicsFormat::A8P8
    #define DXGI_FORMAT_B4G4R4A4_UNORM GraphicsFormat::B4G4R4A4_UNORM
    #define DXGI_FORMAT_P208 GraphicsFormat::P208
    #define DXGI_FORMAT_V208 GraphicsFormat::V208
    #define DXGI_FORMAT_V408 GraphicsFormat::V408

    #define D3D12_INPUT_CLASSIFICATION                      InputClassification 
    #define D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA      InputClassification::PerVertexData
    #define D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA    InputClassification::PerInstanceData

    #define D3D12_BLEND Blend

    #define D3D12_BLEND_ZERO                Blend::Zero
    #define D3D12_BLEND_ONE                 Blend::One
    #define D3D12_BLEND_SRC_COLOR           Blend::SourceColor
    #define D3D12_BLEND_INV_SRC_COLOR       Blend::InverseSourceColor
    #define D3D12_BLEND_SRC_ALPHA           Blend::SourceAlpha
    #define D3D12_BLEND_INV_SRC_ALPHA       Blend::InverseSourceAlpha
    #define D3D12_BLEND_DEST_ALPHA          Blend::DestinationAlpha
    #define D3D12_BLEND_INV_DEST_ALPHA      Blend::InverseDestinationAlpha
    #define D3D12_BLEND_DEST_COLOR          Blend::DestinationColor
    #define D3D12_BLEND_INV_DEST_COLOR      Blend::InverseDestinationColor
    #define D3D12_BLEND_SRC_ALPHA_SAT       Blend::SourceAlphaSaturate
    #define D3D12_BLEND_BLEND_FACTOR        Blend::Factor
    #define D3D12_BLEND_INV_BLEND_FACTOR    Blend::InverseBlendFactor
    #define D3D12_BLEND_SRC1_COLOR          Blend::SourceOneColor
    #define D3D12_BLEND_INV_SRC1_COLOR      Blend::InverseSourceOneColor
    #define D3D12_BLEND_SRC1_ALPHA          Blend::SourceOneAlpha
    #define D3D12_BLEND_INV_SRC1_ALPHA      Blend::InverseSourceOneAlpha

    #define D3D12_BLEND_OP BlendOperation

    #define D3D12_BLEND_OP_ADD BlendOperation::Add 
    #define D3D12_BLEND_OP_SUBTRACT BlendOperation::Subtract
    #define D3D12_BLEND_OP_REV_SUBTRACT BlendOperation::ReverseSubtract
    #define D3D12_BLEND_OP_MIN BlendOperation::Min
    #define D3D12_BLEND_OP_MAX BlendOperation::Max

    #define DXGI_SAMPLE_DESC SampleDescription

    #define D3D12_LOGIC_OP LogicOperation

    #define D3D12_LOGIC_OP_CLEAR LogicOperation::Clear
    #define D3D12_LOGIC_OP_SET LogicOperation::Set
    #define D3D12_LOGIC_OP_COPY LogicOperation::Copy
    #define D3D12_LOGIC_OP_COPY_INVERTED LogicOperation::CopyInverted
    #define D3D12_LOGIC_OP_NOOP LogicOperation::Noop
    #define D3D12_LOGIC_OP_INVERT LogicOperation::Invert
    #define D3D12_LOGIC_OP_AND LogicOperation::And
    #define D3D12_LOGIC_OP_NAND LogicOperation::Nand
    #define D3D12_LOGIC_OP_OR LogicOperation::Or
    #define D3D12_LOGIC_OP_NOR LogicOperation::Nor
    #define D3D12_LOGIC_OP_XOR LogicOperation::Xor
    #define D3D12_LOGIC_OP_EQUIV LogicOperation::Equiv
    #define D3D12_LOGIC_OP_AND_REVERSE LogicOperation::AndReverse
    #define D3D12_LOGIC_OP_AND_INVERTED LogicOperation::AndInverted
    #define D3D12_LOGIC_OP_OR_REVERSE LogicOperation::OrReverse
    #define D3D12_LOGIC_OP_OR_INVERTED LogicOperation::OrInverted


    #define D3D12_DEPTH_STENCILOP_DESC      DepthStencilOperationDescription
    #define D3D12_RENDER_TARGET_BLEND_DESC  RenderTargetBlendDescription 
    #define D3D12_BLEND_DESC                BlendDescription

    #define D3D12_FILL_MODE FillMode
    #define D3D12_FILL_MODE_WIREFRAME FillMode::WireFrame
    #define D3D12_FILL_MODE_SOLID     FillMode::Solid

    #define D3D12_CULL_MODE CullMode
    #define D3D12_CULL_MODE_NONE CullMode::None
    #define D3D12_CULL_MODE_FRONT CullMode::Front
    #define D3D12_CULL_MODE_BACK CullMode::Back

    #define D3D12_CONSERVATIVE_RASTERIZATION_MODE ConservativeRasterizationMode
    #define D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF ConservativeRasterizationMode::Off
    #define D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON  ConservativeRasterizationMode::On

    #define D3D12_RASTERIZER_DESC RasterizerDescription

    #define D3D12_DEPTH_WRITE_MASK DepthWriteMask
    #define D3D12_DEPTH_WRITE_MASK_ZERO DepthWriteMask::Zero
    #define D3D12_DEPTH_WRITE_MASK_ALL DepthWriteMask::All

    #define D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IndexBufferStripCut

    #define D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED     IndexBufferStripCut::ValueDisabled
    #define D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF       IndexBufferStripCut::Value0xFFFF
    #define D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF   IndexBufferStripCut::Value0xFFFFFFFF

    #define D3D12_PRIMITIVE_TOPOLOGY_TYPE PrimitiveTopologyType
    #define D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED PrimitiveTopologyType::Undefined
    #define D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT     PrimitiveTopologyType::Point
    #define D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE      PrimitiveTopologyType::Line
    #define D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE  PrimitiveTopologyType::Triangle
    #define D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH     PrimitiveTopologyType::Patch

    #define D3D12_COMPARISON_FUNC ComparisonFunction
    #define D3D12_COMPARISON_FUNC_NEVER ComparisonFunction::Never
    #define D3D12_COMPARISON_FUNC_LESS ComparisonFunction::Less
    #define D3D12_COMPARISON_FUNC_EQUAL ComparisonFunction::Equal
    #define D3D12_COMPARISON_FUNC_LESS_EQUAL ComparisonFunction::LessEqual
    #define D3D12_COMPARISON_FUNC_GREATER ComparisonFunction::Greater
    #define D3D12_COMPARISON_FUNC_NOT_EQUAL ComparisonFunction::NotEqual
    #define D3D12_COMPARISON_FUNC_GREATER_EQUAL ComparisonFunction::GreaterEqual
    #define D3D12_COMPARISON_FUNC_ALWAYS ComparisonFunction::Always

    #define D3D12_STENCIL_OP StencilOperation
    #define D3D12_STENCIL_OP_KEEP StencilOperation::Keep
    #define D3D12_STENCIL_OP_ZERO StencilOperation::Zero
    #define D3D12_STENCIL_OP_REPLACE StencilOperation::Replace
    #define D3D12_STENCIL_OP_INCR_SAT StencilOperation::IncrementSaturate
    #define D3D12_STENCIL_OP_DECR_SAT StencilOperation::DecrementSaturate
    #define D3D12_STENCIL_OP_INVERT StencilOperation::Invert
    #define D3D12_STENCIL_OP_INCR StencilOperation::Increment
    #define D3D12_STENCIL_OP_DECR StencilOperation::Decrement

    #define D3D12_DEPTH_STENCIL_DESC DepthStencilDescription

    enum ColorWriteEnableX : uint32_t
    {
        ColorWriteEnableX_Red = 1,
        ColorWriteEnableX_Green = 2,
        ColorWriteEnableX_Blue = 4,
        ColorWriteEnableX_Alpha = 8,
        ColorWriteEnableX_All = ColorWriteEnableX_Red | ColorWriteEnableX_Green | ColorWriteEnableX_Blue | ColorWriteEnableX_Alpha
    };

    #define D3D12_COLOR_WRITE_ENABLE ColorWriteEnableX

    #define D3D12_COLOR_WRITE_ENABLE_RED ColorWriteEnableX::ColorWriteEnableX_Red
    #define D3D12_COLOR_WRITE_ENABLE_GREEN ColorWriteEnableX::ColorWriteEnableX_Green
    #define D3D12_COLOR_WRITE_ENABLE_BLUE ColorWriteEnableX::ColorWriteEnableX_Blue
    #define D3D12_COLOR_WRITE_ENABLE_ALPHA ColorWriteEnableX::ColorWriteEnableX_Alpha
    #define D3D12_COLOR_WRITE_ENABLE_ALL ColorWriteEnableX::ColorWriteEnableX_All


    #define D3D12_COLOR_WRITE_ENABLE_RED_X ColorWriteEnable::Red
    #define D3D12_COLOR_WRITE_ENABLE_GREEN_X ColorWriteEnable::Green
    #define D3D12_COLOR_WRITE_ENABLE_BLUE_X ColorWriteEnable::Blue
    #define D3D12_COLOR_WRITE_ENABLE_ALPHA_X ColorWriteEnable::Alpha
    #define D3D12_COLOR_WRITE_ENABLE_ALL_X ColorWriteEnable::All

    #define D3D12_STREAM_OUTPUT_DESC   StreamOutputDescription
    #define D3D12_SO_DECLARATION_ENTRY StreamOutputDeclarationEntry
    #define D3D12_INPUT_LAYOUT_DESC    InputLayoutDescription
    #define D3D12_INPUT_ELEMENT_DESC   InputElementDescription


    typedef
    enum D3D12_PIPELINE_STATE_FLAGS
    {
        D3D12_PIPELINE_STATE_FLAG_NONE = 0,
        D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG = 0x1
    } 	D3D12_PIPELINE_STATE_FLAGS;

#endif

#define STRINGIFY(x) #x
#define STRINGIFY_TYPE(x) STRINGIFY(x)


