#pragma once

#include <boost/program_options.hpp>

#include "uc_build_file_generator_exception.h"

namespace uc
{
    namespace build_file_generator
    {
        enum mode : std::uint32_t
        {
            cpp,
            hlsl,
            pso,
            h
        };

        inline auto build_option_description()
        {
            namespace po = boost::program_options;

            // Declare the supported options.
            po::options_description desc("uc model compiler. packs 3d models into a format suitable for faster loading");

            desc.add_options()

                ("help,?", "produce help message")

                ("input,i", po::value< std::string>(),      "input directory, if missing the current directory is used")
                ("mode,t", po::value< std::string>(),       "mode:hlsl,pso,cpp,h")
                ("backend,b", po::value< std::string>(),    "graphics backend, dev or unique")
                ("type,t", po::value< std::string>(),       "type:project,filters filter or project")
                ("build,m", po::value< std::string>(),      "buildsystem:msbuild,fastbuild")
              ;
                
            return desc;
        }

        inline auto build_option_map(const boost::program_options::options_description& desc, int32_t argc, const char* argv[])
        {
            namespace po = boost::program_options;

            po::variables_map vm;

            po::store(po::command_line_parser(argc, argv)
                .options(desc)
                .style(po::command_line_style::default_style
                    | po::command_line_style::case_insensitive
                    | po::command_line_style::allow_slash_for_short
                    | po::command_line_style::allow_long_disguise)
                .run(), vm);

            po::notify(vm);
            return make_tuple(vm, desc);
        }

        inline auto build_option_map(int32_t argc, const char* argv[])
        {
            return build_option_map(build_option_description(), argc, argv);
        }

        template <typename t, typename e >
        inline auto get_input_value(const boost::program_options::variables_map & map, const std::string& s, e e)
        {
            using namespace std;
            if (!map.count(s))
            {
                e();
                return t();
            }
            else
            {
                return map[s].as< t >();
            }
        }

        template <typename t >
        inline auto get_input_value_optional(const boost::program_options::variables_map & map, const std::string& s, const t& default_value)
        {
            using namespace std;
            if (!map.count(s))
            {
                return default_value;
            }
            else
            {
                return map[s].as< t >();
            }
        }

        inline auto get_value_present(const boost::program_options::variables_map & map, const std::string& s)
        {
            using namespace std;
            return map.count(s);
        }

        inline auto get_input_directory(const boost::program_options::variables_map & map, const std::string& default_value)
        {
            return get_input_value_optional< std::string >(map, "input", default_value);
        }

        inline auto get_backend(const boost::program_options::variables_map & map, const std::string& default_value)
        {
            return get_input_value_optional< std::string >(map, "backend", default_value);
        }

        inline auto get_type(const boost::program_options::variables_map & map, const std::string& default_value)
        {
            return get_input_value_optional< std::string >(map, "type", default_value);
        }

        inline auto get_mode(const boost::program_options::variables_map & map)
        {
            auto s = get_input_value< std::string >(map, "mode", []
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::mode);
            });

            if (s == "cpp") return mode::cpp;
            if (s == "pso") return mode::pso;
            if (s == "hlsl") return mode::hlsl;
            if (s == "h") return mode::h;

            raise_error<missing_input_exception>(missing_input_exception::missing_part::mode);

            return mode::h;
        }

        /*
        inline auto get_model_type(const boost::program_options::variables_map & map)
        {
            auto r = std::string("default");
            if (get_value_present(map, "type"))
            {
                r = get_input_value<std::string>(map, "type", [] {});
            }
            return r;
        }

        inline auto get_textures(const boost::program_options::variables_map & map)
        {
            using ret_type = std::vector< std::string >;
            return ret_type(get_input_value< ret_type >(map, "textures", []
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::textures);
            }));
        }

        inline auto get_texture_formats(const boost::program_options::variables_map & map)
        {
            using ret_type = std::vector< std::string >;
            return ret_type(get_input_value< ret_type >(map, "textures_formats", []
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::textures_formats);
            }));
        }

        inline auto get_bool_option(const boost::program_options::variables_map & map, const std::string& o)
        {
            auto r = false;
            if (get_value_present(map, o))
            {
                r = get_input_value<bool>(map, o, [] {});
            }

            return r;
        }

        inline auto get_swap_y_z(const boost::program_options::variables_map & map)
        {
            return get_bool_option(map, "swap_y_z");
        }

        inline auto get_swap_faces(const boost::program_options::variables_map & map)
        {
            return get_bool_option(map, "swap_faces");
        }

        inline uint64_t get_assimp_option(const boost::program_options::variables_map & map)
        {
            const std::array<std::string, 27> o=
            {
                "calc_tangent_space",
                "join_identical_vertices",
                "make_left_handed",
                "triangulate",
                "remove_component",
                "gen_normals",
                "gen_smooth_normals",
                "split_large_meshes",
                "pre_transform_vertices",
                "limit_bone_weights",
                "validate_data_structure",
                "improve_cache_locality",
                "remove_redundant_materials",
                "fix_infacing_normals",
                "missed_by_assimp_dummy_enum",
                "sort_by_ptype",
                "find_degenerates",
                "find_invalid_data",
                "gen_uv_coords",
                "transform_uv_coords",
                "find_instances",
                "optimize_meshes",
                "optimize_graph",
                "flip_uvs",
                "flip_winding_order",
                "split_by_bone_count",
                "debone"
            };

            uint64_t r = 0;
            for (auto i = 0U; i < o.size(); ++i)
            {
                r |= get_bool_option(map, o[i]) ? ( 1 << (i) ) : 0;

            }

            return r;
        }
        */

    }
}

