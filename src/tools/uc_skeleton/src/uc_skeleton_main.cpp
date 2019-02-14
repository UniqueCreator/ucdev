// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <cstdint>
#include <iostream>

#include <uc_dev/gx/import/assimp/skinned_mesh.h>
#include <uc_dev/gx/import/fbx/skinned_mesh.h>
#include <uc_dev/gx/import/assimp/assimp_options.h>
#include <uc_dev/lzham/lzham_compressor.h>

#include "uc_skeleton_command_line.h"
#include "uc_skeleton_skeleton.h"
#include "uc_skeleton_options.h"
#include "uc_skeleton_lip.h"
#include "uc_skeleton_skeleton.h"


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
    using namespace uc::skeleton;
    
    try
    {
        std::cout << "Command line:" << std::endl;
        for (auto i = 0; i < argc; ++i)
        {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;

        auto&& om = build_option_map(argc, argv);
        auto&& vm = std::get<0>(om);
        auto&& desc = std::get<1>(om);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        //try skeleton
        auto input_skeleton                     = get_input_skeleton(vm);
        auto output_skeleton                    = get_output_skeleton(vm);
        auto make_left_handed                   = get_make_left_handed(vm);

        uint32_t ai_o = 0;
        ai_o |= make_left_handed ? aiProcess_MakeLeftHanded : 0;
        ai_o |= aiProcess_CalcTangentSpace; //

        std::cout << "building skeleton (" << get_environment() << ") " << input_skeleton << std::endl;
        std::cout << "assimp options:" << uc::gx::import::assimp::assimp_postprocess_option_to_string(ai_o) << std::endl;

        std::experimental::filesystem::path path(input_skeleton);
        auto e = path.extension().wstring();

        if (e == L".fbx")//  && false)
        {
            auto mesh = uc::gx::import::fbx::create_skinned_mesh(input_skeleton);
            uc::lip::serialize_object(uc::skeleton::skeleton(mesh->m_skeleton_pose), output_skeleton, uc::lzham::compress_buffer);
        }
        else
        {
            auto mesh = uc::gx::import::assimp::create_skinned_mesh(input_skeleton, ai_o);
            uc::lip::serialize_object(uc::skeleton::skeleton(mesh->m_skeleton_pose), output_skeleton, uc::lzham::compress_buffer);
        }
    }
    
    catch (const std::exception& e)
    {
        std::string  input_model = "uc_dev_skeleton_r.exe";
        std::cerr << input_model << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

