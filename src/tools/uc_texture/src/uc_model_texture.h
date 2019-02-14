// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#pragma once

#include <vector>
#include <gsl/gsl>

#include <uc_dev/gx/lip/geo.h>
#include <uc_dev/util/utf8_conv.h>

#include "uc_model_exception.h"
#include "uc_model_compressonator.h"

namespace uc
{
    namespace model
    {
        inline uc::lip::storage_format cmp_to_lip(CMP_FORMAT f)
        {
            switch (f)
            {
                case CMP_FORMAT_ARGB_8888:      return lip::storage_format::r8_g8_b8_a8_unorm;
                case CMP_FORMAT_BC1:            return lip::storage_format::bc1;
                case CMP_FORMAT_BC2:            return lip::storage_format::bc2;
                case CMP_FORMAT_BC3:            return lip::storage_format::bc3;
                case CMP_FORMAT_BC4:            return lip::storage_format::bc4;
                case CMP_FORMAT_BC5:            return lip::storage_format::bc5;
                default: throw model::exception("Invalid texture format");
            }
        }

        inline CMP_FORMAT lip_to_cmp(uc::lip::storage_format f)
        {
            switch (f)
            {
                case lip::storage_format::r8_g8_b8_a8_unorm: return CMP_FORMAT_ARGB_8888;
                case lip::storage_format::bc1:               return CMP_FORMAT_BC1;
                case lip::storage_format::bc2:               return CMP_FORMAT_BC2;
                case lip::storage_format::bc3:               return CMP_FORMAT_BC3;
                case lip::storage_format::bc4:               return CMP_FORMAT_BC4;
                case lip::storage_format::bc5:               return CMP_FORMAT_BC5;
                default: throw model::exception("Invalid texture format");
            }
        }

        inline uc::lip::storage_format image_type_to_lip(gx::imaging::image_type f)
        {
            switch (f)
            {
                case gx::imaging::image_type::r32_g32_b32_a32_float: return lip::storage_format::r32_g32_b32_a32_float;
                case gx::imaging::image_type::r16_g16_b16_a16_float: return lip::storage_format::r16_g16_b16_a16_float;
                case gx::imaging::image_type::r16_g16_b16_a16_unorm: return lip::storage_format::r16_g16_b16_a16_unorm;

                case gx::imaging::image_type::r8_g8_b8_a8_unorm: return lip::storage_format::r8_g8_b8_a8_unorm;
                case gx::imaging::image_type::b8_g8_r8_a8_unorm: return lip::storage_format::b8_g8_r8_a8_unorm;
                case gx::imaging::image_type::b8_g8_r8_x8_unorm: return lip::storage_format::b8_g8_r8_x8_unorm;

                case gx::imaging::image_type::r10_g10_b10_xr_bias_a2_unorm: return lip::storage_format::r10_g10_b10_xr_bias_a2_unorm;
                case gx::imaging::image_type::r10_g10_b10_a2_unorm: return lip::storage_format::r10_g10_b10_a2_unorm;

                case gx::imaging::image_type::b5_g5_r5_a1_unorm: return lip::storage_format::b5_g5_r5_a1_unorm;
                case gx::imaging::image_type::b5_g6_r5_unorm: return lip::storage_format::b5_g6_r5_unorm;

                case gx::imaging::image_type::r32_float: return lip::storage_format::r32_float;
                case gx::imaging::image_type::r16_float: return lip::storage_format::r16_float;
                case gx::imaging::image_type::r16_unorm: return lip::storage_format::r16_unorm;
                case gx::imaging::image_type::r8_unorm: return lip::storage_format::r8_unorm;
                case gx::imaging::image_type::a8_unorm: return lip::storage_format::a8_unorm;
                case gx::imaging::image_type::r32_g32_b32_float: return lip::storage_format::r32_g32_b32_float;

                default: throw model::exception("Invalid texture format");
            }
        }

        inline std::vector<uint8_t> convert_cmp(compressonator::texture< compressonator::texture_storage >&& source, CMP_FORMAT f)
        {
            auto p = cmp_texture(source.width(), source.height(), f);

            CMP_CompressOptions o = {};
            o.bDisableMultiThreading = TRUE;
            o.dwSize = sizeof(o);
            compressonator::throw_if_failed(CMP_ConvertTexture(&source, &std::get<0>(p), &o, nullptr, 0, 0));
            return std::get<1>(p);
        }

        inline uc::lip::texture2d create_texture_2d(const std::string& file_name)
        {
            auto r0 = gx::imaging::read_image(util::utf16_from_utf8(file_name).c_str());

            uc::lip::texture2d r;

            //storage and view formats match
            r.m_storage_format = static_cast<uint16_t>(image_type_to_lip(r0.type()));
            r.m_view_format = static_cast<uint16_t>(image_type_to_lip(r0.type()));

            auto w = r0.width();
            auto h = r0.height();

            r.m_width = static_cast<uint16_t>(w);
            r.m_height = static_cast<uint16_t>(h);

            auto span = gsl::make_span(r0.pixels().get_pixels_cpu(), r0.size());
            r.m_data.resize(span.size());
            std::copy(span.begin(), span.end(), &r.m_data[0]);

            return r;
        }

        inline uc::lip::texture2d_mip_chain create_texture_2d_mip_chain(const std::string& file_name)
        {
            auto r0 = gx::imaging::read_image(util::utf16_from_utf8(file_name).c_str());

            uc::lip::texture2d_mip_level r;

            //storage and view formats match
            r.m_storage_format = static_cast<uint16_t>(image_type_to_lip(r0.type()));
            r.m_view_format = static_cast<uint16_t>(image_type_to_lip(r0.type()));

            auto w = r0.width();
            auto h = r0.height();

            r.m_width = static_cast<uint16_t>(w);
            r.m_height = static_cast<uint16_t>(h);

            auto span = gsl::make_span(r0.pixels().get_pixels_cpu(), r0.size());
            r.m_data.resize(span.size());
            std::copy(span.begin(), span.end(), &r.m_data[0]);

            uc::lip::texture2d_mip_chain t;

            t.m_levels.push_back(std::move(r));

            return t;
        }


        inline uc::lip::texture2d create_texture_2d( const std::string& file_name, lip::storage_format storage, lip::view_format view)
        {
            auto r0 = gx::imaging::read_image(util::utf16_from_utf8(file_name).c_str());

            uc::lip::texture2d r;
            //only this is supported
            r.m_storage_format = static_cast<uint16_t>(storage);
            r.m_view_format    = static_cast<uint16_t>(view);

            auto w          = r0.width();
            auto h          = r0.height();

            r.m_width       = static_cast<uint16_t>(w);
            r.m_height      = static_cast<uint16_t>(h);
            auto bc         = convert_cmp(compressonator::make_texture(std::move(r0)), lip_to_cmp(storage));

            auto span       = gsl::make_span(&bc[0], bc.size());
            r.m_data.resize(bc.size());
            std::copy(span.begin(), span.end(), &r.m_data[0]);

            return r;
        }

        inline uc::lip::texture2d_mip_chain create_texture_2d_mip_chain(const std::string& file_name, lip::storage_format storage, lip::view_format view)
        {
            auto r0 = gx::imaging::read_image(util::utf16_from_utf8(file_name).c_str());

            uc::lip::texture2d_mip_level r;

            //only this is supported
            r.m_storage_format  = static_cast<uint16_t>(storage);
            r.m_view_format     = static_cast<uint16_t>(view);

            auto w = r0.width();
            auto h = r0.height();

            r.m_width = static_cast<uint16_t>(w);
            r.m_height = static_cast<uint16_t>(h);

            auto span = gsl::make_span(r0.pixels().get_pixels_cpu(), r0.size());
            r.m_data.resize(span.size());
            std::copy(span.begin(), span.end(), &r.m_data[0]);

            uc::lip::texture2d_mip_chain t;
            t.m_levels.push_back(std::move(r));

            return t;
        }
    }
}

