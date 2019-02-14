#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            struct dxgi_format2 : qi::symbols<char, DXGI_FORMAT>
            {
                dxgi_format2()
                {
                    this->add
                    ("UNKNOWN", DXGI_FORMAT_UNKNOWN)
                        ("R32G32B32A32_TYPELESS", DXGI_FORMAT_R32G32B32A32_TYPELESS)
                        ("R32G32B32A32_FLOAT", DXGI_FORMAT_R32G32B32A32_FLOAT)
                        ("R32G32B32A32_UINT", DXGI_FORMAT_R32G32B32A32_UINT)
                        ("R32G32B32A32_SINT", DXGI_FORMAT_R32G32B32A32_SINT)
                        ("R32G32B32_TYPELESS", DXGI_FORMAT_R32G32B32_TYPELESS)
                        ("R32G32B32_FLOAT", DXGI_FORMAT_R32G32B32_FLOAT)
                        ("R32G32B32_UINT", DXGI_FORMAT_R32G32B32_UINT)
                        ("R32G32B32_SINT", DXGI_FORMAT_R32G32B32_SINT)
                        ("R16G16B16A16_TYPELESS", DXGI_FORMAT_R16G16B16A16_TYPELESS)
                        ("R16G16B16A16_FLOAT", DXGI_FORMAT_R16G16B16A16_FLOAT)
                        ("R16G16B16A16_UNORM", DXGI_FORMAT_R16G16B16A16_UNORM)
                        ("R16G16B16A16_UINT", DXGI_FORMAT_R16G16B16A16_UINT)
                        ("R16G16B16A16_SNORM", DXGI_FORMAT_R16G16B16A16_SNORM)
                        ("R16G16B16A16_SINT", DXGI_FORMAT_R16G16B16A16_SINT)
                        ("R32G32_TYPELESS", DXGI_FORMAT_R32G32_TYPELESS)
                        ("R32G32_FLOAT", DXGI_FORMAT_R32G32_FLOAT)
                        ("R32G32_UINT", DXGI_FORMAT_R32G32_UINT)
                        ("R32G32_SINT", DXGI_FORMAT_R32G32_SINT)
                        ("R32G8X24_TYPELESS", DXGI_FORMAT_R32G8X24_TYPELESS)
                        ("D32_FLOAT_S8X24_UINT", DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
                        ("R32_FLOAT_X8X24_TYPELESS", DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS)
                        ("X32_TYPELESS_G8X24_UINT", DXGI_FORMAT_X32_TYPELESS_G8X24_UINT)
                        ("R10G10B10A2_TYPELESS", DXGI_FORMAT_R10G10B10A2_TYPELESS)
                        ("R10G10B10A2_UNORM", DXGI_FORMAT_R10G10B10A2_UNORM)
                        ("R10G10B10A2_UINT", DXGI_FORMAT_R10G10B10A2_UINT)
                        ("R11G11B10_FLOAT", DXGI_FORMAT_R11G11B10_FLOAT)
                        ("R8G8B8A8_TYPELESS", DXGI_FORMAT_R8G8B8A8_TYPELESS)
                        ("R8G8B8A8_UNORM", DXGI_FORMAT_R8G8B8A8_UNORM)
                        ("R8G8B8A8_UNORM_SRGB", DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
                        ("R8G8B8A8_UINT", DXGI_FORMAT_R8G8B8A8_UINT)
                        ("R8G8B8A8_SNORM", DXGI_FORMAT_R8G8B8A8_SNORM)
                        ("R8G8B8A8_SINT", DXGI_FORMAT_R8G8B8A8_SINT)
                        ("R16G16_TYPELESS", DXGI_FORMAT_R16G16_TYPELESS)
                        ("R16G16_FLOAT", DXGI_FORMAT_R16G16_FLOAT)
                        ("R16G16_UNORM", DXGI_FORMAT_R16G16_UNORM)
                        ("R16G16_UINT", DXGI_FORMAT_R16G16_UINT)
                        ("R16G16_SNORM", DXGI_FORMAT_R16G16_SNORM)
                        ("R16G16_SINT", DXGI_FORMAT_R16G16_SINT)
                        ("R32_TYPELESS", DXGI_FORMAT_R32_TYPELESS)
                        ("D32_FLOAT", DXGI_FORMAT_D32_FLOAT)
                        ("R32_FLOAT", DXGI_FORMAT_R32_FLOAT)
                        ("R32_UINT", DXGI_FORMAT_R32_UINT)
                        ("R32_SINT", DXGI_FORMAT_R32_SINT)
                        ("R24G8_TYPELESS", DXGI_FORMAT_R24G8_TYPELESS)
                        ("D24_UNORM_S8_UINT", DXGI_FORMAT_D24_UNORM_S8_UINT)
                        ("R24_UNORM_X8_TYPELESS", DXGI_FORMAT_R24_UNORM_X8_TYPELESS)
                        ("X24_TYPELESS_G8_UINT", DXGI_FORMAT_X24_TYPELESS_G8_UINT)
                        ("R8G8_TYPELESS", DXGI_FORMAT_R8G8_TYPELESS)
                        ("R8G8_UNORM", DXGI_FORMAT_R8G8_UNORM)
                        ("R8G8_UINT", DXGI_FORMAT_R8G8_UINT)
                        ("R8G8_SNORM", DXGI_FORMAT_R8G8_SNORM)
                        ("R8G8_SINT", DXGI_FORMAT_R8G8_SINT)
                        ("R16_TYPELESS", DXGI_FORMAT_R16_TYPELESS)
                        ("R16_FLOAT", DXGI_FORMAT_R16_FLOAT)
                        ("D16_UNORM", DXGI_FORMAT_D16_UNORM)
                        ("R16_UNORM", DXGI_FORMAT_R16_UNORM)
                        ("R16_UINT", DXGI_FORMAT_R16_UINT)
                        ("R16_SNORM", DXGI_FORMAT_R16_SNORM)
                        ("R16_SINT", DXGI_FORMAT_R16_SINT)
                        ("R8_TYPELESS", DXGI_FORMAT_R8_TYPELESS)
                        ("R8_UNORM", DXGI_FORMAT_R8_UNORM)
                        ("R8_UINT", DXGI_FORMAT_R8_UINT)
                        ("R8_SNORM", DXGI_FORMAT_R8_SNORM)
                        ("R8_SINT", DXGI_FORMAT_R8_SINT)
                        ("A8_UNORM", DXGI_FORMAT_A8_UNORM)
                        ("R1_UNORM", DXGI_FORMAT_R1_UNORM)
                        ("R9G9B9E5_SHAREDEXP", DXGI_FORMAT_R9G9B9E5_SHAREDEXP)
                        ("R8G8_B8G8_UNORM", DXGI_FORMAT_R8G8_B8G8_UNORM)
                        ("G8R8_G8B8_UNORM", DXGI_FORMAT_G8R8_G8B8_UNORM)
                        ("BC1_TYPELESS", DXGI_FORMAT_BC1_TYPELESS)
                        ("BC1_UNORM", DXGI_FORMAT_BC1_UNORM)
                        ("BC1_UNORM_SRGB", DXGI_FORMAT_BC1_UNORM_SRGB)
                        ("BC2_TYPELESS", DXGI_FORMAT_BC2_TYPELESS)
                        ("BC2_UNORM", DXGI_FORMAT_BC2_UNORM)
                        ("BC2_UNORM_SRGB", DXGI_FORMAT_BC2_UNORM_SRGB)
                        ("BC3_TYPELESS", DXGI_FORMAT_BC3_TYPELESS)
                        ("BC3_UNORM", DXGI_FORMAT_BC3_UNORM)
                        ("BC3_UNORM_SRGB", DXGI_FORMAT_BC3_UNORM_SRGB)
                        ("BC4_TYPELESS", DXGI_FORMAT_BC4_TYPELESS)
                        ("BC4_UNORM", DXGI_FORMAT_BC4_UNORM)
                        ("BC4_SNORM", DXGI_FORMAT_BC4_SNORM)
                        ("BC5_TYPELESS", DXGI_FORMAT_BC5_TYPELESS)
                        ("BC5_UNORM", DXGI_FORMAT_BC5_UNORM)
                        ("BC5_SNORM", DXGI_FORMAT_BC5_SNORM)
                        ("B5G6R5_UNORM", DXGI_FORMAT_B5G6R5_UNORM)
                        ("B5G5R5A1_UNORM", DXGI_FORMAT_B5G5R5A1_UNORM)
                        ("B8G8R8A8_UNORM", DXGI_FORMAT_B8G8R8A8_UNORM)
                        ("B8G8R8X8_UNORM", DXGI_FORMAT_B8G8R8X8_UNORM)
                        ("R10G10B10_XR_BIAS_A2_UNORM", DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM)
                        ("B8G8R8A8_TYPELESS", DXGI_FORMAT_B8G8R8A8_TYPELESS)
                        ("B8G8R8A8_UNORM_SRGB", DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
                        ("B8G8R8X8_TYPELESS", DXGI_FORMAT_B8G8R8X8_TYPELESS)
                        ("B8G8R8X8_UNORM_SRGB", DXGI_FORMAT_B8G8R8X8_UNORM_SRGB)
                        ("BC6H_TYPELESS", DXGI_FORMAT_BC6H_TYPELESS)
                        ("BC6H_UF16", DXGI_FORMAT_BC6H_UF16)
                        ("BC6H_SF16", DXGI_FORMAT_BC6H_SF16)
                        ("BC7_TYPELESS", DXGI_FORMAT_BC7_TYPELESS)
                        ("BC7_UNORM", DXGI_FORMAT_BC7_UNORM)
                        ("BC7_UNORM_SRGB", DXGI_FORMAT_BC7_UNORM_SRGB)
                        ("AYUV", DXGI_FORMAT_AYUV)
                        ("Y410", DXGI_FORMAT_Y410)
                        ("Y416", DXGI_FORMAT_Y416)
                        ("NV12", DXGI_FORMAT_NV12)
                        ("010", DXGI_FORMAT_P010)
                        ("P016", DXGI_FORMAT_P016)
                        ("420_OPAQUE", DXGI_FORMAT_420_OPAQUE)
                        ("YUY2", DXGI_FORMAT_YUY2)
                        ("Y210", DXGI_FORMAT_Y210)
                        ("Y216", DXGI_FORMAT_Y216)
                        ("NV11", DXGI_FORMAT_NV11)
                        ("AI44", DXGI_FORMAT_AI44)
                        ("IA44", DXGI_FORMAT_IA44)
                        ("P8", DXGI_FORMAT_P8)
                        ("A8P8", DXGI_FORMAT_A8P8)
                        ("B4G4R4A4_UNORM", DXGI_FORMAT_B4G4R4A4_UNORM)
                        ("P208", DXGI_FORMAT_P208)
                        ("V208", DXGI_FORMAT_V208)
                        ("V408", DXGI_FORMAT_V408)
                        ;

                    this->name("dxgi_format2");
                }
            };

            template <typename iterator >
            struct named_dxgi_format : qi::grammar< iterator, DXGI_FORMAT(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, DXGI_FORMAT(), ascii::space_type  >;
            public:
                named_dxgi_format(const std::string& name) : base(m_start, "dxgi_format")
                {
                    qi::lit_type    lit;
                    m_start = lit(name) > '=' > m_value;
                }

                dxgi_format2                                                m_value;
                qi::rule<iterator, DXGI_FORMAT(), ascii::space_type >       m_start;
            };

            template <typename iterator >
            struct dxgi_format : named_dxgi_format<iterator>
            {
                using base = named_dxgi_format<iterator>;

                public:
                dxgi_format() : base(".Format")
                {
                }
            };
        }
    }
}


