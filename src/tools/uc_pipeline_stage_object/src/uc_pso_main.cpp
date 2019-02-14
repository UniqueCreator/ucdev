// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"


#include "uc_pso_command_line.h"
#include "uc_pso_exception.h"
#include "uc_pso_parser.h"
#include "uc_pso_parser_utils.h"
#include "uc_pso_generator.h"

#include <fstream>
#include <streambuf>
#include <filesystem>

static std::string read_file(const std::string& file_name)
{
    using namespace std;
    ifstream s(file_name);

    if (!s.good())
    {
        uc::pso::raise_error<uc::pso::file_not_found_exception>(file_name);
    }

    return string( istreambuf_iterator<char>(s.rdbuf()), istreambuf_iterator<char>());
}

std::ostream& operator<<(std::ostream& o, const uc::pso::missing_input_exception& e)
{
    using namespace uc::pso;
    auto p = e.part();

    switch (p)
    {
        case missing_input_exception::missing_part::pipeline_state_object : 
            o << "pipeline state object";
        case missing_input_exception::missing_part::rasterizer_description:
            o << "rasterizer description";
        case missing_input_exception::missing_part::blend_description:
            o << "blend description";
        case missing_input_exception::missing_part::depth_stencil_description:
            o << "depth stencil description";
            break;
        default:
            o << "unknown missing part";
    }

    return o;
}

std::ostream& operator<<(std::ostream& o, const uc::pso::missing_variable_input_exception& e)
{
    using namespace uc::pso;
    auto p = e.part();

    switch (p)
    {
    case missing_input_exception::missing_part::pipeline_state_object:
        o << "pipeline state object";
    case missing_input_exception::missing_part::rasterizer_description:
        o << "rasterizer description";
    case missing_input_exception::missing_part::blend_description:
        o << "blend description";
    case missing_input_exception::missing_part::depth_stencil_description:
        o << "depth stencil description";
        break;
    default:
        o << "unknown missing part";
    }

    o << " variable " << e.variable();

    return o;
}

namespace
{

    template <typename result, typename parser >
    std::vector< result > parse_imports(const std::list<uc::pso::ast::import >& imports, const std::string& file_path, parser p )
    {
        std::vector< result > parsed_imports;
        auto imports_local = imports;

        while (!imports_local.empty())
        {
            auto& i             = imports_local.front();
            const auto name     = file_path + "/" + i.m_location.m_name + ".pso";
            auto  file          = read_file(name);
            //auto parsed = uc::pso::parse_graphics_pso(read_file(name), name);
            auto parsed = p(file, name);

            std::copy(std::begin(parsed.m_ast.m_imports), std::end(parsed.m_ast.m_imports), std::back_inserter(imports_local));
            imports_local.pop_front();
            parsed_imports.emplace_back(std::move(parsed));
        }

        return parsed_imports;
    }

    std::vector< uc::pso::graphics_parsed_unit > parse_graphics_imports(const std::list<uc::pso::ast::import >& imports, const std::string& file_path)
    {
        using namespace uc::pso;
        return parse_imports< graphics_parsed_unit, decltype (parse_graphics_pso )  > (imports, file_path, parse_graphics_pso);
    }

    std::vector< uc::pso::compute_parsed_unit > parse_compute_imports(const std::list<uc::pso::ast::import >& imports, const std::string& file_path)
    {
        using namespace uc::pso;
        return parse_imports<compute_parsed_unit, decltype (parse_compute_pso) >(imports, file_path, parse_compute_pso);
    }

    std::string pso_path( const std::string& file_name )
    {
        return std::experimental::filesystem::path(file_name).parent_path().generic_string();
    }


    template <typename parser, typename importer, typename generator>
    void generate_pso(const std::string& file_name, const boost::program_options::variables_map& vm, parser p, importer im, generator g)
    {
        using namespace uc::pso;

        auto main           = get_main_function(vm);
        auto header         = get_input_value(vm, "header");

        auto file           = read_file(file_name);
        auto compiled_unit  = p( file , file_name);
        auto parsed_imports = im( compiled_unit.m_ast.m_imports, pso_path(file_name) );

        //merge definitions
        for (auto&& i : parsed_imports)
        {
            std::copy(std::begin(i.m_ast.m_definitions), std::end(i.m_ast.m_definitions), std::back_inserter(compiled_unit.m_ast.m_definitions));
        }

        auto gen = g(compiled_unit, main, header);
        {
            auto header_path = get_header_file(vm);
            std::ofstream f(header_path, std::ios_base::binary);
            f << gen.m_header;
        }

        {
            auto cpp_path = get_cpp_file(vm);
            std::ofstream f(cpp_path, std::ios_base::binary);
            f << gen.m_cpp;
        }
    }
}


int32_t main(int32_t argc, const char* argv[])
{
    try
    {
        using namespace uc::pso;

        auto&& r   = build_option_map(argc, argv);
        auto&& vm  = std::get<0>(r);
        auto&& desc= std::get<1>(r);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        auto file_name  = get_input_file(vm);

        try
        {
            if (get_compute_pso(vm))
            {
                generate_pso(file_name, vm, uc::pso::parse_compute_pso, parse_compute_imports, generate_compute_pso_unit);
            }
            else
            {
                generate_pso(file_name, vm, uc::pso::parse_graphics_pso, parse_graphics_imports, generate_graphics_pso_unit);
            }

            /*


            auto compiled_unit  = uc::pso::parse_graphics_pso( read_file(file_name), file_name );
            auto parsed_imports = parse_graphics_imports(compiled_unit.m_ast.m_imports, pso_path(file_name) );

            //merge definitions
            for (auto&& i : parsed_imports)
            {
                std::copy(std::begin(i.m_ast.m_definitions), std::end(i.m_ast.m_definitions), std::back_inserter(compiled_unit.m_ast.m_definitions));
            }

            auto gen = uc::pso::generate_graphics_pso_unit(compiled_unit, main, get_input_value(vm, "header"));
            {
                auto header_path = get_header_file(vm);
                std::ofstream f(header_path, std::ios_base::binary);
                f << gen.m_header;
            }

            {
                auto cpp_path = get_cpp_file(vm);
                std::ofstream f(cpp_path, std::ios_base::binary);
                f << gen.m_cpp;
            }
            */

        }
        catch (const uc::pso::missing_variable_input_exception& e)
        {
            std::cerr << file_name << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e << "\r\n";
            return 1;
        }
        catch (const uc::pso::missing_input_exception& e)
        {
            std::cerr << file_name << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e <<  "\r\n";
            return 1;
        }
        catch (const uc::pso::input_exception& e)
        {
            std::cerr << file_name << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.option() << "\r\n";
            return 1;
        }
        catch (const uc::pso::file_not_found_exception& e)
        {
            std::cerr << file_name << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing file " << e.file_name() << "\r\n";
            return 1;
        }
        catch (...)
        {
            throw ;
        }

        return 0;
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        return 1;
    }
}

