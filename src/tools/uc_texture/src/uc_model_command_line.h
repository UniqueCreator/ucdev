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
            po::options_description desc("uc texture compiler. compresses a texture for use in the engine");

            desc.add_options()

                ("help,?", "produce help message")

                ("input_texture,i", po::value< std::string>(), "input texture")
                ("output_texture,o", po::value< std::string>(), "output texture")
                ("texture_format", po::value< std::string>(), "texture format ( unknown, bc1_unorm, bc1_unorm_srgb, bc2_unorm, bc2_unorm_srgb, bc3_unorm, bc3_unorm_srgb, bc4_unorm, bc4_snorm, bc5_unorm, bc5_snorm )")
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

        inline auto get_value_present(const boost::program_options::variables_map & map, const std::string& s)
        {
            using namespace std;
            return map.count(s);
        }

        inline auto get_input_texture(const boost::program_options::variables_map & map)
        {
            return std::string( get_input_value< std::string > (map, "input_texture", [ ]
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::input_file_name);
            }));
        }

        inline auto get_output_texture(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "output_texture", []
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::output_file_name);
            }));
        }

        inline auto get_texture_format(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "texture_format", []
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

    }
}

