// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "uc_pso_exception.h"

#include <iostream>
#include <tuple>
#include <vector>

#include <boost/program_options.hpp>
#include <filesystem>

namespace uc
{
    namespace pso
    {
        inline std::string get_abs_path(const std::string& file_name)
        {
            namespace fs = std::experimental::filesystem;
            return fs::canonical(fs::absolute( fs::path(file_name) ) ).string();
        }

        inline auto get_input_value(const boost::program_options::variables_map & map, const std::string& s)
        {
            using namespace std;
            if (!map.count(s))
            {
                uc::pso::raise_error<uc::pso::input_exception>(s);
                return string();
            }
            else
            {
                return *map[s].as< std::vector<string> >().begin();
            }
        }

        inline auto get_value_present(const boost::program_options::variables_map & map, const std::string& s)
        {
            using namespace std;
            return map.count(s);
        }

        inline auto get_input_path(const boost::program_options::variables_map & map, const std::string& s )
        {
            return get_abs_path( get_input_value(map,s) );
        }

        inline auto get_input_file(const boost::program_options::variables_map & map)
        {
            return get_input_path(map, "file");
        }

        inline auto get_main_function(const boost::program_options::variables_map & map)
        {
            return get_input_value(map, "main");
        }

        inline auto get_cpp_file(const boost::program_options::variables_map & map)
        {
            return get_input_path(map, "cpp");
        }

        inline auto get_header_file(const boost::program_options::variables_map & map)
        {
            return get_input_path(map, "header");
        }

        inline auto get_compute_pso(const boost::program_options::variables_map & map)
        {
            return get_value_present(map, "compute");
        }

        inline auto get_graphics_pso(const boost::program_options::variables_map & map)
        {
            return get_value_present(map, "graphics");
        }

        auto build_option_description()
        {
            namespace po = boost::program_options;

            // Declare the supported options.
            po::options_description desc("uc pipeline state object compiler. produces cpp and header fles from pso files. Available options");

            desc.add_options()
                ("help,?", "produce help message")
                ("file,c",  po::value< std::vector<std::string>> (), "file to process")
                ("cpp,o",   po::value< std::vector<std::string>> (), "cpp    file name")
                ("header,h", po::value< std::vector<std::string>>(), "header file name")
                ("main,m", po::value< std::vector< std::string>> (), "main function for a pso")
                ("graphics,g","graphics pipeline")
                ("compute,t", "compute pipeline")
                ;

            return desc;
        }

        inline auto build_option_map( const boost::program_options::options_description& desc, int32_t argc, const char* argv[] )
        {
            namespace po = boost::program_options;

            po::variables_map vm;

            po::store( po::command_line_parser(argc, argv)
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
    }
}



