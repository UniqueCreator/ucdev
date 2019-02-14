#pragma once

#include <boost/program_options.hpp>

#include "uc_model_exception.h"

namespace uc
{
    namespace model
    {
        inline auto build_option_description()
        {
            namespace po = boost::program_options;

            // Declare the supported options.
            po::options_description desc("uc model compiler. packs 3d models into a format suitable for faster loading");

            desc.add_options()

                ("help,?", "produce help message")

                ("input_model,i", po::value< std::string>(), "input 3d model")
                ("output_model,o", po::value< std::string>(), "output 3d model")
                ("type,t", po::value< std::string>(), "model type ( default, parametrized, textured, multi_textured, skinned )")

                ("textures", po::value< std::vector<std::string>>(), "textures for 3d model")

                ("textures_formats", po::value< std::vector<std::string> >(), "texture formats for 3d models (unknown, bc1_unorm, bc1_unorm_srgb, bc2_unorm, bc2_unorm_srgb, bc3_unorm, bc3_unorm_srgb, bc4_unorm, bc4_snorm, bc5_unorm, bc5_snorm)")

                /////
                ("calc_tangent_space", po::value< bool >(), "Calculates the tangents and bitangents for the imported meshes")
                ("join_identical_vertices", po::value< bool >(), "Identifies and joins identical vertex data sets within all imported meshes.")
                ("make_left_handed", po::value< bool >(), "Converts all the imported data to a left-handed coordinate space by negating z axis")
                ("triangulate", po::value< bool >(), "Triangulates all faces of all meshes.")
                ("remove_component", po::value< bool >(), "Removes some parts of the data structure (animations, materials,light sources, cameras, textures, vertex components).")
                ("gen_normals", po::value< bool >(), "Generates normals for all faces of all meshes.")
                ("gen_smooth_normals", po::value< bool >(), "Generates smooth normals for all vertices in the mesh.")
                ("split_large_meshes", po::value< bool >(), "Splits large meshes into smaller sub-meshes.")
                ("pre_transform_vertices", po::value< bool >(), "Removes the node graph and pre-transforms all vertices with the local transformation matrices of their nodes.")
                ("limit_bone_weights", po::value< bool >(), "Limits the number of bones simultaneously affecting a single vertex to a maximum value ( 4 is the default in our case).")
                ("validate_data_structure", po::value< bool >(), "Validates the imported scene data structure. This makes sure that all indices are valid, all animations and bones are linked correctly, all material references are correct .. etc.")
                ("improve_cache_locality", po::value< bool >(), "Reorders triangles for better vertex cache locality.")
                ("remove_redundant_materials", po::value< bool >(), "Searches for redundant/unreferenced materials and removes them.")
                ("fix_infacing_normals", po::value< bool >(), "This step tries to determine which meshes have normal vectors that are facing inwards and inverts them.")
                ("sort_by_ptype", po::value< bool >(), "This step splits meshes with more than one primitive type in homogeneous sub-meshes.")
                ("find_degenerates", po::value< bool >(), "This step searches all meshes for degenerate primitives and converts them to proper lines or points.")
                ("find_invalid_data", po::value< bool >(), "This step searches all meshes for invalid data, such as zeroed normal vectors or invalid UV coords and removes/fixes them. This is intended to get rid of some common exporter errors.")
                ("gen_uv_coords", po::value< bool >(), "This step converts non-UV mappings (such as spherical or cylindrical mapping) to proper texture coordinate channels.")
                ("transform_uv_coords", po::value< bool >(), "This step applies per-texture UV transformations and bakes them into stand-alone vtexture coordinate channels.")
                ("find_instances", po::value< bool >(), "This step searches for duplicate meshes and replaces them with references to the first mesh.")
                ("optimize_meshes", po::value< bool >(), "A postprocessing step to reduce the number of meshes.")
                ("optimize_graph", po::value< bool >(), "A postprocessing step to optimize the scene hierarchy.")
                ("flip_uvs", po::value< bool >(), "This step flips all UV coordinates along the y-axis and adjusts material settings and bitangents accordingly.")
                ("flip_winding_order", po::value< bool >(), "This step adjusts the output face winding order to be CW. The default face winding order is counter clockwise (CCW).")
                ("split_by_bone_count", po::value< bool >(), "This step splits meshes with many bones into sub-meshes so that each submesh has fewer or as many bones as a given limit.")
                ("debone", po::value< bool >(), "This step removes bones losslessly or according to some threshold.");
                
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

        inline auto get_value_present(const boost::program_options::variables_map & map, const std::string& s)
        {
            using namespace std;
            return map.count(s);
        }

        inline auto get_input_model(const boost::program_options::variables_map & map)
        {
            return std::string( get_input_value< std::string > (map, "input_model", [ ]
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::input_file_name);
            }));
        }

        inline auto get_output_model(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "output_model", []
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::output_file_name);
            }));
        }

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
        

    }
}

