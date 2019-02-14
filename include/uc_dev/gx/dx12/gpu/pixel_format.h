#pragma once

#include <assert.h>
#include <d3d12.h>
#include <algorithm>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            //removes the types from the formats
            inline DXGI_FORMAT get_base_format(DXGI_FORMAT defaultFormat)
            {
                switch (defaultFormat)
                {
                case DXGI_FORMAT_R8G8B8A8_UNORM:
                case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                    return DXGI_FORMAT_R8G8B8A8_TYPELESS;

                case DXGI_FORMAT_B8G8R8A8_UNORM:
                case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                    return DXGI_FORMAT_B8G8R8A8_TYPELESS;

                case DXGI_FORMAT_B8G8R8X8_UNORM:
                case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                    return DXGI_FORMAT_B8G8R8X8_TYPELESS;

                    // 32-bit Z w/ Stencil
                case DXGI_FORMAT_R32G8X24_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    return DXGI_FORMAT_R32G8X24_TYPELESS;

                    // No Stencil
                case DXGI_FORMAT_R32_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT:
                case DXGI_FORMAT_R32_FLOAT:
                    return DXGI_FORMAT_R32_TYPELESS;

                    // 24-bit Z
                case DXGI_FORMAT_R24G8_TYPELESS:
                case DXGI_FORMAT_D24_UNORM_S8_UINT:
                case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    return DXGI_FORMAT_R24G8_TYPELESS;

                    // 16-bit Z w/o Stencil
                case DXGI_FORMAT_R16_TYPELESS:
                case DXGI_FORMAT_D16_UNORM:
                case DXGI_FORMAT_R16_UNORM:
                    return DXGI_FORMAT_R16_TYPELESS;

                default:
                    return defaultFormat;
                }
            }

            //given format, returns format that allows in uav writing
            inline DXGI_FORMAT get_uav_format(DXGI_FORMAT defaultFormat)
            {
                switch (defaultFormat)
                {
                    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                    case DXGI_FORMAT_R8G8B8A8_UNORM:
                    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                    return DXGI_FORMAT_R8G8B8A8_UNORM;

                    case DXGI_FORMAT_R32_TYPELESS:
                    case DXGI_FORMAT_R32_FLOAT:
                    return DXGI_FORMAT_R32_FLOAT;

#ifdef _DEBUG
                case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                case DXGI_FORMAT_B8G8R8X8_UNORM:
                case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                case DXGI_FORMAT_B8G8R8A8_UNORM:
                case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                case DXGI_FORMAT_R32G8X24_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                case DXGI_FORMAT_D32_FLOAT:
                case DXGI_FORMAT_R24G8_TYPELESS:
                case DXGI_FORMAT_D24_UNORM_S8_UINT:
                case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                case DXGI_FORMAT_D16_UNORM:
                    assert(false);
#endif
                default:
                    return defaultFormat;
                }
            }

            inline DXGI_FORMAT get_dsv_format(DXGI_FORMAT defaultFormat)
            {
                switch (defaultFormat)
                {
                    // 32-bit Z w/ Stencil
                case DXGI_FORMAT_R32G8X24_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

                    // No Stencil
                case DXGI_FORMAT_R32_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT:
                case DXGI_FORMAT_R32_FLOAT:
                    return DXGI_FORMAT_D32_FLOAT;

                    // 24-bit Z
                case DXGI_FORMAT_R24G8_TYPELESS:
                case DXGI_FORMAT_D24_UNORM_S8_UINT:
                case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    return DXGI_FORMAT_D24_UNORM_S8_UINT;

                    // 16-bit Z w/o Stencil
                case DXGI_FORMAT_R16_TYPELESS:
                case DXGI_FORMAT_D16_UNORM:
                case DXGI_FORMAT_R16_UNORM:
                    return DXGI_FORMAT_D16_UNORM;

                default:
                    return defaultFormat;
                }
            }

            inline DXGI_FORMAT get_depth_format(DXGI_FORMAT defaultFormat)
            {
                switch (defaultFormat)
                {
                    // 32-bit Z w/ Stencil
                case DXGI_FORMAT_R32G8X24_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

                    // No Stencil
                case DXGI_FORMAT_R32_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT:
                case DXGI_FORMAT_R32_FLOAT:
                    return DXGI_FORMAT_R32_FLOAT;

                    // 24-bit Z
                case DXGI_FORMAT_R24G8_TYPELESS:
                case DXGI_FORMAT_D24_UNORM_S8_UINT:
                case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

                    // 16-bit Z w/o Stencil
                case DXGI_FORMAT_R16_TYPELESS:
                case DXGI_FORMAT_D16_UNORM:
                case DXGI_FORMAT_R16_UNORM:
                    return DXGI_FORMAT_R16_UNORM;

                default:
                    return DXGI_FORMAT_UNKNOWN;
                }
            }

            inline DXGI_FORMAT get_stencil_format(DXGI_FORMAT defaultFormat)
            {
                switch (defaultFormat)
                {
                    // 32-bit Z w/ Stencil
                case DXGI_FORMAT_R32G8X24_TYPELESS:
                case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

                    // 24-bit Z
                case DXGI_FORMAT_R24G8_TYPELESS:
                case DXGI_FORMAT_D24_UNORM_S8_UINT:
                case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

                default:
                    return DXGI_FORMAT_UNKNOWN;
                }
            }

            //-------------------------------------------------------------------------------------
            inline bool is_compressed_format(DXGI_FORMAT fmt)
            {
                switch (fmt)
                {
                    case DXGI_FORMAT_BC1_TYPELESS:
                    case DXGI_FORMAT_BC1_UNORM:
                    case DXGI_FORMAT_BC1_UNORM_SRGB:
                    case DXGI_FORMAT_BC2_TYPELESS:
                    case DXGI_FORMAT_BC2_UNORM:
                    case DXGI_FORMAT_BC2_UNORM_SRGB:
                    case DXGI_FORMAT_BC3_TYPELESS:
                    case DXGI_FORMAT_BC3_UNORM:
                    case DXGI_FORMAT_BC3_UNORM_SRGB:
                    case DXGI_FORMAT_BC4_TYPELESS:
                    case DXGI_FORMAT_BC4_UNORM:
                    case DXGI_FORMAT_BC4_SNORM:
                    case DXGI_FORMAT_BC5_TYPELESS:
                    case DXGI_FORMAT_BC5_UNORM:
                    case DXGI_FORMAT_BC5_SNORM:
                    case DXGI_FORMAT_BC6H_TYPELESS:
                    case DXGI_FORMAT_BC6H_UF16:
                    case DXGI_FORMAT_BC6H_SF16:
                    case DXGI_FORMAT_BC7_TYPELESS:
                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                        return true;

                    default:
                        return false;
                }
            }
            //-------------------------------------------------------------------------------------
            inline bool is_block_format(DXGI_FORMAT f)
            {
                switch (f)
                {
                    case DXGI_FORMAT_BC1_TYPELESS:
                    case DXGI_FORMAT_BC1_UNORM:
                    case DXGI_FORMAT_BC1_UNORM_SRGB:
                    case DXGI_FORMAT_BC2_TYPELESS:
                    case DXGI_FORMAT_BC2_UNORM:
                    case DXGI_FORMAT_BC2_UNORM_SRGB:
                    case DXGI_FORMAT_BC3_TYPELESS:
                    case DXGI_FORMAT_BC3_UNORM:
                    case DXGI_FORMAT_BC3_UNORM_SRGB:
                    case DXGI_FORMAT_BC4_TYPELESS:
                    case DXGI_FORMAT_BC4_UNORM:
                    case DXGI_FORMAT_BC4_SNORM:
                    case DXGI_FORMAT_BC5_TYPELESS:
                    case DXGI_FORMAT_BC5_UNORM:
                    case DXGI_FORMAT_BC5_SNORM:
                    case DXGI_FORMAT_BC6H_TYPELESS:
                    case DXGI_FORMAT_BC6H_UF16:
                    case DXGI_FORMAT_BC6H_SF16:
                    case DXGI_FORMAT_BC7_TYPELESS:
                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                    default: return false;
                }
            }   
            //-------------------------------------------------------------------------------------
            inline bool is_packed_format(DXGI_FORMAT f)
            {
                switch (static_cast<int>(f))
                {
                    case DXGI_FORMAT_R8G8_B8G8_UNORM:
                    case DXGI_FORMAT_G8R8_G8B8_UNORM:
                    case DXGI_FORMAT_YUY2: // 4:2:2 8-bit
                    case DXGI_FORMAT_Y210: // 4:2:2 10-bit
                    case DXGI_FORMAT_Y216: // 4:2:2 16-bit
                        return true;

                    default:
                        return false;
                }
            }
            //-------------------------------------------------------------------------------------
            inline bool is_video_format(DXGI_FORMAT f)
            {
                switch (static_cast<int>(f))
                {
                    case DXGI_FORMAT_AYUV:
                    case DXGI_FORMAT_Y410:
                    case DXGI_FORMAT_Y416:
                    case DXGI_FORMAT_NV12:
                    case DXGI_FORMAT_P010:
                    case DXGI_FORMAT_P016:
                    case DXGI_FORMAT_YUY2:
                    case DXGI_FORMAT_Y210:
                    case DXGI_FORMAT_Y216:
                    case DXGI_FORMAT_NV11:
                        // These video formats can be used with the 3D pipeline through special view mappings

                    case DXGI_FORMAT_420_OPAQUE:
                    case DXGI_FORMAT_AI44:
                    case DXGI_FORMAT_IA44:
                    case DXGI_FORMAT_P8:
                    case DXGI_FORMAT_A8P8:
                        // These are limited use video formats not usable in any way by the 3D pipeline

                    case DXGI_FORMAT_P208:
                    case DXGI_FORMAT_V208:
                    case DXGI_FORMAT_V408:
                        // These video formats are for JPEG Hardware decode (DXGI 1.4)
                        return true;

                    default:
                        return false;
                }
            }

            #define XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT DXGI_FORMAT(116)
            #define XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT DXGI_FORMAT(117)
            #define XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT DXGI_FORMAT(118)
            #define XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS DXGI_FORMAT(119)
            #define XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT DXGI_FORMAT(120)

            #ifndef XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM
            #define XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM DXGI_FORMAT(189)
            #endif

            #define XBOX_DXGI_FORMAT_R4G4_UNORM DXGI_FORMAT(190)

            inline bool is_planar_format(DXGI_FORMAT f)
            {
                switch (static_cast<int>(f))
                {

                    case DXGI_FORMAT_NV12:      // 4:2:0 8-bit
                    case DXGI_FORMAT_P010:      // 4:2:0 10-bit
                    case DXGI_FORMAT_P016:      // 4:2:0 16-bit
                    case DXGI_FORMAT_420_OPAQUE:// 4:2:0 8-bit
                    case DXGI_FORMAT_NV11:      // 4:1:1 8-bit

                    case DXGI_FORMAT_P208: // 4:2:2 8-bit
                    case DXGI_FORMAT_V208: // 4:4:0 8-bit
                    case DXGI_FORMAT_V408: // 4:4:4 8-bit
                                                 // These are JPEG Hardware decode formats (DXGI 1.4)

                    case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
                    case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                    case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                        // These are Xbox One platform specific types
                        return true;

                    default:
                        return false;
                }
            }
            //-------------------------------------------------------------------------------------
            inline bool is_depth_stencil_format(DXGI_FORMAT f)
            {
                switch (static_cast<int>(f))
                {
                    case DXGI_FORMAT_R32G8X24_TYPELESS:
                    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    case DXGI_FORMAT_D32_FLOAT:
                    case DXGI_FORMAT_R24G8_TYPELESS:
                    case DXGI_FORMAT_D24_UNORM_S8_UINT:
                    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    case DXGI_FORMAT_D16_UNORM:
                    case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
                    case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                    case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                        return true;

                    default:
                        return false;
                }
            }

            //-------------------------------------------------------------------------------------
            inline bool is_typless_format(DXGI_FORMAT fmt, bool partial_typeless)
            {
                switch (static_cast<int>(fmt))
                {
                    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
                    case DXGI_FORMAT_R32G32B32_TYPELESS:
                    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
                    case DXGI_FORMAT_R32G32_TYPELESS:
                    case DXGI_FORMAT_R32G8X24_TYPELESS:
                    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
                    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                    case DXGI_FORMAT_R16G16_TYPELESS:
                    case DXGI_FORMAT_R32_TYPELESS:
                    case DXGI_FORMAT_R24G8_TYPELESS:
                    case DXGI_FORMAT_R8G8_TYPELESS:
                    case DXGI_FORMAT_R16_TYPELESS:
                    case DXGI_FORMAT_R8_TYPELESS:
                    case DXGI_FORMAT_BC1_TYPELESS:
                    case DXGI_FORMAT_BC2_TYPELESS:
                    case DXGI_FORMAT_BC3_TYPELESS:
                    case DXGI_FORMAT_BC4_TYPELESS:
                    case DXGI_FORMAT_BC5_TYPELESS:
                    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                    case DXGI_FORMAT_BC6H_TYPELESS:
                    case DXGI_FORMAT_BC7_TYPELESS:
                        return true;

                    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                    case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                        return partial_typeless;

                    default:
                        return false;
                }
            }

            //-------------------------------------------------------------------------------------
            inline bool has_alpha_format(DXGI_FORMAT f)
            {
                switch (static_cast<int>(f))
                {
                    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
                    case DXGI_FORMAT_R32G32B32A32_FLOAT:
                    case DXGI_FORMAT_R32G32B32A32_UINT:
                    case DXGI_FORMAT_R32G32B32A32_SINT:
                    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
                    case DXGI_FORMAT_R16G16B16A16_FLOAT:
                    case DXGI_FORMAT_R16G16B16A16_UNORM:
                    case DXGI_FORMAT_R16G16B16A16_UINT:
                    case DXGI_FORMAT_R16G16B16A16_SNORM:
                    case DXGI_FORMAT_R16G16B16A16_SINT:
                    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
                    case DXGI_FORMAT_R10G10B10A2_UNORM:
                    case DXGI_FORMAT_R10G10B10A2_UINT:
                    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                    case DXGI_FORMAT_R8G8B8A8_UNORM:
                    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                    case DXGI_FORMAT_R8G8B8A8_UINT:
                    case DXGI_FORMAT_R8G8B8A8_SNORM:
                    case DXGI_FORMAT_R8G8B8A8_SINT:
                    case DXGI_FORMAT_A8_UNORM:
                    case DXGI_FORMAT_BC1_TYPELESS:
                    case DXGI_FORMAT_BC1_UNORM:
                    case DXGI_FORMAT_BC1_UNORM_SRGB:
                    case DXGI_FORMAT_BC2_TYPELESS:
                    case DXGI_FORMAT_BC2_UNORM:
                    case DXGI_FORMAT_BC2_UNORM_SRGB:
                    case DXGI_FORMAT_BC3_TYPELESS:
                    case DXGI_FORMAT_BC3_UNORM:
                    case DXGI_FORMAT_BC3_UNORM_SRGB:
                    case DXGI_FORMAT_B5G5R5A1_UNORM:
                    case DXGI_FORMAT_B8G8R8A8_UNORM:
                    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
                    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                    case DXGI_FORMAT_BC7_TYPELESS:
                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                    case DXGI_FORMAT_AYUV:
                    case DXGI_FORMAT_Y410:
                    case DXGI_FORMAT_Y416:
                    case DXGI_FORMAT_AI44:
                    case DXGI_FORMAT_IA44:
                    case DXGI_FORMAT_A8P8:
                    case DXGI_FORMAT_B4G4R4A4_UNORM:
                    case XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
                        return true;

                    default:
                        return false;
                }
            }
            //-------------------------------------------------------------------------------------
            inline uint32_t bits_per_pixel(DXGI_FORMAT fmt)
            {
                switch (static_cast<int>(fmt))
                {
                    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
                    case DXGI_FORMAT_R32G32B32A32_FLOAT:
                    case DXGI_FORMAT_R32G32B32A32_UINT:
                    case DXGI_FORMAT_R32G32B32A32_SINT:
                        return 128U;

                    case DXGI_FORMAT_R32G32B32_TYPELESS:
                    case DXGI_FORMAT_R32G32B32_FLOAT:
                    case DXGI_FORMAT_R32G32B32_UINT:
                    case DXGI_FORMAT_R32G32B32_SINT:
                        return 96U;

                    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
                    case DXGI_FORMAT_R16G16B16A16_FLOAT:
                    case DXGI_FORMAT_R16G16B16A16_UNORM:
                    case DXGI_FORMAT_R16G16B16A16_UINT:
                    case DXGI_FORMAT_R16G16B16A16_SNORM:
                    case DXGI_FORMAT_R16G16B16A16_SINT:
                    case DXGI_FORMAT_R32G32_TYPELESS:
                    case DXGI_FORMAT_R32G32_FLOAT:
                    case DXGI_FORMAT_R32G32_UINT:
                    case DXGI_FORMAT_R32G32_SINT:
                    case DXGI_FORMAT_R32G8X24_TYPELESS:
                    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    case DXGI_FORMAT_Y416:
                    case DXGI_FORMAT_Y210:
                    case DXGI_FORMAT_Y216:
                        return 64U;

                    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
                    case DXGI_FORMAT_R10G10B10A2_UNORM:
                    case DXGI_FORMAT_R10G10B10A2_UINT:
                    case DXGI_FORMAT_R11G11B10_FLOAT:
                    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                    case DXGI_FORMAT_R8G8B8A8_UNORM:
                    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                    case DXGI_FORMAT_R8G8B8A8_UINT:
                    case DXGI_FORMAT_R8G8B8A8_SNORM:
                    case DXGI_FORMAT_R8G8B8A8_SINT:
                    case DXGI_FORMAT_R16G16_TYPELESS:
                    case DXGI_FORMAT_R16G16_FLOAT:
                    case DXGI_FORMAT_R16G16_UNORM:
                    case DXGI_FORMAT_R16G16_UINT:
                    case DXGI_FORMAT_R16G16_SNORM:
                    case DXGI_FORMAT_R16G16_SINT:
                    case DXGI_FORMAT_R32_TYPELESS:
                    case DXGI_FORMAT_D32_FLOAT:
                    case DXGI_FORMAT_R32_FLOAT:
                    case DXGI_FORMAT_R32_UINT:
                    case DXGI_FORMAT_R32_SINT:
                    case DXGI_FORMAT_R24G8_TYPELESS:
                    case DXGI_FORMAT_D24_UNORM_S8_UINT:
                    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
                    case DXGI_FORMAT_R8G8_B8G8_UNORM:
                    case DXGI_FORMAT_G8R8_G8B8_UNORM:
                    case DXGI_FORMAT_B8G8R8A8_UNORM:
                    case DXGI_FORMAT_B8G8R8X8_UNORM:
                    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
                    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                    case DXGI_FORMAT_AYUV:
                    case DXGI_FORMAT_Y410:
                    case DXGI_FORMAT_YUY2:
                    case XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
                        return 32U;

                    case DXGI_FORMAT_P010:
                    case DXGI_FORMAT_P016:
                    case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
                    case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                    case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                    case DXGI_FORMAT_V408:
                        return 24U;

                    case DXGI_FORMAT_R8G8_TYPELESS:
                    case DXGI_FORMAT_R8G8_UNORM:
                    case DXGI_FORMAT_R8G8_UINT:
                    case DXGI_FORMAT_R8G8_SNORM:
                    case DXGI_FORMAT_R8G8_SINT:
                    case DXGI_FORMAT_R16_TYPELESS:
                    case DXGI_FORMAT_R16_FLOAT:
                    case DXGI_FORMAT_D16_UNORM:
                    case DXGI_FORMAT_R16_UNORM:
                    case DXGI_FORMAT_R16_UINT:
                    case DXGI_FORMAT_R16_SNORM:
                    case DXGI_FORMAT_R16_SINT:
                    case DXGI_FORMAT_B5G6R5_UNORM:
                    case DXGI_FORMAT_B5G5R5A1_UNORM:
                    case DXGI_FORMAT_A8P8:
                    case DXGI_FORMAT_B4G4R4A4_UNORM:
                    case DXGI_FORMAT_P208:
                    case DXGI_FORMAT_V208:
                        return 16U;

                    case DXGI_FORMAT_NV12:
                    case DXGI_FORMAT_420_OPAQUE:
                    case DXGI_FORMAT_NV11:
                        return 12U;

                    case DXGI_FORMAT_R8_TYPELESS:
                    case DXGI_FORMAT_R8_UNORM:
                    case DXGI_FORMAT_R8_UINT:
                    case DXGI_FORMAT_R8_SNORM:
                    case DXGI_FORMAT_R8_SINT:
                    case DXGI_FORMAT_A8_UNORM:
                    case DXGI_FORMAT_AI44:
                    case DXGI_FORMAT_IA44:
                    case DXGI_FORMAT_P8:
                    case XBOX_DXGI_FORMAT_R4G4_UNORM:
                        return 8U;

                    case DXGI_FORMAT_R1_UNORM:
                        return 1U;

                    case DXGI_FORMAT_BC1_TYPELESS:
                    case DXGI_FORMAT_BC1_UNORM:
                    case DXGI_FORMAT_BC1_UNORM_SRGB:
                    case DXGI_FORMAT_BC4_TYPELESS:
                    case DXGI_FORMAT_BC4_UNORM:
                    case DXGI_FORMAT_BC4_SNORM:
                        return 4U;

                    case DXGI_FORMAT_BC2_TYPELESS:
                    case DXGI_FORMAT_BC2_UNORM:
                    case DXGI_FORMAT_BC2_UNORM_SRGB:
                    case DXGI_FORMAT_BC3_TYPELESS:
                    case DXGI_FORMAT_BC3_UNORM:
                    case DXGI_FORMAT_BC3_UNORM_SRGB:
                    case DXGI_FORMAT_BC5_TYPELESS:
                    case DXGI_FORMAT_BC5_UNORM:
                    case DXGI_FORMAT_BC5_SNORM:
                    case DXGI_FORMAT_BC6H_TYPELESS:
                    case DXGI_FORMAT_BC6H_UF16:
                    case DXGI_FORMAT_BC6H_SF16:
                    case DXGI_FORMAT_BC7_TYPELESS:
                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                        return 8U;

                    default:
                        return 0U;
                }
            }
            //-------------------------------------------------------------------------------------
            inline uint32_t bits_per_color(DXGI_FORMAT fmt)
            {
                switch (static_cast<int>(fmt))
                {
                    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
                    case DXGI_FORMAT_R32G32B32A32_FLOAT:
                    case DXGI_FORMAT_R32G32B32A32_UINT:
                    case DXGI_FORMAT_R32G32B32A32_SINT:
                    case DXGI_FORMAT_R32G32B32_TYPELESS:
                    case DXGI_FORMAT_R32G32B32_FLOAT:
                    case DXGI_FORMAT_R32G32B32_UINT:
                    case DXGI_FORMAT_R32G32B32_SINT:
                    case DXGI_FORMAT_R32G32_TYPELESS:
                    case DXGI_FORMAT_R32G32_FLOAT:
                    case DXGI_FORMAT_R32G32_UINT:
                    case DXGI_FORMAT_R32G32_SINT:
                    case DXGI_FORMAT_R32G8X24_TYPELESS:
                    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
                    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
                    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
                    case DXGI_FORMAT_R32_TYPELESS:
                    case DXGI_FORMAT_D32_FLOAT:
                    case DXGI_FORMAT_R32_FLOAT:
                    case DXGI_FORMAT_R32_UINT:
                    case DXGI_FORMAT_R32_SINT:
                        return 32U;

                    case DXGI_FORMAT_R24G8_TYPELESS:
                    case DXGI_FORMAT_D24_UNORM_S8_UINT:
                    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
                    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
                        return 24U;

                    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
                    case DXGI_FORMAT_R16G16B16A16_FLOAT:
                    case DXGI_FORMAT_R16G16B16A16_UNORM:
                    case DXGI_FORMAT_R16G16B16A16_UINT:
                    case DXGI_FORMAT_R16G16B16A16_SNORM:
                    case DXGI_FORMAT_R16G16B16A16_SINT:
                    case DXGI_FORMAT_R16G16_TYPELESS:
                    case DXGI_FORMAT_R16G16_FLOAT:
                    case DXGI_FORMAT_R16G16_UNORM:
                    case DXGI_FORMAT_R16G16_UINT:
                    case DXGI_FORMAT_R16G16_SNORM:
                    case DXGI_FORMAT_R16G16_SINT:
                    case DXGI_FORMAT_R16_TYPELESS:
                    case DXGI_FORMAT_R16_FLOAT:
                    case DXGI_FORMAT_D16_UNORM:
                    case DXGI_FORMAT_R16_UNORM:
                    case DXGI_FORMAT_R16_UINT:
                    case DXGI_FORMAT_R16_SNORM:
                    case DXGI_FORMAT_R16_SINT:
                    case DXGI_FORMAT_BC6H_TYPELESS:
                    case DXGI_FORMAT_BC6H_UF16:
                    case DXGI_FORMAT_BC6H_SF16:
                    case DXGI_FORMAT_Y416:
                    case DXGI_FORMAT_P016:
                    case DXGI_FORMAT_Y216:
                    case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
                    case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                    case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                        return 16U;

                    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
                        return 14U;

                    case DXGI_FORMAT_R11G11B10_FLOAT:
                        return 11U;

                    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
                    case DXGI_FORMAT_R10G10B10A2_UNORM:
                    case DXGI_FORMAT_R10G10B10A2_UINT:
                    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
                    case DXGI_FORMAT_Y410:
                    case DXGI_FORMAT_P010:
                    case DXGI_FORMAT_Y210:
                    case XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
                        return 10U;

                    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                    case DXGI_FORMAT_R8G8B8A8_UNORM:
                    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                    case DXGI_FORMAT_R8G8B8A8_UINT:
                    case DXGI_FORMAT_R8G8B8A8_SNORM:
                    case DXGI_FORMAT_R8G8B8A8_SINT:
                    case DXGI_FORMAT_R8G8_TYPELESS:
                    case DXGI_FORMAT_R8G8_UNORM:
                    case DXGI_FORMAT_R8G8_UINT:
                    case DXGI_FORMAT_R8G8_SNORM:
                    case DXGI_FORMAT_R8G8_SINT:
                    case DXGI_FORMAT_R8_TYPELESS:
                    case DXGI_FORMAT_R8_UNORM:
                    case DXGI_FORMAT_R8_UINT:
                    case DXGI_FORMAT_R8_SNORM:
                    case DXGI_FORMAT_R8_SINT:
                    case DXGI_FORMAT_A8_UNORM:
                    case DXGI_FORMAT_R8G8_B8G8_UNORM:
                    case DXGI_FORMAT_G8R8_G8B8_UNORM:
                    case DXGI_FORMAT_BC4_TYPELESS:
                    case DXGI_FORMAT_BC4_UNORM:
                    case DXGI_FORMAT_BC4_SNORM:
                    case DXGI_FORMAT_BC5_TYPELESS:
                    case DXGI_FORMAT_BC5_UNORM:
                    case DXGI_FORMAT_BC5_SNORM:
                    case DXGI_FORMAT_B8G8R8A8_UNORM:
                    case DXGI_FORMAT_B8G8R8X8_UNORM:
                    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                    case DXGI_FORMAT_AYUV:
                    case DXGI_FORMAT_NV12:
                    case DXGI_FORMAT_420_OPAQUE:
                    case DXGI_FORMAT_YUY2:
                    case DXGI_FORMAT_NV11:
                    case DXGI_FORMAT_P208:
                    case DXGI_FORMAT_V208:
                    case DXGI_FORMAT_V408:
                        return 8U;

                    case DXGI_FORMAT_BC7_TYPELESS:
                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                        return 7U;

                    case DXGI_FORMAT_BC1_TYPELESS:
                    case DXGI_FORMAT_BC1_UNORM:
                    case DXGI_FORMAT_BC1_UNORM_SRGB:
                    case DXGI_FORMAT_BC2_TYPELESS:
                    case DXGI_FORMAT_BC2_UNORM:
                    case DXGI_FORMAT_BC2_UNORM_SRGB:
                    case DXGI_FORMAT_BC3_TYPELESS:
                    case DXGI_FORMAT_BC3_UNORM:
                    case DXGI_FORMAT_BC3_UNORM_SRGB:
                    case DXGI_FORMAT_B5G6R5_UNORM:
                        return 6U;

                    case DXGI_FORMAT_B5G5R5A1_UNORM:
                        return 5U;

                    case DXGI_FORMAT_B4G4R4A4_UNORM:
                    case XBOX_DXGI_FORMAT_R4G4_UNORM:
                        return 4U;

                    case DXGI_FORMAT_R1_UNORM:
                        return 1U;

                    case DXGI_FORMAT_AI44:
                    case DXGI_FORMAT_IA44:
                    case DXGI_FORMAT_P8:
                    case DXGI_FORMAT_A8P8:
                        
                    // Palette formats return 0 for this function
                    default:
                        return 0U;
                    }
                }
                //-------------------------------------------------------------------------------------
                inline void row_slice_pitch(DXGI_FORMAT fmt, uint32_t width, uint32_t height, uint32_t& row_pitch, uint32_t& slice_pitch)
                {
                    switch (static_cast<int>(fmt))
                    {
                        case DXGI_FORMAT_BC1_TYPELESS:
                        case DXGI_FORMAT_BC1_UNORM:
                        case DXGI_FORMAT_BC1_UNORM_SRGB:
                        case DXGI_FORMAT_BC4_TYPELESS:
                        case DXGI_FORMAT_BC4_UNORM:
                        case DXGI_FORMAT_BC4_SNORM:
                            assert(is_compressed_format(fmt));
                            {
                                auto nbw = std::max<uint32_t>(1U, (width + 3U) / 4U);
                                auto nbh = std::max<uint32_t>(1U, (height + 3U) / 4U);
                                row_pitch = nbw * 8;
                                slice_pitch = row_pitch * nbh;
                            }
                            break;

                        case DXGI_FORMAT_BC2_TYPELESS:
                        case DXGI_FORMAT_BC2_UNORM:
                        case DXGI_FORMAT_BC2_UNORM_SRGB:
                        case DXGI_FORMAT_BC3_TYPELESS:
                        case DXGI_FORMAT_BC3_UNORM:
                        case DXGI_FORMAT_BC3_UNORM_SRGB:
                        case DXGI_FORMAT_BC5_TYPELESS:
                        case DXGI_FORMAT_BC5_UNORM:
                        case DXGI_FORMAT_BC5_SNORM:
                        case DXGI_FORMAT_BC6H_TYPELESS:
                        case DXGI_FORMAT_BC6H_UF16:
                        case DXGI_FORMAT_BC6H_SF16:
                        case DXGI_FORMAT_BC7_TYPELESS:
                        case DXGI_FORMAT_BC7_UNORM:
                        case DXGI_FORMAT_BC7_UNORM_SRGB:
                            assert(is_compressed_format(fmt));
                            {
                                auto nbw    = std::max<uint32_t>(1U, (width + 3U) / 4U);
                                auto nbh    = std::max<uint32_t>(1U, (height + 3U) / 4U);
                                row_pitch   = nbw * 16;
                                slice_pitch = row_pitch * nbh;
                            }
                            break;

                        case DXGI_FORMAT_R8G8_B8G8_UNORM:
                        case DXGI_FORMAT_G8R8_G8B8_UNORM:
                        case DXGI_FORMAT_YUY2:
                            assert(is_packed_format(fmt));
                            {
                                row_pitch = ((width + 1U) >> 1U) * 4U;
                                slice_pitch = row_pitch * height;
                            }
                            break;

                        case DXGI_FORMAT_Y210:
                        case DXGI_FORMAT_Y216:
                            assert(is_packed_format(fmt));
                            {
                                row_pitch = ((width + 1U) >> 1U) * 8U;
                                slice_pitch = row_pitch * height;
                            }
                            break;

                        case DXGI_FORMAT_NV12:
                        case DXGI_FORMAT_420_OPAQUE:
                            assert(is_planar_format(fmt));
                            {
                                row_pitch = ((width + 1U) >> 1U) * 2U;
                                slice_pitch = row_pitch * (height + ((height + 1U) >> 1U));
                            }
                            break;

                        case DXGI_FORMAT_P010:
                        case DXGI_FORMAT_P016:
                        case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
                        case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                        case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                            assert(is_planar_format(fmt));
                            {
                                row_pitch = ((width + 1U) >> 1U) * 4U;
                                slice_pitch = row_pitch * (height + ((height + 1U) >> 1U));
                            }
                            break;

                        case DXGI_FORMAT_NV11:
                            assert(is_planar_format(fmt));
                            {
                                row_pitch = ((width + 3U) >> 2U) * 4U;
                                slice_pitch = row_pitch * height * 2U;
                            }
                            break;

                        case DXGI_FORMAT_P208:
                            assert(is_planar_format(fmt));
                            {
                                row_pitch = ((width + 1U) >> 1U) * 2U;
                                slice_pitch = row_pitch * height * 2U;
                            }
                            break;

                        case DXGI_FORMAT_V208:
                            assert(is_planar_format(fmt));
                            {
                                row_pitch = width;
                                slice_pitch = row_pitch * (height + (((height + 1U) >> 1U) * 2U));
                            }
                            break;

                        case DXGI_FORMAT_V408:
                            assert(is_planar_format(fmt));
                            {
                                row_pitch = width;
                                slice_pitch = row_pitch * (height + ((height >> 1U) * 4U));
                            }
                            break;

                        default:
                            assert(!is_compressed_format(fmt) && !is_planar_format(fmt) && !is_planar_format(fmt));
                            {

                                auto bpp = bits_per_pixel(fmt);

                                {
                                    // Default byte alignment
                                    row_pitch = (width * bpp + 7U) / 8U;
                                    slice_pitch = row_pitch * height;
                                }
                            }
                            break;
                        }
                }

                //-------------------------------------------------------------------------------------
                inline bool is_palettized_format(DXGI_FORMAT fmt)
                {
                    switch (fmt)
                    {
                        case DXGI_FORMAT_AI44:
                        case DXGI_FORMAT_IA44:
                        case DXGI_FORMAT_P8:
                        case DXGI_FORMAT_A8P8:
                            return true;

                        default:
                            return false;
                    }
                }

                //-------------------------------------------------------------------------------------
                inline bool is_srgb(DXGI_FORMAT fmt)
                {
                    switch (fmt)
                    {
                        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                        case DXGI_FORMAT_BC1_UNORM_SRGB:
                        case DXGI_FORMAT_BC2_UNORM_SRGB:
                        case DXGI_FORMAT_BC3_UNORM_SRGB:
                        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                        case DXGI_FORMAT_BC7_UNORM_SRGB:
                            return true;

                        default:
                            return false;
                    }
                }

                //-------------------------------------------------------------------------------------
                inline uint32_t scan_lines(DXGI_FORMAT fmt, uint32_t height)
                {
                    switch (static_cast<int>(fmt))
                    {
                        case DXGI_FORMAT_BC1_TYPELESS:
                        case DXGI_FORMAT_BC1_UNORM:
                        case DXGI_FORMAT_BC1_UNORM_SRGB:
                        case DXGI_FORMAT_BC2_TYPELESS:
                        case DXGI_FORMAT_BC2_UNORM:
                        case DXGI_FORMAT_BC2_UNORM_SRGB:
                        case DXGI_FORMAT_BC3_TYPELESS:
                        case DXGI_FORMAT_BC3_UNORM:
                        case DXGI_FORMAT_BC3_UNORM_SRGB:
                        case DXGI_FORMAT_BC4_TYPELESS:
                        case DXGI_FORMAT_BC4_UNORM:
                        case DXGI_FORMAT_BC4_SNORM:
                        case DXGI_FORMAT_BC5_TYPELESS:
                        case DXGI_FORMAT_BC5_UNORM:
                        case DXGI_FORMAT_BC5_SNORM:
                        case DXGI_FORMAT_BC6H_TYPELESS:
                        case DXGI_FORMAT_BC6H_UF16:
                        case DXGI_FORMAT_BC6H_SF16:
                        case DXGI_FORMAT_BC7_TYPELESS:
                        case DXGI_FORMAT_BC7_UNORM:
                        case DXGI_FORMAT_BC7_UNORM_SRGB:
                            assert(is_compressed_format(fmt));
                            return std::max<uint32_t>(1U, (height + 3U) / 4U);

                        case DXGI_FORMAT_NV11:
                        case DXGI_FORMAT_P208:
                            assert(is_planar_format(fmt));
                            return height * 2U;

                        case DXGI_FORMAT_V208:
                            assert(is_planar_format(fmt));
                            return height + (((height + 1U) >> 1U) * 2U);

                        case DXGI_FORMAT_V408:
                            assert(is_planar_format(fmt));
                            return height + ((height >> 1U) * 4U);

                        case DXGI_FORMAT_NV12:
                        case DXGI_FORMAT_P010:
                        case DXGI_FORMAT_P016:
                        case DXGI_FORMAT_420_OPAQUE:
                        case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
                        case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
                        case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
                            assert(is_planar_format(fmt));
                            return height + ((height + 1U) >> 1U);

                        default:
                            assert(!is_planar_format(fmt) && !is_planar_format(fmt));
                            return height;
                    }
                }
                //-------------------------------------------------------------------------------------
                inline DXGI_FORMAT make_srgb(DXGI_FORMAT fmt)
                {
                    switch (fmt)
                    {
                        case DXGI_FORMAT_R8G8B8A8_UNORM:
                            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

                        case DXGI_FORMAT_BC1_UNORM:
                            return DXGI_FORMAT_BC1_UNORM_SRGB;

                        case DXGI_FORMAT_BC2_UNORM:
                            return DXGI_FORMAT_BC2_UNORM_SRGB;

                        case DXGI_FORMAT_BC3_UNORM:
                            return DXGI_FORMAT_BC3_UNORM_SRGB;

                        case DXGI_FORMAT_B8G8R8A8_UNORM:
                            return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

                        case DXGI_FORMAT_B8G8R8X8_UNORM:
                            return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

                        case DXGI_FORMAT_BC7_UNORM:
                            return DXGI_FORMAT_BC7_UNORM_SRGB;

                        default:
                            return fmt;
                    }
                }
                //-------------------------------------------------------------------------------------
                inline DXGI_FORMAT make_typeless(DXGI_FORMAT fmt)
                {
                    switch (static_cast<int>(fmt))
                    {
                    case DXGI_FORMAT_R32G32B32A32_FLOAT:
                    case DXGI_FORMAT_R32G32B32A32_UINT:
                    case DXGI_FORMAT_R32G32B32A32_SINT:
                        return DXGI_FORMAT_R32G32B32A32_TYPELESS;

                    case DXGI_FORMAT_R32G32B32_FLOAT:
                    case DXGI_FORMAT_R32G32B32_UINT:
                    case DXGI_FORMAT_R32G32B32_SINT:
                        return DXGI_FORMAT_R32G32B32_TYPELESS;

                    case DXGI_FORMAT_R16G16B16A16_FLOAT:
                    case DXGI_FORMAT_R16G16B16A16_UNORM:
                    case DXGI_FORMAT_R16G16B16A16_UINT:
                    case DXGI_FORMAT_R16G16B16A16_SNORM:
                    case DXGI_FORMAT_R16G16B16A16_SINT:
                        return DXGI_FORMAT_R16G16B16A16_TYPELESS;

                    case DXGI_FORMAT_R32G32_FLOAT:
                    case DXGI_FORMAT_R32G32_UINT:
                    case DXGI_FORMAT_R32G32_SINT:
                        return DXGI_FORMAT_R32G32_TYPELESS;

                    case DXGI_FORMAT_R10G10B10A2_UNORM:
                    case DXGI_FORMAT_R10G10B10A2_UINT:
                    case XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
                    case XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
                        return DXGI_FORMAT_R10G10B10A2_TYPELESS;

                    case DXGI_FORMAT_R8G8B8A8_UNORM:
                    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
                    case DXGI_FORMAT_R8G8B8A8_UINT:
                    case DXGI_FORMAT_R8G8B8A8_SNORM:
                    case DXGI_FORMAT_R8G8B8A8_SINT:
                        return DXGI_FORMAT_R8G8B8A8_TYPELESS;

                    case DXGI_FORMAT_R16G16_FLOAT:
                    case DXGI_FORMAT_R16G16_UNORM:
                    case DXGI_FORMAT_R16G16_UINT:
                    case DXGI_FORMAT_R16G16_SNORM:
                    case DXGI_FORMAT_R16G16_SINT:
                        return DXGI_FORMAT_R16G16_TYPELESS;

                    case DXGI_FORMAT_D32_FLOAT:
                    case DXGI_FORMAT_R32_FLOAT:
                    case DXGI_FORMAT_R32_UINT:
                    case DXGI_FORMAT_R32_SINT:
                        return DXGI_FORMAT_R32_TYPELESS;

                    case DXGI_FORMAT_R8G8_UNORM:
                    case DXGI_FORMAT_R8G8_UINT:
                    case DXGI_FORMAT_R8G8_SNORM:
                    case DXGI_FORMAT_R8G8_SINT:
                        return DXGI_FORMAT_R8G8_TYPELESS;

                    case DXGI_FORMAT_R16_FLOAT:
                    case DXGI_FORMAT_D16_UNORM:
                    case DXGI_FORMAT_R16_UNORM:
                    case DXGI_FORMAT_R16_UINT:
                    case DXGI_FORMAT_R16_SNORM:
                    case DXGI_FORMAT_R16_SINT:
                        return DXGI_FORMAT_R16_TYPELESS;

                    case DXGI_FORMAT_R8_UNORM:
                    case DXGI_FORMAT_R8_UINT:
                    case DXGI_FORMAT_R8_SNORM:
                    case DXGI_FORMAT_R8_SINT:
                    case XBOX_DXGI_FORMAT_R4G4_UNORM:
                        return DXGI_FORMAT_R8_TYPELESS;

                    case DXGI_FORMAT_BC1_UNORM:
                    case DXGI_FORMAT_BC1_UNORM_SRGB:
                        return DXGI_FORMAT_BC1_TYPELESS;

                    case DXGI_FORMAT_BC2_UNORM:
                    case DXGI_FORMAT_BC2_UNORM_SRGB:
                        return DXGI_FORMAT_BC2_TYPELESS;

                    case DXGI_FORMAT_BC3_UNORM:
                    case DXGI_FORMAT_BC3_UNORM_SRGB:
                        return DXGI_FORMAT_BC3_TYPELESS;

                    case DXGI_FORMAT_BC4_UNORM:
                    case DXGI_FORMAT_BC4_SNORM:
                        return DXGI_FORMAT_BC4_TYPELESS;

                    case DXGI_FORMAT_BC5_UNORM:
                    case DXGI_FORMAT_BC5_SNORM:
                        return DXGI_FORMAT_BC5_TYPELESS;

                    case DXGI_FORMAT_B8G8R8A8_UNORM:
                    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
                        return DXGI_FORMAT_B8G8R8A8_TYPELESS;

                    case DXGI_FORMAT_B8G8R8X8_UNORM:
                    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
                        return DXGI_FORMAT_B8G8R8X8_TYPELESS;

                    case DXGI_FORMAT_BC6H_UF16:
                    case DXGI_FORMAT_BC6H_SF16:
                        return DXGI_FORMAT_BC6H_TYPELESS;

                    case DXGI_FORMAT_BC7_UNORM:
                    case DXGI_FORMAT_BC7_UNORM_SRGB:
                        return DXGI_FORMAT_BC7_TYPELESS;

                    default:
                        return fmt;
                    }
                }
                //-------------------------------------------------------------------------------------
                inline DXGI_FORMAT make_typeless_unorm(DXGI_FORMAT fmt)
                {
                    switch (fmt)
                    {
                        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
                            return DXGI_FORMAT_R16G16B16A16_UNORM;

                        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
                            return DXGI_FORMAT_R10G10B10A2_UNORM;

                        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
                            return DXGI_FORMAT_R8G8B8A8_UNORM;

                        case DXGI_FORMAT_R16G16_TYPELESS:
                            return DXGI_FORMAT_R16G16_UNORM;

                        case DXGI_FORMAT_R8G8_TYPELESS:
                            return DXGI_FORMAT_R8G8_UNORM;

                        case DXGI_FORMAT_R16_TYPELESS:
                            return DXGI_FORMAT_R16_UNORM;

                        case DXGI_FORMAT_R8_TYPELESS:
                            return DXGI_FORMAT_R8_UNORM;

                        case DXGI_FORMAT_BC1_TYPELESS:
                            return DXGI_FORMAT_BC1_UNORM;

                        case DXGI_FORMAT_BC2_TYPELESS:
                            return DXGI_FORMAT_BC2_UNORM;

                        case DXGI_FORMAT_BC3_TYPELESS:
                            return DXGI_FORMAT_BC3_UNORM;

                        case DXGI_FORMAT_BC4_TYPELESS:
                            return DXGI_FORMAT_BC4_UNORM;

                        case DXGI_FORMAT_BC5_TYPELESS:
                            return DXGI_FORMAT_BC5_UNORM;

                        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
                            return DXGI_FORMAT_B8G8R8A8_UNORM;

                        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
                            return DXGI_FORMAT_B8G8R8X8_UNORM;

                        case DXGI_FORMAT_BC7_TYPELESS:
                            return DXGI_FORMAT_BC7_UNORM;

                        default:
                            return fmt;
                    }
                }
        }
    }
}
