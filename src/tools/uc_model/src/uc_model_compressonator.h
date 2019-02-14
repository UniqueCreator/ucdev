// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#pragma once

#include <memory>
#include <vector>
#include <uc_dev/gx/img/img.h>

#include <Compressonator.h>
#include "uc_model_swizzle.h"
#include "uc_model_exception.h"

namespace uc
{
    namespace model
    {
        namespace compressonator
        {
            class compressonator_exception : public exception
            {
                using base = model::exception;

                const std::string error_message(CMP_ERROR e)
                {
                    switch (e)
                    {
                        case CMP_OK:                                return std::string("OK");
                        case CMP_ABORTED:                           return std::string("The conversion was aborted.");
                        case CMP_ERR_INVALID_SOURCE_TEXTURE:        return std::string("The source texture is invalid.");
                        case CMP_ERR_INVALID_DEST_TEXTURE:          return std::string("The destination texture is invalid.");
                        case CMP_ERR_UNSUPPORTED_SOURCE_FORMAT:     return std::string("The source format is not a supported format.");
                        case CMP_ERR_UNSUPPORTED_DEST_FORMAT:       return std::string("The destination format is not a supported format.");
                        case CMP_ERR_SIZE_MISMATCH:                 return std::string("The source and destination texture sizes do not match.");
                        case CMP_ERR_UNABLE_TO_INIT_CODEC:          return std::string("Compressonator was unable to initialize the codec needed for conversion.");
                        case CMP_ERR_UNABLE_TO_INIT_DECOMPRESSLIB:  return std::string("GPU_Decode Lib was unable to initialize the codec needed for decompression.");
                        case CMP_ERR_GENERIC:
                        default:                                    return std::string("An unknown error occurred.");
                    }
                }

                public:

                compressonator_exception( CMP_ERROR e) : base( error_message(e) ),  m_error(e)
                {

                }

                CMP_ERROR m_error;
            };

            class bc_exception : public model::exception
            {
                using base = model::exception;

                const std::string error_message(BC_ERROR e)
                {
                    switch (e)
                    {
                        case BC_ERROR_NONE:                         return std::string("Error none");
                        case BC_ERROR_LIBRARY_NOT_INITIALIZED:      return std::string("Library not initialized");
                        case BC_ERROR_LIBRARY_ALREADY_INITIALIZED:  return std::string("Library already initialized");
                        case BC_ERROR_INVALID_PARAMETERS:           return std::string("Invalid parameters");
                        case BC_ERROR_OUT_OF_MEMORY:                return std::string("Error out of memory");
                        default:                                    return std::string("An unknown error occurred.");
                    }
                }

            public:

                bc_exception(BC_ERROR e) : base(error_message(e)), m_error(e)
                {

                }

                BC_ERROR m_error;
            };


            inline void throw_if_failed(CMP_ERROR e)
            {
                if (e != CMP_OK)
                {
                    throw compressonator_exception(e);
                }
            }

            inline void throw_if_failed(BC_ERROR e)
            {
                if (e != CMP_OK)
                {
                    throw bc_exception(e);
                }
            }

            struct initializer
            {
                initializer()
                {
                    throw_if_failed(CMP_InitializeBCLibrary());
                }

                ~initializer()
                {
                    CMP_ShutdownBCLibrary();
                }
            };


            inline std::tuple<uint32_t, uint32_t, uint32_t > block(CMP_FORMAT f)
            {
                switch (f)
                {
                case CMP_FORMAT_ARGB_8888: return std::make_tuple(1, 1, 1);                  ///< ARGB format with 8-bit fixed channels.
                case CMP_FORMAT_RGB_888: return std::make_tuple(1, 1, 1);                    ///<    RGB format with 8-bit fixed channels.
                case CMP_FORMAT_RG_8:return std::make_tuple(1, 1, 1);                       ///<    Two component format with 8-bit fixed channels.
                case CMP_FORMAT_R_8:return std::make_tuple(1, 1, 1);                        ///<    Single component format with 8-bit fixed channels.
                case CMP_FORMAT_ARGB_2101010:return std::make_tuple(1, 1, 1);               ///< ARGB format with 10-bit fixed channels for color & a 2-bit fixed channel for alpha.
                case CMP_FORMAT_ARGB_16:return std::make_tuple(1, 1, 1);                    ///<    RGB format with 16-bit fixed channels.
                case CMP_FORMAT_RG_16:return std::make_tuple(1, 1, 1);                      ///<    Two component format with 16-bit fixed channels.
                case CMP_FORMAT_R_16:return std::make_tuple(1, 1, 1);                       ///<    Single component format with 16-bit fixed channels.
                case CMP_FORMAT_ARGB_16F:return std::make_tuple(1, 1, 1);                   ///< ARGB format with 16-bit floating-point channels.
                case CMP_FORMAT_RG_16F:return std::make_tuple(1, 1, 1);                     ///<    Two component format with 16-bit floating-point channels.
                case CMP_FORMAT_R_16F:return std::make_tuple(1, 1, 1);                      ///<    Single component with 16-bit floating-point channels.
                case CMP_FORMAT_ARGB_32F:return std::make_tuple(1, 1, 1);                   ///< ARGB format with 32-bit floating-point channels.
                case CMP_FORMAT_RGB_32F:return std::make_tuple(1, 1, 1);                     ///<     RGB format with 32-bit floating-point channels.
                case CMP_FORMAT_RG_32F:return std::make_tuple(1, 1, 1);                     ///<    Two component format with 32-bit floating-point channels.
                case CMP_FORMAT_R_32F:return std::make_tuple(1, 1, 1);                      ///<    Single component with 32-bit floating-point channels.
                // Compression formats -----------------------------------------------------------------------------------
                case CMP_FORMAT_ASTC:return std::make_tuple(4, 4, 1);                       ///< ASTC (Adaptive Scalable Texture Compression) open texture compression standard
                case CMP_FORMAT_ATI1N:return std::make_tuple(4, 4, 1);                      ///< Single component compression format using the same technique as DXT5 alpha. Four bits per pixel.
                case CMP_FORMAT_ATI2N:return std::make_tuple(4, 4, 1);                      ///<     Two component compression format using the same technique as DXT5 alpha. Designed for compression object space normal maps. Eight bits per pixel.
                case CMP_FORMAT_ATI2N_XY:return std::make_tuple(4, 4, 1);                   ///<    Two component compression format using the same technique as DXT5 alpha. The same as ATI2N but with the channels swizzled. Eight bits per pixel.
                case CMP_FORMAT_ATI2N_DXT5:return std::make_tuple(4, 4, 1);                 ///<    ATI2N like format using DXT5. Intended for use on GPUs that do not natively support ATI2N. Eight bits per pixel.
                case CMP_FORMAT_ATC_RGB:return std::make_tuple(4, 4, 1);                    ///< CMP - a compressed RGB format.
                case CMP_FORMAT_ATC_RGBA_Explicit:return std::make_tuple(4, 4, 1);          ///< CMP - a compressed ARGB format with explicit alpha.
                case CMP_FORMAT_ATC_RGBA_Interpolated:return std::make_tuple(4, 4, 1);      ///< CMP - a compressed ARGB format with interpolated alpha.
                case CMP_FORMAT_BC1:return std::make_tuple(4, 4, 1);                        ///< A four component opaque (or 1-bit alpha) compressed texture format for Microsoft DirectX10. Identical to DXT1.  Four bits per pixel.
                case CMP_FORMAT_BC2:return std::make_tuple(4, 4, 1);                        ///< A four component compressed texture format with explicit alpha for Microsoft DirectX10. Identical to DXT3. Eight bits per pixel.
                case CMP_FORMAT_BC3:return std::make_tuple(4, 4, 1);                        ///< A four component compressed texture format with interpolated alpha for Microsoft DirectX10. Identical to DXT5. Eight bits per pixel.
                case CMP_FORMAT_BC4:return std::make_tuple(4, 4, 1);                        ///< A single component compressed texture format for Microsoft DirectX10. Identical to ATI1N. Four bits per pixel.
                case CMP_FORMAT_BC5:return std::make_tuple(4, 4, 1);                        ///< A two component compressed texture format for Microsoft DirectX10. Identical to ATI2N. Eight bits per pixel.
                case CMP_FORMAT_BC6H:return std::make_tuple(4, 4, 1);                       ///< BC6H compressed texture format 
                case CMP_FORMAT_BC7:return std::make_tuple(4, 4, 1);                        ///< BC7  compressed texture format
                case CMP_FORMAT_DXT1:return std::make_tuple(4, 4, 1);                       ///< An DXTC compressed texture matopaque (or 1-bit alpha). Four bits per pixel.
                case CMP_FORMAT_DXT3:return std::make_tuple(4, 4, 1);                       ///<    DXTC compressed texture format with explicit alpha. Eight bits per pixel.
                case CMP_FORMAT_DXT5:return std::make_tuple(4, 4, 1);                       ///<    DXTC compressed texture format with interpolated alpha. Eight bits per pixel.
                case CMP_FORMAT_DXT5_xGBR:return std::make_tuple(4, 4, 1);                  ///<    DXT5 with the red component swizzled into the alpha channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_RxBG:return std::make_tuple(4, 4, 1);                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_RBxG:return std::make_tuple(4, 4, 1);                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel & the blue component swizzled into the green channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_xRBG:return std::make_tuple(4, 4, 1);                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel & the red component swizzled into the green channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_RGxB:return std::make_tuple(4, 4, 1);                  ///<    swizzled DXT5 format with the blue component swizzled into the alpha channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_xGxR:return std::make_tuple(4, 4, 1);                  ///<    two-component swizzled DXT5 format with the red component swizzled into the alpha channel & the green component in the green channel. Eight bits per pixel.
                case CMP_FORMAT_ETC_RGB:return std::make_tuple(4, 4, 1);                    ///< ETC  (Ericsson Texture Compression) 
                case CMP_FORMAT_ETC2_RGB:return std::make_tuple(4, 4, 1);                   ///< ETC2 (Ericsson Texture Compression) 
                default: assert(false); return std::make_tuple(4, 4, 1);
                }
            }

            inline uint32_t bits_per_pixel(CMP_FORMAT f)
            {
                switch (f)
                {
                case CMP_FORMAT_ARGB_8888: return 32;                  ///< ARGB format with 8-bit fixed channels.
                case CMP_FORMAT_RGB_888: return 24;                    ///<    RGB format with 8-bit fixed channels.
                case CMP_FORMAT_RG_8:return 16;                       ///<    Two component format with 8-bit fixed channels.
                case CMP_FORMAT_R_8:return 8;                        ///<    Single component format with 8-bit fixed channels.
                case CMP_FORMAT_ARGB_2101010: return 32;               ///< ARGB format with 10-bit fixed channels for color & a 2-bit fixed channel for alpha.
                case CMP_FORMAT_ARGB_16:return 64;                    ///<    RGB format with 16-bit fixed channels.
                case CMP_FORMAT_RG_16:return 32;                      ///<    Two component format with 16-bit fixed channels.
                case CMP_FORMAT_R_16:return 16;                       ///<    Single component format with 16-bit fixed channels.
                case CMP_FORMAT_ARGB_16F:return 64;                   ///< ARGB format with 16-bit floating-point channels.
                case CMP_FORMAT_RG_16F:return 32;                     ///<    Two component format with 16-bit floating-point channels.
                case CMP_FORMAT_R_16F:return 16;                      ///<    Single component with 16-bit floating-point channels.
                case CMP_FORMAT_ARGB_32F:return 128;                   ///< ARGB format with 32-bit floating-point channels.
                case CMP_FORMAT_RGB_32F:return 96;                     ///<     RGB format with 32-bit floating-point channels.
                case CMP_FORMAT_RG_32F:return 64;                     ///<    Two component format with 32-bit floating-point channels.
                case CMP_FORMAT_R_32F:return 32;                      ///<    Single component with 32-bit floating-point channels.
                                                                                            // Compression formats -----------------------------------------------------------------------------------
                case CMP_FORMAT_ASTC:return 4;                                               ///< ASTC (Adaptive Scalable Texture Compression) open texture compression standard
                case CMP_FORMAT_ATI1N:return 4;                                          ///< Single component compression format using the same technique as DXT5 alpha. Four bits per pixel.
                case CMP_FORMAT_ATI2N:return 8;                      ///<     Two component compression format using the same technique as DXT5 alpha. Designed for compression object space normal maps. Eight bits per pixel.
                case CMP_FORMAT_ATI2N_XY:return 8;                   ///<    Two component compression format using the same technique as DXT5 alpha. The same as ATI2N but with the channels swizzled. Eight bits per pixel.
                case CMP_FORMAT_ATI2N_DXT5:return 8;                 ///<    ATI2N like format using DXT5. Intended for use on GPUs that do not natively support ATI2N. Eight bits per pixel.
                case CMP_FORMAT_ATC_RGB:return 4;                    ///< CMP - a compressed RGB format.
                case CMP_FORMAT_ATC_RGBA_Explicit:return 8;          ///< CMP - a compressed ARGB format with explicit alpha.
                case CMP_FORMAT_ATC_RGBA_Interpolated:return 8;      ///< CMP - a compressed ARGB format with interpolated alpha.
                case CMP_FORMAT_BC1:return 4;                        ///< A four component opaque (or 1-bit alpha) compressed texture format for Microsoft DirectX10. Identical to DXT1.  Four bits per pixel.
                case CMP_FORMAT_BC2:return 8;                        ///< A four component compressed texture format with explicit alpha for Microsoft DirectX10. Identical to DXT3. Eight bits per pixel.
                case CMP_FORMAT_BC3:return 8;                        ///< A four component compressed texture format with interpolated alpha for Microsoft DirectX10. Identical to DXT5. Eight bits per pixel.
                case CMP_FORMAT_BC4:return 4;                        ///< A single component compressed texture format for Microsoft DirectX10. Identical to ATI1N. Four bits per pixel.
                case CMP_FORMAT_BC5:return 8;                        ///< A two component compressed texture format for Microsoft DirectX10. Identical to ATI2N. Eight bits per pixel.
                case CMP_FORMAT_BC6H:return 4;                       ///< BC6H compressed texture format 
                case CMP_FORMAT_BC7:return 4;                        ///< BC7  compressed texture format
                case CMP_FORMAT_DXT1:return 4;                       ///< An DXTC compressed texture matopaque (or 1-bit alpha). Four bits per pixel.
                case CMP_FORMAT_DXT3:return 8;                       ///<    DXTC compressed texture format with explicit alpha. Eight bits per pixel.
                case CMP_FORMAT_DXT5:return 8;                       ///<    DXTC compressed texture format with interpolated alpha. Eight bits per pixel.
                case CMP_FORMAT_DXT5_xGBR:return 8;                  ///<    DXT5 with the red component swizzled into the alpha channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_RxBG:return 8;                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_RBxG:return 8;                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel & the blue component swizzled into the green channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_xRBG:return 8;                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel & the red component swizzled into the green channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_RGxB:return 8;                  ///<    swizzled DXT5 format with the blue component swizzled into the alpha channel. Eight bits per pixel.
                case CMP_FORMAT_DXT5_xGxR:return 8;                  ///<    two-component swizzled DXT5 format with the red component swizzled into the alpha channel & the green component in the green channel. Eight bits per pixel.
                case CMP_FORMAT_ETC_RGB:return 4;                    ///< ETC  (Ericsson Texture Compression) 
                case CMP_FORMAT_ETC2_RGB:return 4;                   ///< ETC2 (Ericsson Texture Compression) 
                default: assert(false); return 0;
                }
            }

            inline uint32_t block(uint32_t width, uint32_t block_size)
            {
                return (width + block_size - 1) / block_size;
            }

            inline uint32_t row_pitch(CMP_FORMAT f, uint32_t base_width)
            {
                auto bpp = bits_per_pixel(f);
                auto block_size_pixels = block(f);
                auto bits_per_block = bpp * std::get<0>(block_size_pixels) * std::get<1>(block_size_pixels);
                auto bytes_per_block = (bits_per_block + 7) / 8U;

                return block(base_width, std::get<0>(block_size_pixels))  * bytes_per_block;
            }

            inline uint32_t slice_pitch(CMP_FORMAT f, uint32_t base_width, uint32_t base_height)
            {
                uint32_t w = row_pitch(f, base_width);
                auto block_size_pixels = block(f);
                auto wh = block(base_height, std::get<1>(block_size_pixels));
                return w * wh;
            }

            template <typename storage> struct to_storage
            {
                const uint8_t* operator() ( storage s ) const;
            };


            template <> struct to_storage< std::vector<uint8_t> >
            {
                const uint8_t* operator()( const std::vector<uint8_t>& o)  const
                {
                    return &o[0];
                }
            };

            template <typename storage >
            struct texture : CMP_Texture
            {
                texture(uint32_t width, uint32_t height, CMP_FORMAT f, storage &&data) : m_data(std::move(data))
                {
                    auto d = block(f);
                    this->dwSize = sizeof(CMP_Texture);
                    this->dwWidth = width;
                    this->dwHeight = height;
                    this->format = f;
                    this->dwPitch = row_pitch(f, width);
                    this->nBlockWidth = static_cast<CMP_BYTE>(std::get<0>(d));
                    this->nBlockHeight = static_cast<CMP_BYTE>(std::get<1>(d));
                    this->nBlockDepth = static_cast<CMP_BYTE>(std::get<2>(d));
                    this->pData = const_cast<uint8_t*> (this->data());
                    this->dwDataSize = slice_pitch(f, width, height);

                    #if defined(_DEBUG)
                    auto c = CMP_CalculateBufferSize(this);
                    assert(c == this->dwDataSize);
                    #endif
                }

                const uint8_t* data() const
                {
                    return to_storage<storage>()(m_data);
                }

                size_t size() const
                {
                    return dwDataSize;
                }

                uint32_t width() const
                {
                    return dwWidth;
                }

                uint32_t height() const
                {
                    return dwHeight;
                }

                uint32_t block_width() const
                {
                    return nBlockWidth;
                }

                uint32_t block_height() const
                {
                    return nBlockHeight;
                }

                uint32_t block_depth() const
                {
                    return nBlockDepth;
                }

                storage m_data;
            };

            using texture_storage = std::vector< std::uint8_t >;

            namespace details
            {
                inline bool can_copy(gx::imaging::image_type t)
                {
                    switch (t)
                    {
                        case gx::imaging::image_type::b8_g8_r8_a8_unorm : return true;
                        case gx::imaging::image_type::b8_g8_r8_x8_unorm : return true;

                        case gx::imaging::image_type::b5_g5_r5_a1_unorm : return true;
                        case gx::imaging::image_type::b5_g6_r5_unorm    : return true;

                        case gx::imaging::image_type::r32_float         : return true;
                        case gx::imaging::image_type::r16_float         : return true;
                        case gx::imaging::image_type::r16_unorm         : return true;
                        case gx::imaging::image_type::r8_unorm          : return true;
                        case gx::imaging::image_type::a8_unorm          : return true;

                        default: return false;
                    }
                }

                inline CMP_FORMAT imaging_to_cmp(gx::imaging::image_type t)
                {
                    switch (t)
                    {
                        case gx::imaging::image_type::r32_g32_b32_a32_float         : return CMP_FORMAT_ARGB_32F;
                        case gx::imaging::image_type::r16_g16_b16_a16_float         : return CMP_FORMAT_ARGB_16F;
                        case gx::imaging::image_type::r16_g16_b16_a16_unorm         : return CMP_FORMAT_ARGB_16;
                        case gx::imaging::image_type::r8_g8_b8_a8_unorm             : return CMP_FORMAT_ARGB_8888;
                        case gx::imaging::image_type::b8_g8_r8_a8_unorm             : return CMP_FORMAT_ARGB_8888;
                        case gx::imaging::image_type::b8_g8_r8_x8_unorm             : return CMP_FORMAT_ARGB_8888;

                        case gx::imaging::image_type::r10_g10_b10_xr_bias_a2_unorm  : return CMP_FORMAT_ARGB_2101010;
                        case gx::imaging::image_type::r10_g10_b10_a2_unorm          : return CMP_FORMAT_ARGB_2101010;

                        case gx::imaging::image_type::b5_g5_r5_a1_unorm             : return CMP_FORMAT_R_16;
                        case gx::imaging::image_type::b5_g6_r5_unorm                : return CMP_FORMAT_R_16;

                        case gx::imaging::image_type::r32_float                     : return CMP_FORMAT_R_32F;
                        case gx::imaging::image_type::r16_float                     : return CMP_FORMAT_R_16F;
                        case gx::imaging::image_type::r16_unorm                     : return CMP_FORMAT_R_16;
                        case gx::imaging::image_type::r8_unorm                      : return CMP_FORMAT_R_8;
                        case gx::imaging::image_type::a8_unorm                      : return CMP_FORMAT_R_8;
                        case gx::imaging::image_type::r32_g32_b32_float             : return CMP_FORMAT_RGB_32F;

                        default: assert(false); return CMP_FORMAT_Unknown;
                    }
                }
            }

            inline texture< texture_storage > make_texture( gx::imaging::cpu_texture&& r0 )
            {
                if ( details::can_copy(r0.type()) )
                {
                    texture_storage s(r0.size());
                    std::memcpy(&s[0], r0.pixels().get_pixels_cpu(), r0.size());
                    return texture<texture_storage >(r0.width(), r0.height(), details::imaging_to_cmp( r0.type() ),  std::move(s) );
                }
                else
                {
                    texture_storage s(r0.size());
                    swizzle_rgba_bgra(r0, &s[0]);
                    return texture<texture_storage >(r0.width(), r0.height(), details::imaging_to_cmp(r0.type()), std::move(s));
                }
            }
        }

        inline std::tuple<CMP_Texture, std::vector<uint8_t> > cmp_texture(uint32_t width, uint32_t height, CMP_FORMAT f)
        {
            CMP_Texture s   = {};
            s.dwSize        = sizeof(CMP_Texture);
            s.dwWidth       = width;
            s.dwHeight      = height;
            s.format        = f;
            s.nBlockHeight  = 4;
            s.nBlockWidth   = 4;
            s.nBlockDepth   = 1;

            const CMP_Texture* c = &s;
            auto e = CMP_CalculateBufferSize(c);

            std::vector<uint8_t> bc1;
            bc1.resize(e);
            s.dwDataSize = e;
            s.pData = &bc1[0];

            return std::make_tuple(s, std::move(bc1));
        }

        inline lip::storage_format string_to_storage_format(const std::string& s)
        {
            struct formats
            {
                lip::storage_format f;
                const char* s;
            };

            formats f[] =
            {
                 { lip::storage_format::unknown, "unknown" }
                ,{ lip::storage_format::bc1, "bc1_unorm" }
                ,{ lip::storage_format::bc1, "bc1_unorm_srgb" }
                ,{ lip::storage_format::bc2, "bc2_unorm" }
                ,{ lip::storage_format::bc2, "bc2_unorm_srgb" }
                ,{ lip::storage_format::bc3, "bc3_unorm" }
                ,{ lip::storage_format::bc3, "bc3_unorm_srgb" }
                ,{ lip::storage_format::bc4, "bc4_unorm" }
                ,{ lip::storage_format::bc4, "bc4_snorm" }
                ,{ lip::storage_format::bc5, "bc5_unorm" }
                ,{ lip::storage_format::bc5, "bc5_snorm" }
            };

            for (auto i = 0; i < sizeof(f) / sizeof(f[0]); ++i)
            {
                if (std::string(f[i].s) == s)
                {
                    return f[i].f;
                }
            }

            throw model::exception(std::string("invalid string to texture format: ") + s);
        }

        inline lip::view_format string_to_view_format(const std::string& s)
        {
            struct formats
            {
                lip::view_format f;
                const char* s;
            };

            formats f[] =
            {
                 { lip::view_format::unknown, "unknown" }
                ,{ lip::view_format::bc1_unorm , "bc1_unorm" }
                ,{ lip::view_format::bc1_unorm_srgb, "bc1_unorm_srgb" }
                ,{ lip::view_format::bc2_unorm, "bc2_unorm" }
                ,{ lip::view_format::bc2_unorm_srgb, "bc2_unorm_srgb" }
                ,{ lip::view_format::bc3_unorm, "bc3_unorm" }
                ,{ lip::view_format::bc3_unorm_srgb, "bc3_unorm_srgb" }
                ,{ lip::view_format::bc4_unorm, "bc4_unorm" }
                ,{ lip::view_format::bc4_snorm, "bc4_snorm" }
                ,{ lip::view_format::bc5_unorm, "bc5_unorm" }
                ,{ lip::view_format::bc5_snorm, "bc5_snorm" }
            };

            for (auto i = 0; i < sizeof(f) / sizeof(f[0]); ++i)
            {
                if (std::string(f[i].s) == s)
                {
                    return f[i].f;
                }
            }

            throw model::exception(std::string("invalid string to texture format: ") + s);
        }

    }
}

