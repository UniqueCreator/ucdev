// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <cstdint>
#include <iostream>
#include <algorithm>

#include <uc_dev/os/windows/com_initializer.h>
#include <uc_dev/gx/img/img.h>


inline std::ostream& operator<<(std::ostream& s, const std::string& str)
{
    s << str.c_str();
    return s;
}

inline std::string get_environment()
{
#if defined(_X86)
    return "x86";
#endif

#if defined(_X64)
    return "x64";
#endif
}


class media_source
{
public:

    media_source(const wchar_t* file_name) : m_path(file_name)
    {

    }

    media_source(const std::wstring& file_name) : m_path(file_name)
    {

    }

    media_source(std::wstring&& file_name) : m_path(std::move(file_name))
    {

    }

    const wchar_t* get_path() const
    {
        return m_path.c_str();
    }

private:

    std::wstring m_path;
};

class media_url
{
public:

    media_url(const wchar_t* file_name) : m_file_name(file_name)
    {

    }

    media_url(const std::wstring& file_name) : m_file_name(file_name)
    {

    }

    media_url(std::wstring&& file_name) : m_file_name(std::move(file_name))
    {

    }

    const wchar_t* get_path() const
    {
        return m_file_name.c_str();
    }

private:

    std::wstring m_file_name;
};

inline auto make_media_url(const media_source& source, const wchar_t* path)
{
    return media_url(std::wstring(source.get_path()) + std::wstring(path));
}

inline auto make_media_url(const media_source& source, const std::wstring&& path)
{
    return media_url(std::wstring(source.get_path()) + std::wstring(path));
}

enum class blue_noise_type : uint32_t
{
    HDR_L,
    HDR_LA,
    HDR_RGB,
    HDR_RGBA,
    LDR_LLL1,
    LDR_RG01,
    LDR_RGB1,
    LDR_RGBA
};

static auto make_file_name(blue_noise_type type, uint32_t index)
{
    static const std::wstring bluenoise_types[] =
    {
        L"HDR_L_",
        L"HDR_LA_",
        L"HDR_RGB_",
        L"HDR_RGBA_",
        L"LDR_LLL1_",
        L"LDR_RG01_",
        L"LDR_RGB1_",
        L"LDR_RGBA_"
    };

    static std::wstring extentsion = L".png";

    return bluenoise_types[static_cast<uint32_t>(type)] + std::to_wstring(index) + extentsion;
}


static auto make_file_name(blue_noise_type type)
{
    static const std::wstring bluenoise_types[] =
    {
        L"hdr_l_64x64",
        L"hdr_la_64x64",
        L"hdr_rgb_64x64",
        L"hdr_rgba_64x64",
        L"ldr_lll1_64x64",
        L"ldr_rg01_64x64",
        L"ldr_rgb1_64x64",
        L"ldr_rgba_64x64"
    };

    static std::wstring extentsion = L".png";

    return bluenoise_types[static_cast<uint32_t>(type)] + extentsion;
}

static auto bake_images_8x8(const std::vector< uc::gx::imaging::cpu_texture  >& images)
{
    auto composite = uc::gx::imaging::make_image(8 * 64, static_cast<uint32_t>( ((images.size() + 7) / 8) * 64), images[0].type());

    auto composite_pixels = composite.pixels();
    auto composite_lock_initial = composite_pixels.get_pixels_cpu();

    auto composite_row = 0;
    for (auto imgc = 0U; imgc < images.size(); imgc += 8)
    {
        auto composite_lock = composite_lock_initial + (composite_row++) * 64 * composite.row_pitch();

        //fill row by row
        for (auto i = 0; i < 64; ++i)
        {
            auto destination = composite_lock + i * composite.row_pitch();

            for (auto j = imgc; j < std::min(static_cast<size_t>(imgc + 8), images.size()); ++j)
            {
                auto&&      img = images[j];
                auto img_pixels = img.pixels();
                auto img_lock = img_pixels.get_pixels_cpu();
                auto row = img_lock + i * img.row_pitch();
                memcpy_s(destination, img.row_pitch(), row, img.row_pitch());
                destination += img.row_pitch();
            }
        }
    }

    return composite;
}

static auto bake_images(const std::vector< uc::gx::imaging::cpu_texture  >& images)
{
    auto composite = uc::gx::imaging::make_image(static_cast<uint32_t>(images.size()) * 64, static_cast<uint32_t>(((1 + 7) / 8) * 64), images[0].type());

    auto composite_pixels = composite.pixels();
    auto composite_lock_initial = composite_pixels.get_pixels_cpu();

    auto composite_row = 0;
    for (auto imgc = 0U; imgc < 1; imgc += 1)
    {
        auto composite_lock = composite_lock_initial + (composite_row++) * 64 * composite.row_pitch();

        //fill row by row
        for (auto i = 0; i < 64; ++i)
        {
            auto destination = composite_lock + i * composite.row_pitch();

            for (auto j = imgc; j < images.size(); ++j)
            {
                auto&&      img = images[j];
                auto img_pixels = img.pixels();
                auto img_lock = img_pixels.get_pixels_cpu();
                auto row = img_lock + i * img.row_pitch();
                memcpy_s(destination, img.row_pitch(), row, img.row_pitch());
                destination += img.row_pitch();
            }
        }
    }

    return composite;
}

static auto bake_images_bgra(const std::vector< uc::gx::imaging::cpu_texture  >& images)
{
    auto composite = uc::gx::imaging::make_image(static_cast<uint32_t>(images.size()) * 64, static_cast<uint32_t>(((1 + 7) / 8) * 64), uc::gx::imaging::image_type::r16_unorm);

    auto composite_pixels       = composite.pixels();
    auto composite_lock_initial = composite_pixels.get_pixels_cpu();

    auto composite_row = 0;
    for (auto imgc = 0U; imgc < 1; imgc += 1)
    {
        auto composite_lock = composite_lock_initial + (composite_row++) * 64 * composite.row_pitch();

        //fill row by row
        for (auto i = 0; i < 64; ++i)
        {
            auto destination = composite_lock + i * composite.row_pitch();

            for (auto j = imgc; j < images.size(); ++j)
            {
                auto&&      img = images[j];
                auto img_pixels = img.pixels();
                auto img_lock   = img_pixels.get_pixels_cpu();
                auto row        = img_lock + i * img.row_pitch();

                //bgra
                //auto write_pixels = destination;
                for (auto pixels = 0U; pixels < img.row_pitch() / 4; ++pixels)
                {
                    row++;                      //skip b
                    *destination++ = *row++;    //copy g
                    *destination++ = *row++;    //copy r
                    row++;                      //skip a
                }

                //memcpy_s(destination, img.row_pitch(), row, img.row_pitch());
                //destination += composite.row_pitch();
            }
        }
    }

    return composite;
}


static auto read_images(const media_source& source, blue_noise_type type)
{
    std::vector< uc::gx::imaging::cpu_texture> images;
    images.reserve(64);

    for (auto i = 0U; i < 64; ++i)
    {
        images.push_back(uc::gx::imaging::read_image(make_media_url(source, make_file_name(type, i)).get_path()));
    }

    return images;
}


int32_t main(int32_t argc, const char* argv[])
{
    try
    {
        std::cout << "Command line:" << std::endl;
        for (auto i = 0; i < argc; ++i)
        {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;

        uc::os::windows::com_initializer com;

        media_source source(L"../src/data//");

        uc::gx::imaging::write_image(bake_images( read_images(source, blue_noise_type::HDR_L ) ), make_file_name(blue_noise_type::HDR_L).c_str());
        uc::gx::imaging::write_image(bake_images(read_images(source, blue_noise_type::HDR_LA)), make_file_name(blue_noise_type::HDR_LA).c_str());
        uc::gx::imaging::write_image(bake_images(read_images(source, blue_noise_type::HDR_RGB)), make_file_name(blue_noise_type::HDR_RGB).c_str());
        uc::gx::imaging::write_image(bake_images(read_images(source, blue_noise_type::HDR_RGBA)), make_file_name(blue_noise_type::HDR_RGBA).c_str());

        uc::gx::imaging::write_image(bake_images(read_images(source, blue_noise_type::LDR_LLL1)), make_file_name(blue_noise_type::LDR_LLL1).c_str());
        uc::gx::imaging::write_image(bake_images_bgra(read_images(source, blue_noise_type::LDR_RG01)), make_file_name(blue_noise_type::LDR_RG01).c_str());
        uc::gx::imaging::write_image(bake_images(read_images(source, blue_noise_type::LDR_RGB1)), make_file_name(blue_noise_type::LDR_RGB1).c_str());
        uc::gx::imaging::write_image(bake_images(read_images(source, blue_noise_type::LDR_RGBA)), make_file_name(blue_noise_type::LDR_RGBA).c_str());
    }
    
    catch (const std::exception& e)
    {
        std::string  input_model = "uc_dev_bluenoise_r.exe";
        std::cerr << input_model << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

