// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#pragma once

#include <vector>
#include <uc_dev/gx/img/img.h>

namespace uc
{
    namespace model
    {
        namespace details
        {
            template <typename swizzle_t >
            inline void swizzle_texture(const gx::imaging::cpu_texture& o, uint8_t* pixels_out)
            {
                auto w = o.width();
                auto h = o.height();
                //auto bpp = o.bpp();
                auto pitch = o.row_pitch();
                auto pixels_s = reinterpret_cast<const swizzle_t*> (o.pixels().get_pixels_cpu());
                //auto size = o.size();

                auto pixels_d = reinterpret_cast<swizzle_t*> (pixels_out);

                for (auto i = 0U; i < h; ++i)
                {
                    auto rs = pixels_s + i * pitch;
                    auto ds = pixels_d + i * pitch;

                    for (auto j = 0U; j < w; ++j)
                    {
                        //rgba to 
                        swizzle_t r = *rs; rs++;
                        swizzle_t g = *rs; rs++;
                        swizzle_t b = *rs; rs++;
                        swizzle_t a = *rs; rs++;

                        //argb conversion
                        *ds = b; ds++;
                        *ds = g; ds++;
                        *ds = r; ds++;
                        *ds = a; ds++;
                    }
                }
            }

            template <typename swizzle_t >
            inline void swizzle_texture_3(const gx::imaging::cpu_texture& o, uint8_t* pixels_out)
            {
                auto w = o.width();
                auto h = o.height();
                //auto bpp = o.bpp();
                auto pitch = o.row_pitch();
                auto pixels_s = reinterpret_cast<const swizzle_t*> (o.pixels().get_pixels_cpu());
                //auto size = o.size();

                auto pixels_d = reinterpret_cast<swizzle_t*> (pixels_out);

                for (auto i = 0U; i < h; ++i)
                {
                    auto rs = pixels_s + i * pitch;
                    auto ds = pixels_d + i * pitch;

                    for (auto j = 0U; j < w; ++j)
                    {
                        //rgba to 
                        swizzle_t r = *rs; rs++;
                        swizzle_t g = *rs; rs++;
                        swizzle_t b = *rs; rs++;

                        //argb conversion
                        *ds = b; ds++;
                        *ds = g; ds++;
                        *ds = r; ds++;
                    }
                }
            }

            inline uint32_t reverse_bit(uint32_t x)
            {
                x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
                x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
                x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
                x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
                return((x >> 16) | (x << 16));

            }

            inline void mirror_bits_texture(const gx::imaging::cpu_texture& o, uint8_t* pixels_out)
            {
                auto w = o.width();
                auto h = o.height();
                //auto bpp = o.bpp();
                auto pitch = o.row_pitch();
                auto pixels_s = reinterpret_cast<const uint32_t*> (o.pixels().get_pixels_cpu());

                auto pixels_d = reinterpret_cast<uint32_t*> (pixels_out);

                for (auto i = 0U; i < h; ++i)
                {
                    auto rs = pixels_s + i * pitch;
                    auto ds = pixels_d + i * pitch;

                    for (auto j = 0U; j < w; ++j)
                    {
                        //rgba to 
                        uint32_t bytes = *rs; rs++;

                        //10bit
                        uint8_t  r = static_cast<uint8_t>(bytes & 1023);
                        uint8_t  g = static_cast<uint8_t>((bytes >> 10) & 1023);
                        uint8_t  b = static_cast<uint8_t>((bytes >> 20) & 1023);
                        uint8_t  a = static_cast<uint8_t>((bytes >> 30) & 0x3);

                        uint32_t m = a << 30 | (r << 20) | (g << 10) | b;



                        //argb conversion
                        *ds = m; ds++;
                    }
                }
            }
        }

        inline void swizzle_rgba_bgra(const gx::imaging::cpu_texture& o, uint8_t* pixels_out)
        {
            switch (o.type())
            {
                case gx::imaging::image_type::r32_g32_b32_a32_float: details::swizzle_texture<float>(o, pixels_out); break;
                case gx::imaging::image_type::r16_g16_b16_a16_float: details::swizzle_texture<std::uint16_t>(o, pixels_out); break;
                case gx::imaging::image_type::r16_g16_b16_a16_unorm: details::swizzle_texture<std::uint16_t>(o, pixels_out); break;
                case gx::imaging::image_type::r8_g8_b8_a8_unorm:     details::swizzle_texture<std::uint8_t>(o, pixels_out); break;
                case gx::imaging::image_type::r10_g10_b10_xr_bias_a2_unorm: details::mirror_bits_texture(o, pixels_out); break;
                case gx::imaging::image_type::r10_g10_b10_a2_unorm : details::mirror_bits_texture(o, pixels_out); break;
                case gx::imaging::image_type::r32_g32_b32_float     : details::swizzle_texture_3<float>(o, pixels_out); break;

                default: assert(false);
            }
        }

    }

}

