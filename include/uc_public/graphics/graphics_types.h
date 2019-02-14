#pragma once

#include <cstdint>

namespace UniqueCreator
{
    namespace Graphics
    {
        struct Size2D final
        {
            float m_width;
            float m_height;
        };

        struct Size3D final
        {
            float m_width;
            float m_height;
            float m_depth;
        };

        struct Rectangle2D final
        {
            float m_left;
            float m_top;
            float m_right;
            float m_bottom;
        };

        struct ViewPort final
        {
            float m_topLeftX;
            float m_topLeftY;
            float m_width;
            float m_height;
            float m_minDepth;
            float m_maxDepth;
        };

        struct FenceValue final
        {
            public:

            FenceValue(uint64_t value = 0 ) : m_value(value)
            {

            }

            ~FenceValue() = default;
            FenceValue(const FenceValue&) = default;
            FenceValue(FenceValue&&) = default;

            FenceValue& operator=(const FenceValue&) = default;
            FenceValue& operator=(FenceValue&&) = default;

            operator uint64_t () const
            {
                return m_value;
            }

            private:
            uint64_t m_value = 0;
        };

        enum class GraphicsFormat : uint32_t
        {
            UNKNOWN = 0,
            R32G32B32A32_TYPELESS = 1,
            R32G32B32A32_FLOAT = 2,
            R32G32B32A32_UINT = 3,
            R32G32B32A32_SINT = 4,
            R32G32B32_TYPELESS = 5,
            R32G32B32_FLOAT = 6,
            R32G32B32_UINT = 7,
            R32G32B32_SINT = 8,
            R16G16B16A16_TYPELESS = 9,
            R16G16B16A16_FLOAT = 10,
            R16G16B16A16_UNORM = 11,
            R16G16B16A16_UINT = 12,
            R16G16B16A16_SNORM = 13,
            R16G16B16A16_SINT = 14,
            R32G32_TYPELESS = 15,
            R32G32_FLOAT = 16,
            R32G32_UINT = 17,
            R32G32_SINT = 18,
            R32G8X24_TYPELESS = 19,
            D32_FLOAT_S8X24_UINT = 20,
            R32_FLOAT_X8X24_TYPELESS = 21,
            X32_TYPELESS_G8X24_UINT = 22,
            R10G10B10A2_TYPELESS = 23,
            R10G10B10A2_UNORM = 24,
            R10G10B10A2_UINT = 25,
            R11G11B10_FLOAT = 26,
            R8G8B8A8_TYPELESS = 27,
            R8G8B8A8_UNORM = 28,
            R8G8B8A8_UNORM_SRGB = 29,
            R8G8B8A8_UINT = 30,
            R8G8B8A8_SNORM = 31,
            R8G8B8A8_SINT = 32,
            R16G16_TYPELESS = 33,
            R16G16_FLOAT = 34,
            R16G16_UNORM = 35,
            R16G16_UINT = 36,
            R16G16_SNORM = 37,
            R16G16_SINT = 38,
            R32_TYPELESS = 39,
            D32_FLOAT = 40,
            R32_FLOAT = 41,
            R32_UINT = 42,
            R32_SINT = 43,
            R24G8_TYPELESS = 44,
            D24_UNORM_S8_UINT = 45,
            R24_UNORM_X8_TYPELESS = 46,
            X24_TYPELESS_G8_UINT = 47,
            R8G8_TYPELESS = 48,
            R8G8_UNORM = 49,
            R8G8_UINT = 50,
            R8G8_SNORM = 51,
            R8G8_SINT = 52,
            R16_TYPELESS = 53,
            R16_FLOAT = 54,
            D16_UNORM = 55,
            R16_UNORM = 56,
            R16_UINT = 57,
            R16_SNORM = 58,
            R16_SINT = 59,
            R8_TYPELESS = 60,
            R8_UNORM = 61,
            R8_UINT = 62,
            R8_SNORM = 63,
            R8_SINT = 64,
            A8_UNORM = 65,
            R1_UNORM = 66,
            R9G9B9E5_SHAREDEXP = 67,
            R8G8_B8G8_UNORM = 68,
            G8R8_G8B8_UNORM = 69,
            BC1_TYPELESS = 70,
            BC1_UNORM = 71,
            BC1_UNORM_SRGB = 72,
            BC2_TYPELESS = 73,
            BC2_UNORM = 74,
            BC2_UNORM_SRGB = 75,
            BC3_TYPELESS = 76,
            BC3_UNORM = 77,
            BC3_UNORM_SRGB = 78,
            BC4_TYPELESS = 79,
            BC4_UNORM = 80,
            BC4_SNORM = 81,
            BC5_TYPELESS = 82,
            BC5_UNORM = 83,
            BC5_SNORM = 84,
            B5G6R5_UNORM = 85,
            B5G5R5A1_UNORM = 86,
            B8G8R8A8_UNORM = 87,
            B8G8R8X8_UNORM = 88,
            R10G10B10_XR_BIAS_A2_UNORM = 89,
            B8G8R8A8_TYPELESS = 90,
            B8G8R8A8_UNORM_SRGB = 91,
            B8G8R8X8_TYPELESS = 92,
            B8G8R8X8_UNORM_SRGB = 93,
            BC6H_TYPELESS = 94,
            BC6H_UF16 = 95,
            BC6H_SF16 = 96,
            BC7_TYPELESS = 97,
            BC7_UNORM = 98,
            BC7_UNORM_SRGB = 99,
            AYUV = 100,
            Y410 = 101,
            Y416 = 102,
            NV12 = 103,
            P010 = 104,
            P016 = 105,
            F420_OPAQUE = 106,
            YUY2 = 107,
            Y210 = 108,
            Y216 = 109,
            NV11 = 110,
            AI44 = 111,
            IA44 = 112,
            P8 = 113,
            A8P8 = 114,
            B4G4R4A4_UNORM = 115,

            P208 = 130,
            V208 = 131,
            V408 = 132,
        };

        enum class DepthBufferFormat : uint32_t
        {
            Depth32Float = static_cast<uint32_t>(GraphicsFormat::D32_FLOAT),
            Depth16Unorm = static_cast<uint32_t>(GraphicsFormat::D16_UNORM)
        };

        enum class DepthStencilBufferFormat : uint32_t
        {
            Depth24UnormS8Uint = static_cast<uint32_t> ( GraphicsFormat::D24_UNORM_S8_UINT )
        };

        enum class ColorBufferFormat : uint32_t
        {
            R8G8B8A8Unorm = static_cast<uint32_t>( GraphicsFormat::R8G8B8A8_UNORM )
        };

        enum class IndexBufferFormat : uint32_t
        {
            Bytes2 = static_cast<uint32_t>(GraphicsFormat::R16_UINT),
            Bytes4 = static_cast<uint32_t>(GraphicsFormat::R32_UINT)
        };

        enum class ResourceState : uint32_t
        {
            Common                      = 0,
            VertexAndConstantBuffer     = 0x1,
            IndexBuffer                 = 0x2,
            RenderTarget                = 0x4,
            UnorderedAccess             = 0x8,
            DepthWrite                  = 0x10,
            DepthRead                   = 0x20,
            NonPixelShaderResource      = 0x40,
            PixelShaderResource         = 0x80,
            StreamOut                   = 0x100,
            IndirectArgument            = 0x200,
            CopyDestination             = 0x400,
            CopySource                  = 0x800,
            ResolveDestination          = 0x1000,
            ResolveSource               = 0x2000,
            GenericRead                 = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
            Present                     = 0,
            Predication                 = 0x200
        };

        enum class InputClassification : uint32_t
        {
            PerVertexData   = 0,
            PerInstanceData = 1
        };

        enum class PrimitiveTopologyType : uint32_t
        {
            Undefined   = 0,
            Point       = 1,
            Line        = 2,
            Triangle    = 3,
            Patch       = 4
        };

        enum class PrimitiveTopology : uint32_t
        {
            LineList     = 2,
            TriangleList = 4
        };

        struct InputElementDescription
        {
            const char*         m_semanticName;
            uint32_t            m_semanticIndex;
            GraphicsFormat      m_format;
            uint32_t            m_inputSlot;
            uint32_t            m_alignedByteOffset;
            InputClassification m_inputSlotClass;
            uint32_t            m_instanceDataStepRate;
        };

        enum class FillMode : uint32_t
        {
            WireFrame = 2,
            Solid = 3
        };

        enum class CullMode : uint32_t
        {
            None    = 1,
            Front   = 2,
            Back    = 3
        };

        struct StreamOutputDeclarationEntry
        {
            uint32_t    m_stream;
            const char* m_semanticName;
            uint32_t    m_semanticIndex;
            uint8_t     m_startComponent;
            uint8_t     m_componentCount;
            uint8_t     m_outputSlot;
        };

        enum class ComparisonFunction : uint32_t
        {
            Never           = 1,
            Less            = 2,
            Equal           = 3,
            LessEqual       = 4,
            Greater         = 5,
            NotEqual        = 6,
            GreaterEqual    = 7,
            Always          = 8
        };

        enum class DepthWriteMask : uint32_t
        {
            Zero = 0,
            All = 1
        };

        enum class StencilOperation : uint32_t
        {
            Keep                = 1,
            Zero                = 2,
            Replace             = 3,
            IncrementSaturate   = 4,
            DecrementSaturate   = 5,
            Invert              = 6,
            Increment           = 7,
            Decrement           = 8
        };

        struct DepthStencilOperationDescription
        {
            StencilOperation    m_stencilFailOperation;
            StencilOperation    m_stencilDepthFailOperation;
            StencilOperation    m_stencilPassOperation;
            ComparisonFunction  m_stencilFunction;
        };

        struct DepthStencilDescription
        {
            bool                                m_depthEnable;
            DepthWriteMask                      m_depthWriteMask;
            ComparisonFunction                  m_depthFunc;
            bool                                m_stencilEnable;
            uint8_t                             m_stencilReadMask;
            uint8_t                             m_stencilWriteMask;
            DepthStencilOperationDescription    m_frontFace;
            DepthStencilOperationDescription    m_backFace;
        };

     
        enum class Blend : uint32_t
        {
            Zero = 1,
            One = 2,
            SourceColor = 3,
            InverseSourceColor = 4,
            SourceAlpha = 5,
            InverseSourceAlpha = 6,
            DestinationAlpha = 7,
            InverseDestinationAlpha = 8,
            DestinationColor = 9,
            InverseDestinationColor = 10,
            SourceAlphaSaturate = 11,
            Factor = 14,
            InverseBlendFactor = 15,
            SourceOneColor = 16,
            InverseSourceOneColor = 17,
            SourceOneAlpha = 18,
            InverseSourceOneAlpha = 19
        };
                
        enum class BlendOperation : uint32_t
        {
            Add = 1,
            Subtract = 2,
            ReverseSubtract = 3,
            Min = 4,
            Max = 5
        };

        enum class ColorWriteEnable : uint32_t
        {
            Red     = 1,
            Green   = 2,
            Blue    = 4,
            Alpha   = 8,
            ColorWriteEnable_All     = Red | Green | Blue | Alpha
        };

        enum class LogicOperation : uint32_t
        {
            Clear           = 0,
            Set             = 1,
            Copy            = 2,
            CopyInverted    = 3,
            Noop            = 4,
            Invert          = 5,
            And             = 6,
            Nand            = 7,
            Or              = 8,
            Nor             = 9,
            Xor             = 10,
            Equiv           = 11,
            AndReverse      = 12,
            AndInverted     = 13,
            OrReverse       = 14,
            OrInverted      = 15
        };

        struct RenderTargetBlendDescription
        {
            bool            m_blendEnable;
            bool            m_logicOperationEnable;
            Blend           m_sourceBlend;
            Blend           m_destinationBlend;
            BlendOperation  m_blendOperation;
            Blend           m_sourceBlendAlpha;
            Blend           m_destinationBlendAlpha;
            BlendOperation  m_blendOperationAlpha;
            LogicOperation  m_logicOperation;
            uint8_t         m_renderTargetWriteMask;
        };

        struct BlendDescription
        {
            bool m_alphaToCoverageEnable;
            bool m_independentBlendEnable;
            RenderTargetBlendDescription m_renderTarget[8];
        };

        enum class ConservativeRasterizationMode : uint32_t 
        {
            Off = 0,
            On  = 1
        };

        enum class IndexBufferStripCut : uint32_t
        {
            ValueDisabled       = 0,
            Value0xFFFF         = 1,
            Value0xFFFFFFFF     = 2
        };

        struct RasterizerDescription
        {
            FillMode                        m_fillMode;
            CullMode                        m_cullMode;
            bool                            m_frontCounterClockwise;
            int32_t                         m_depthBias;
            float                           m_depthBiasClamp;
            float                           m_slopeScaledDepthBias;
            bool                            m_depthClipEnable;
            bool                            m_multisampleEnable;
            bool                            m_antialiasedLineEnable;
            uint32_t                        m_forcedSampleCount;
            ConservativeRasterizationMode   m_conservativeRaster;
        };

        struct ShaderByteCode
        {
            const void* m_code      = 0;
            uint32_t    m_code_size = 0;
        };

        struct ComputeShaderByteCode : public ShaderByteCode
        {

        };

        struct VertexShaderByteCode : public ShaderByteCode
        {

        };

        struct PixelShaderByteCode : public ShaderByteCode
        {

        };

        struct DomainShaderByteCode : public ShaderByteCode
        {

        };

        struct GeometryShaderByteCode : public ShaderByteCode
        {

        };

        struct HullShaderByteCode : public ShaderByteCode
        {

        };

        struct RootSignatureByteCode : public ShaderByteCode
        {

        };

        struct StreamOutputDescription final
        {
            const StreamOutputDeclarationEntry* m_soDeclaration;
            uint32_t                         m_numEntries;
            const uint32_t*                  m_bufferStrides;
            uint32_t                         m_numStrides;
            uint32_t                         m_rasterizedStream;
        };

        struct InputLayoutDescription final
        {
            const InputElementDescription* m_inputElementDescs;
            uint32_t                m_numElements;
        };

        struct ComputePipelineStateDescription final
        {
            ComputeShaderByteCode   m_cs;
        };

        struct SampleDescription final
        {
            uint32_t m_count;
            uint32_t m_quality;
        };

        struct GraphicsPipelineStateDescription
        {
            VertexShaderByteCode        m_vs;
            PixelShaderByteCode         m_ps;
            DomainShaderByteCode        m_ds;
            HullShaderByteCode          m_hs;
            GeometryShaderByteCode      m_gs;

            StreamOutputDescription     m_streamOutput;
            BlendDescription            m_blendState;

            uint32_t                    m_sampleMask;
            RasterizerDescription       m_rasterizerState;
            DepthStencilDescription     m_depthStencilState;

            InputLayoutDescription      m_inputLayout;
            IndexBufferStripCut         m_ibStripCutValue;
            PrimitiveTopologyType       m_primitiveTopologyType;
            uint32_t                    m_numRenderTargets;
            GraphicsFormat              m_rtvFormats[8];
            GraphicsFormat              m_dsvFormat;
            SampleDescription           m_sampleDescription;
        };

        struct GpuVirtualAddress final
        {
            uint64_t m_value;

            GpuVirtualAddress( uint64_t v = 0ULL ) : m_value(v)
            {

            }

            operator uint64_t() const
            {
                return m_value;
            }
        };

        struct CpuVirtualAddress final
        {
            uint64_t m_value;

            CpuVirtualAddress(uint64_t v = 0ULL) : m_value(v)
            {

            }

            CpuVirtualAddress(const void* v) : m_value(reinterpret_cast<uint64_t>(v))
            {

            }

            operator uint64_t() const
            {
                return m_value;
            }

            operator void*() const
            {
                return reinterpret_cast<void*>(m_value);
            }
        };

        struct RootIndex final
        {
            uint32_t m_value;

            RootIndex(uint32_t v = 0) : m_value(v)
            {

            }

            operator uint32_t() const
            {
                return m_value;
            }
        };

        struct VertexBufferView
        {
            GpuVirtualAddress m_bufferLocation;
            uint32_t          m_sizeInBytes;
            uint32_t          m_strideInBytes;
        };

        enum class VertexSlot : uint32_t
        {
            Zero    = 0,
            One     = 1,
            Two     = 2,
            Three   = 3,
            Four    = 4,
            Five    = 5,
            Six     = 6
        };

        struct IndexBufferView
        {
            GpuVirtualAddress m_bufferLocation;
            uint32_t          m_sizeInBytes;
            IndexBufferFormat m_format;
        };

        struct SubresourceData final
        {
            CpuVirtualAddress m_data;
            uint64_t          m_rowPitch;
            uint64_t          m_slicePitch;
        };

        struct PrimitiveRange
        {
            uint32_t m_begin;
            uint32_t m_end;
        };

        struct PrimitiveRangeDescription final
        {
            PrimitiveRange* m_ranges;
            uint64_t        m_ranges_size;
        };
    }
}



