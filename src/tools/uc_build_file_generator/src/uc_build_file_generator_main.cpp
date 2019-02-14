// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <algorithm>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <iostream>
#include <array>
#include <unordered_set>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <experimental/filesystem>

#include "uc_build_file_generator_command_line.h"
#include <uc_dev/mem/alloc.h>

inline std::string get_environment()
{
    return "x64";
}

std::string make_xml_tag_with_attribute(const char* tag, const char* attributeName, const char* value)
{
    return std::string("<") + std::string(tag) + std::string(" ") + std::string(attributeName) + std::string(" = \"") + std::string(value) + std::string("\"/>");
}

std::string make_xml_tag_with_attribute_begin(const char* tag, const char* attributeName, const char* value)
{
    return std::string("<") + std::string(tag) + std::string(" ") + std::string(attributeName) + std::string(" = \"") + std::string(value) + std::string("\">");
}

struct XmlTag
{
    const char* m_tag;

    XmlTag(const char* tag) : m_tag(tag)
    {
        std::cout << "\r\n<" << tag << ">";
    }

    ~XmlTag()
    {
        std::cout << "\r\n</" << m_tag << ">";
    }
};

struct XmlTag2
{
    const char* m_tag;

    XmlTag2(const char* tag) : m_tag(tag)
    {
        std::cout << "\r\n<" << tag << ">";
    }

    ~XmlTag2()
    {
        std::cout << "\r\n</" << m_tag << ">";
    }
};


struct XmlTagWithAttribute
{
    const char* m_tag;

    XmlTagWithAttribute(const char* tag, const char* attributeName, const char* value) : m_tag(tag)
    {
        std::cout << "\r\n" << make_xml_tag_with_attribute_begin(tag, attributeName, value);
    }

    ~XmlTagWithAttribute()
    {
        std::cout << "\r\n</" << m_tag << ">";
    }
};

struct XmlTagWithAttribute2
{
    const char* m_tag;

    XmlTagWithAttribute2(const char* tag, const char* attributeName, const char* value) : m_tag(tag)
    {
        std::cout << "\r\n" << make_xml_tag_with_attribute(tag, attributeName, value);
    }

    ~XmlTagWithAttribute2()
    {
    }
};





namespace buckets
{
    enum type : uint32_t
    {
        cpp,
        pch,
        header,
        root_signature,
        vertex_shader,
        pixel_shader,
        compute_shader,
        graphics_pso,
        compute_pso,
        unknown,
        count,
    };

    constexpr uint32_t bucket_count = count;

    struct conainer
    {
        std::array < std::vector< std::experimental::filesystem::path > , bucket_count > m_buckets;
    };

    static bool is_root_signature(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), "_signature.hlsl");
    }

    static bool is_vertex_shader(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), "_vertex.hlsl");
    }

    static bool is_pixel_shader(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), "_pixel.hlsl");
    }

    static bool is_compute_shader(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), "_compute.hlsl");
    }

    static bool is_graphics_pso(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), "_graphics.pso");
    }

    static bool is_compute_pso(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), "_compute.pso");
    }

    static bool is_cpp(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), ".cpp");
    }

    static bool is_pch(const std::experimental::filesystem::path& p)
    {
        return 	p.filename().string() == "pch.cpp";
    }

    static bool is_header(const std::experimental::filesystem::path& p)
    {
        return 	boost::algorithm::ends_with(p.filename().string(), ".h");
    }

    type get_file_bucket(const std::experimental::filesystem::path& p)
    {
        if (is_pch(p))
        {
            return buckets::pch;
        }
        else if (is_cpp(p))
        {
            return buckets::cpp;
        }
        else if (is_header(p))
        {
            return buckets::header;
        }
        else if (is_root_signature(p))
        {
            return buckets::root_signature;
        }
        else if (is_vertex_shader(p))
        {
            return buckets::vertex_shader;
        }
        else if (is_pixel_shader(p))
        {
            return 	buckets::pixel_shader;
        }
        else if (is_compute_shader(p))
        {
            return buckets::compute_shader;
        }
        else if (is_graphics_pso(p))
        {
            return 	buckets::graphics_pso;
        }
        else if (is_compute_pso(p))
        {
            return buckets::compute_pso;
        }
        else
        {
            return buckets::unknown;
        }
    }

    template <typename it>
    conainer make_container(it begin, it end)
    {
        conainer r;

        for (auto&& i = begin; i != end; ++i)
        {
            type t = get_file_bucket(*i);

            r.m_buckets[t].push_back(*i);
        }

        return r;
    }
}

const std::string make_filter(const std::experimental::filesystem::path& p, const bool insert_file = false)
{
    std::experimental::filesystem::path o = p;
    std::experimental::filesystem::path dot_dot = "..";
    std::vector<std::string> paths;

    if (insert_file)
    paths.push_back(o.stem().string());

    while (o.has_parent_path() && o.parent_path().stem() != dot_dot)
    {
        o = o.parent_path();
        paths.push_back(o.stem().string());
    }

    std::string r;
    auto s = paths.crbegin();
    std::string c = "\\";
    for (; s != paths.crend()-1; ++s)
    {
        r += *s;
        r += c;
    }

    if (s != paths.crend())
    {
        r += *s;
    }
    return r;
}


void visit_filters(const std::experimental::filesystem::path& p, std::unordered_set<std::string>& filters)
{
    std::experimental::filesystem::path o = p;
    std::experimental::filesystem::path dot_dot = "..";
    std::vector<std::string> paths;

    filters.insert(make_filter(o, false));

    while (o.has_parent_path() && o.parent_path().stem() != dot_dot)
    {
        o = o.parent_path();
        filters.insert(make_filter(o, true));
    }
}



namespace hlsl
{
    static std::string build_hlsl_tag(const std::experimental::filesystem::path& shader, const std::string& shader_type, const std::string& backend)
    {
        return std::string("\r\n<Shader Include = \"") + shader.string() + std::string("\"><ShaderPipelineStage>") + shader_type + std::string("</ShaderPipelineStage>") + std::string("<Backend>") + backend + std::string("</Backend>") + std::string("</Shader>");
    }

    static std::string build_hlls_tag_filter(const std::experimental::filesystem::path& shader, const std::string& shader_type, const std::string& backend)
    {
        shader_type;
        backend;
        std::string r = std::string("\r\n<Shader Include = \"") + shader.string() + std::string("\">");
        r +=std::string("<Filter>") + make_filter(shader) + std::string("</Filter>");
        r +=std::string("</Shader>");
        return r;
    }

    template <typename tag_function, typename it>
    static void process_shaders(it begin, it end, const std::string& backend, tag_function tag, const std::string& shader_type)
    {
        std::vector<std::string> lines;

        for (auto&& i = begin ; i!= end; ++i)
        {
            lines.push_back(tag(*i, shader_type, backend));
        }

        std::sort(lines.begin(), lines.end());

        XmlTag t("ItemGroup");

        for (auto&& l : lines)
        {
            std::cout <<"\t" << l;
        }
    }

    template <typename tag_function, typename it>
    static void process_signatures(it begin, it end, const std::string& backend, tag_function tag)
    {
        process_shaders(begin, end, backend, tag, "RootSignature");
    }

    template <typename tag_function, typename it>
    static void process_vertex_shaders(it begin, it end, const std::string& backend, tag_function tag)
    {
        process_shaders(begin, end, backend, tag, "Vertex");
    }

    template <typename tag_function, typename it>
    static void process_pixel_shaders(it begin, it end, const std::string& backend, tag_function tag)
    {
        process_shaders(begin, end, backend, tag, "Pixel");
    }

    template <typename tag_function, typename it>
    static void process_compute_shaders(it begin, it end, const std::string& backend, tag_function tag)
    {
        process_shaders(begin, end, backend, tag, "Compute");
    }
}

namespace pso
{
    static std::string build_graphics_pso_tag(const std::experimental::filesystem::path& shader, const std::string& backend)
    {
        std::string entry_point_name = shader.stem().string();
        return std::string("\r\n<GraphicsPipelineStateObject Include = \"") + shader.string() + std::string("\"><Backend>") + backend + std::string("</Backend>") + std::string("<EntryPointName>") + entry_point_name + std::string("</EntryPointName>") + std::string("</GraphicsPipelineStateObject>");
    }

    static std::string build_graphics_pso_tag_filter(const std::experimental::filesystem::path& shader, const std::string& backend)
    {
        backend;
        std::string r = std::string("\r\n<GraphicsPipelineStateObject Include = \"") + shader.string() + std::string("\">");
        r += std::string("<Filter>") + make_filter(shader) + std::string("</Filter>");
        r += std::string("</GraphicsPipelineStateObject>");
        return r;
    }

    static std::string build_compute_pso_tag_filter(const std::experimental::filesystem::path& shader, const std::string& backend)
    {
        backend;
        std::string r = std::string("\r\n<ComputePipelineStateObject Include = \"") + shader.string() + std::string("\">");
        r += std::string("<Filter>") + make_filter(shader) + std::string("</Filter>");
        r += std::string("</ComputePipelineStateObject>");
        return r;
    }

    static std::string build_compute_pso_tag(const std::experimental::filesystem::path& shader, const std::string& backend)
    {
        std::string entry_point_name = shader.stem().string();
        return std::string("\r\n<ComputePipelineStateObject Include = \"") + shader.string() + std::string("\"><Backend>") + backend + std::string("</Backend>") + std::string("<EntryPointName>") + entry_point_name + std::string("</EntryPointName>") + std::string("</ComputePipelineStateObject>");
    }

    template <typename tag_function, typename it>
    static void process_pso(it begin, it end, const std::string& backend, tag_function tag)
    {
        std::vector<std::string> lines;

        for (auto&& i = begin; i != end; ++i)
        {
            lines.push_back(tag(*i, backend));
        }

        std::sort(lines.begin(), lines.end());

        XmlTag t("ItemGroup");

        for (auto&& l : lines)
        {
            std::cout << "\t" << l;
        }
    }
}

namespace cpp
{
    static std::string build_cpp_tag(const std::experimental::filesystem::path& shader)
    {
        return std::string("\r\n<ClCompile Include = \"") + shader.string() + std::string("\" />");
    }

    static std::string build_cpp_tag_filter(const std::experimental::filesystem::path& shader)
    {
        std::string r = std::string("\r\n<ClCompile Include = \"") + shader.string() + std::string("\">");
        r += std::string("<Filter>") + make_filter(shader) + std::string("</Filter>");
        r += std::string("</ClCompile>");
        return r;
    }

    static std::string build_pch_tag(const std::experimental::filesystem::path& shader)
    {
        return std::string("\r\n<ClCompile Include = \"") + shader.string() + std::string("\">") + std::string("<PrecompiledHeader>Create</PrecompiledHeader>") + std::string("</ClCompile>");
    }

    static std::string build_pch_tag_filter(const std::experimental::filesystem::path& shader)
    {
        return build_cpp_tag_filter(shader);
    }

    template <typename tag_function, typename it>
    static void process_cpp(it begin, it end, tag_function tag)
    {
        std::vector<std::string> lines;

        for (auto&& i = begin; i != end; ++i)
        {
            lines.push_back(tag(*i));
        }

        std::sort(lines.begin(), lines.end());

        XmlTag t("ItemGroup");
        for (auto&& l : lines)
        {
            std::cout << "\t" << l;
        }
    }
}


namespace header
{
    static std::string build_header_tag(const std::experimental::filesystem::path& shader)
    {
        return std::string("\r\n<ClInclude Include = \"") + shader.string() + std::string("\"/>");
    }

    static std::string build_header_tag_filter(const std::experimental::filesystem::path& shader)
    {
        std::string r = std::string("\r\n<ClInclude Include = \"") + shader.string() + std::string("\">");
        r += std::string("<Filter>") + make_filter(shader) + std::string("</Filter>");
        r += std::string("</ClInclude>");
        return r;
    }

    template <typename tag_function, typename it>
    static void process_header(it begin, it end, tag_function tag)
    {
        std::vector<std::string> lines;

        for (auto&& i = begin; i != end; ++i)
        {
            lines.push_back(tag(*i));
        }

        std::sort(lines.begin(), lines.end());

        {
            XmlTag t("ItemGroup");

            for (auto&& l : lines)
            {
                std::cout << "\t" << l;
            }
        }
    }
}

std::string make_uuid()
{
    std::string r;

    // Create a new uuid
    UUID uuid;
    RPC_STATUS ret_val = ::UuidCreate(&uuid);

    if (ret_val == RPC_S_OK)
    {
        // convert UUID to LPWSTR
        CHAR* wszUuid = NULL;
        ::UuidToStringA(&uuid, (RPC_CSTR*)&wszUuid);
        if (wszUuid != NULL)
        {
            //TODO: do something with wszUuid
            r = wszUuid;
            // free up the allocated string
            ::RpcStringFreeA((RPC_CSTR*)&wszUuid);
        }
    }

    return r;
}

template <typename it> void process_filters( it begin, it end)
{
    std::unordered_set< std::string > filters;

    for (auto&& i = begin;i != end;++i)
    {
        visit_filters(*i, filters);
    }

    std::vector<std::string> sorted_filters;
    sorted_filters.resize(filters.size());
    std::copy(filters.cbegin(), filters.cend(), sorted_filters.begin());

    std::sort(sorted_filters.begin(), sorted_filters.end());


    {
        XmlTag t("ItemGroup");
        
        for (auto& f : sorted_filters)
        {
            XmlTagWithAttribute2 tag0("Filter", "Include", f.c_str());
            //XmlTag2             tag01("UniqueIdentifier");
            //std::cout << "{" << make_uuid() << "}";
        }
    }

}

int32_t main(int32_t argc, const char* argv[])
{
    using namespace uc::build_file_generator;

    
    std::string input_model_error = "uc_dev_cpp_source_generator_r.exe";

    try
    {
        auto&& om   = build_option_map(argc, argv);
        auto&& vm   = std::get<0>(om);
        auto&& desc = std::get<1>(om);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        namespace fs = std::experimental::filesystem ;

        auto input_dir = fs::path(get_input_directory(vm, fs::current_path().string()));
        auto backend    = get_backend(vm, "dev");
        auto project    = get_type(vm, "project");

        if (backend == "dev")
        {
            backend = "UniqueCreatorDev";
        }
        else
        {
            backend = "UniqueCreatorPublic";
        }

        auto mode       = get_mode(vm);

        std::string extension = "";

        switch (mode)
        {
            case mode::cpp:     extension = ".cpp"; break;
            case mode::hlsl:    extension = ".hlsl"; break;
            case mode::pso:     extension = ".pso"; break;
            case mode::h:       extension = ".h"; break;
        };

        std::vector< fs::path > files;

        if ( fs::exists(input_dir) && fs::is_directory( input_dir) )
        {
            fs::recursive_directory_iterator dir(input_dir);

            for (auto&& v : dir)
            {
                fs::path file = v;
                
                if ( fs::is_regular_file(file) && file.extension() == extension)
                {
                    files.push_back(file);

                }
            }
        }

        if (!files.empty())
        {
            buckets::conainer c = buckets::make_container(files.begin(), files.end());

            bool is_project = (project == "project");

            if ( mode == mode::hlsl )
            {
                {
                    const auto& b = c.m_buckets[buckets::root_signature];
                    
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            hlsl::process_signatures(b.cbegin(), b.cend(), backend, hlsl::build_hlsl_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            hlsl::process_signatures(b.cbegin(), b.cend(), backend, hlsl::build_hlls_tag_filter);
                        }
                    }
                }

                {
                    const auto& b = c.m_buckets[buckets::vertex_shader];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            hlsl::process_vertex_shaders(b.cbegin(), b.cend(), backend, hlsl::build_hlsl_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            hlsl::process_vertex_shaders(b.cbegin(), b.cend(), backend, hlsl::build_hlls_tag_filter);
                        }
                    }
                }
                {
                    const auto& b = c.m_buckets[buckets::pixel_shader];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            hlsl::process_pixel_shaders(b.cbegin(), b.cend(), backend, hlsl::build_hlsl_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            hlsl::process_pixel_shaders(b.cbegin(), b.cend(), backend, hlsl::build_hlls_tag_filter);
                        }
                    }

                }

                {
                    const auto& b = c.m_buckets[buckets::compute_shader];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            hlsl::process_compute_shaders(b.cbegin(), b.cend(), backend, hlsl::build_hlsl_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            hlsl::process_compute_shaders(b.cbegin(), b.cend(), backend, hlsl::build_hlls_tag_filter);
                        }
                    }
                }
            }

            if (mode == mode::pso)
            {
                {
                    const auto& b = c.m_buckets[buckets::graphics_pso];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            pso::process_pso(b.cbegin(), b.cend(), backend, pso::build_graphics_pso_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            pso::process_pso(b.cbegin(), b.cend(), backend, pso::build_graphics_pso_tag_filter);
                        }
                    }
                }

                {
                    const auto& b = c.m_buckets[buckets::compute_pso];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            pso::process_pso(b.cbegin(), b.cend(), backend, pso::build_compute_pso_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            pso::process_pso(b.cbegin(), b.cend(), backend, pso::build_compute_pso_tag_filter);
                        }
                    }
                }

            }

            if (mode == mode::cpp)
            {
                {
                    const auto& b = c.m_buckets[buckets::pch];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            cpp::process_cpp(b.cbegin(), b.cend(), cpp::build_pch_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            cpp::process_cpp(b.cbegin(), b.cend(), cpp::build_pch_tag_filter);
                        }
                    }
                }

                {
                    const auto& b = c.m_buckets[buckets::cpp];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            cpp::process_cpp(b.cbegin(), b.cend(), cpp::build_cpp_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            cpp::process_cpp(b.cbegin(), b.cend(), cpp::build_cpp_tag_filter);
                        }
                    }
                }
            }

            if (mode == mode::h)
            {
                {
                    const auto& b = c.m_buckets[buckets::header];
                    if (is_project)
                    {
                        if (!b.empty())
                        {
                            header::process_header(b.cbegin(), b.cend(), header::build_header_tag);
                        }
                    }
                    else
                    {
                        if (!b.empty())
                        {
                            process_filters(b.cbegin(), b.cend());
                            header::process_header(b.cbegin(), b.cend(), header::build_header_tag_filter);
                        }
                    }
                }
            }
        }
        else
        {
            //todo:
            return -1;
        }
    }

    catch (const std::exception& e)
    {
        std::cerr << input_model_error << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

