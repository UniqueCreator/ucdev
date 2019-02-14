// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <cstdint>
#include <iostream>

#include <uc_dev/gx/import/anm/animation.h>
#include <uc_dev/gx/import/assimp/animation.h>
#include <uc_dev/gx/import/assimp/assimp_options.h>
#include <uc_dev/gx/import/assimp/skinned_mesh.h>
#include <uc_dev/lzham/lzham_compressor.h>

#include "uc_animation_command_line.h"
#include "uc_animation_options.h"
#include "uc_animation_lip.h"
#include "uc_animation_animation.h"

#include <uc_dev/gx/import/fbx/animation.h>


namespace uc
{
    namespace animation
    {

    }
}

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


int32_t main(int32_t argc, const char* argv[])
{
    using namespace uc::animation;
    
    try
    {
        auto&& om = build_option_map(argc, argv);
        auto&& vm = std::get<0>(om);
        auto&& desc = std::get<1>(om);

        std::cout << "Command line:" << std::endl;
        for (auto i = 0; i < argc; ++i)
        {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        auto input_animation    = get_input_animation(vm);
        auto output_animation   = get_output_animation(vm);

        std::cout << "building animation (" << get_environment() << ") " << input_animation << std::endl;
    
        auto make_left_handed = get_make_left_handed(vm);

        uint32_t ai_o = 0;
        ai_o |= make_left_handed ? aiProcess_MakeLeftHanded : 0;
        ai_o |= aiProcess_CalcTangentSpace; //
        std::cout << "assimp options:" << uc::gx::import::assimp::assimp_postprocess_option_to_string(ai_o) << std::endl;

        std::experimental::filesystem::path path(input_animation);
        auto e = path.extension().wstring();

        std::vector<uc::gx::import::anm::joint_animations> animations;

        if (e == L".fbx")
        {
            animations = uc::gx::import::fbx::create_animations(input_animation);
        }
        else
        {
            animations = uc::gx::import::assimp::create_animations_from_assimp(input_animation, ai_o);
        }

        {
            auto&& a = animations.front();
            //take the 1st one only
            uc::lip::serialize_object( uc::animation::animation(a), output_animation, uc::lzham::compress_buffer);
        }
    }
    
    catch (const std::exception& e)
    {
        std::string  input_model = "uc_dev_animation_r.exe";
        std::cerr << input_model << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

