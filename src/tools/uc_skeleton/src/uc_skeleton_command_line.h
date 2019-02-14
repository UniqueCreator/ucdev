#pragma once

#include <boost/program_options.hpp>

#include "uc_skeleton_exception.h"

namespace uc
{
    namespace skeleton
    {
        inline auto build_option_description()
        {
            namespace po = boost::program_options;

            // Declare the supported options.
            po::options_description desc("uc animation compiler. packs animations and skeletons into a format suitable for faster loading");

            desc.add_options()
                ("help,?", "produce help message")
                ("input_skeleton", po::value< std::string>(), "input skeleton")
                ("output_skeleton", po::value< std::string>(), "output skeleton")
                ("swap_y_z", po::value< bool >(), "swaps y and z coordinates, suitable for some tools like 3ds max")
                ("make_left_handed", po::value< bool >(), "swaps y and z coordinates, suitable for some tools like 3ds max");
                

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

        inline auto get_bool_option(const boost::program_options::variables_map & map, const std::string& o)
        {
            auto r = false;
            if (get_value_present(map, o))
            {
                r = get_input_value<bool>(map, o, [] {});
            }

            return r;
        }

        inline auto get_input_skeleton(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "input_skeleton", []
            {
                raise_error<exception>("missing input skeleton");
            }));
        }

        inline auto get_output_skeleton(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "output_skeleton", []
            {
                raise_error<exception>("missing output skeleton");
            }));
        }

        inline auto get_input_animation(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "input_animation", []
            {
                raise_error<exception>("missing input animation");
            }));
        }

        inline auto get_output_animation(const boost::program_options::variables_map & map)
        {
            return std::string(get_input_value< std::string >(map, "output_animation", []
            {
                raise_error<exception>("missing output animation");
            }));
        }

        inline auto get_make_left_handed(const boost::program_options::variables_map & map)
        {
            return get_bool_option(map, "make_left_handed");
        }
    }
}

