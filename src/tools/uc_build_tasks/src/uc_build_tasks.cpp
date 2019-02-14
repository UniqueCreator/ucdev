// This is the main DLL file.

#include "pch.h"
#include "uc_build_tasks.h"

#include <fstream>

#include <string>


#include "uc_build_tasks_build_caches.h"
#include "uc_build_tasks_fx_spawner.h"
#include "uc_build_tasks_shaders.h"

#include "uc_build_tasks_string_helpers.h"
#include "uc_build_tasks_string_helpers_managed.h"
#include "uc_build_tasks_error.h"


#include <boost/wave/cpp_exceptions.hpp>
#include <filesystem>

#include <d3dcompiler.h>
#include <d3dcommon.h>

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            using namespace uc::build::tasks;
            using namespace Microsoft::Build::Utilities;

            namespace
            {
                template <typename container>
                size_t count(container c) { static_assert(fale); return 0; };

                template < > size_t count( array<ITaskItem^>^ c )
                {
                    return c->Length;
                }

                template <> size_t count< > ( List<ITaskItem^>^ c)
                {
                    return c->Count;
                }

                template <typename container> 
                std::vector< std::string > file_names(container^ sources)
                {
                    std::vector< std::string > file_names;
                    auto length = count(sources);

                    file_names.reserve( length );

                    for (auto i = 0U; i < length; ++i)
                    {
                        file_names.emplace_back(marshal(sources[i]->ItemSpec));
                    }

                    return file_names;
                }

                static uc::build::tasks::shader_pipeline_stage to_pipeline_stage(const std::string& s)
                {
                    const std::array<shader_pipeline_stage, 7>  stages =
                    {
                        shader_pipeline_stage::pixel,
                        shader_pipeline_stage::vertex,
                        shader_pipeline_stage::geometry,
                        shader_pipeline_stage::hull,
                        shader_pipeline_stage::domain,
                        shader_pipeline_stage::compute,
                        shader_pipeline_stage::root_signature
                    };
                    
                    const std::array<std::string, 7> stages_text =
                    {
                        "Pixel",
                        "Vertex",
                        "Geometry",
                        "Hull",
                        "Domain",
                        "Compute",
                        "RootSignature"
                    };

                    for (auto i = 0U; i < stages.size(); ++i)
                    {
                        if (s == stages_text[i])
                        {
                            return stages[i];
                        }
                    }

                    return shader_pipeline_stage::vertex;
                }

                template <typename container>
                std::vector< uc::build::tasks::shader_pipeline_stage > stage_types(container^ sources)
                {
                    std::vector< uc::build::tasks::shader_pipeline_stage > stages;
                    auto length = count(sources);

                    stages.reserve(length);

                    for (auto i = 0U; i < length; ++i)
                    {
                        System::String^ shader = sources[i]->GetMetadata("ShaderPipelineStage");

                        if ( shader == nullptr || shader->Length == 0)
                        {
                            stages.emplace_back( uc::build::tasks::vertex );
                        }
                        else
                        {
                            stages.emplace_back( to_pipeline_stage( marshal(shader)  ) );
                        }
                       
                    }

                    return stages;
                }

                static std::vector< std::string > append_extension(const std::vector< std::string> file_names, const std::string& extension)
                {
                    std::vector< std::string > result;
                    result.reserve(file_names.size());

                    for (auto&& i : file_names)
                    {
                        result.emplace_back(i + std::string(".") + extension);
                    }

                    return result;
                }

                static std::string append_extension(const std::string file_name, const std::string& extension)
                {
                    return file_name + "." + extension;
                }

                template < typename container >
                std::string concatenate_options(const container& o, const std::string& delimiter = " ")
                {
                    std::string result;

                    for (auto&& i : o)
                    {
                        result += i;
                        result += delimiter;
                    }

                    return result;
                }

                template < typename container1, typename container2 >
                std::string merge_options(const container1& o1, const container2& o2, const std::string& delimiter = " ")
                {
                    std::string result;

                    for (auto i = 0U; i < o1.size(); ++i)
                    {
                        result += o1[i];
                        result += delimiter;
                        result += o2[i];
                        result += delimiter;
                    }

                    return result;
                }

                static std::string stage_to_command_line2(uc::build::tasks::shader_pipeline_stage s)
                {
                    switch (s)
                    {
                    case shader_pipeline_stage::pixel:
                    {
                        return std::string("ps_5_1");
                    }

                    case shader_pipeline_stage::vertex:
                    {
                        return std::string("vs_5_1");
                    }

                    case shader_pipeline_stage::geometry:
                    {
                        return std::string("gs_5_1");
                    }

                    case shader_pipeline_stage::hull:
                    {
                        return std::string("hs_5_1");
                    }

                    case shader_pipeline_stage::domain:
                    {
                        return std::string("s_5_1");
                    }

                    case shader_pipeline_stage::compute:
                    {
                        return std::string("cs_5_1");
                    }

                    case shader_pipeline_stage::root_signature:
                    {
                        return std::string("rootsig_1_0");
                    }

                    default:
                    {
                        return std::string("vs_5_1");
                    }
                    }
                }

                static std::string stage_to_command_line(uc::build::tasks::shader_pipeline_stage s)
                {
                    return std::string("/T ") + stage_to_command_line2(s);
                }

                static std::string make_option( const std::vector<std::string>& data, const std::string& prefix, const std::string& delimiter = " ")
                {
                    std::string result;
                    
                    for (auto i = 0U; i < data.size(); ++i)
                    {
                        result += prefix;
                        result += delimiter;
                        result += data[i];
                        result += delimiter;
                    }

                    return result;
                }

                static inline std::string make_includes(const std::vector<std::string>& data)
                {
                    return make_option(data, "/I");
                }

                static inline std::string make_macros(const std::vector<std::string>& data)
                {
                    return make_option(data, "/D");
                }

                System::String^ build_command_line
                (
                    const std::string& shader_file_name,
                    const std::string& shader_byte_code,
                    const std::string& pdb_name,
                    const std::string& header_file_name,
                    uc::build::tasks::shader_pipeline_stage stage,
                    std::vector< std::string > includes,
                    std::vector< std::string > macros
                )
                {
                    std::array< std::string, 5  > options_prefix =
                    {
                        "",
                        "/Fo",
                        "/Fd",
                        "/Fh",
                        "/Vn"
                    };

                    std::array< std::string, 5  > options_data =
                    {
                        shader_file_name,
                        shader_byte_code,
                        pdb_name,
                        header_file_name,
                        std::string("g_main")
                    };

                    std::array< std::string, 4  >  options_additional1 =
                    {
                        "/Qstrip_priv",
                        "/Qstrip_rootsignature",
                        "/O3",
                        "/Zi",
                    };

                    std::array< std::string, 6  >  options_additional2 =
                    {
                        "/Qstrip_reflect",
                        "/Qstrip_debug",
                        "/Qstrip_priv",
                        "/E MyRS1",
                        "/O3",
                        "/Zi"
                    };

                    auto&& r0 = merge_options(options_prefix, options_data);
                    std::string r1;

                    if (stage == shader_pipeline_stage::root_signature)
                    {
                        r1 = concatenate_options(options_additional2);
                    }
                    else
                    {
                        r1 = concatenate_options(options_additional1);
                    }

                    auto&& r2 = make_includes(includes);
                    auto&& r3 = make_macros(macros);
                    auto&& r4 = stage_to_command_line(stage);

                    auto r = marshal(r0 + r1 + r2 + r3 + r4);
                    return r;
                }

                template <typename container>
                List<ITaskItem^>^  build_task_items(const container& file_names)
                {
                    List<ITaskItem^>^ result = gcnew List<ITaskItem^>();
                 
                    for (auto&& i : file_names)
                    {
                        ITaskItem^ item = gcnew TaskItem(from_string ( i ) );
                        result->Add(item);
                    }

                    return result;
                }

                inline std::string optional(const std::string& s, const::std::string& o)
                {
                    if (!s.empty())
                    {
                        return s;
                    }
                    else
                    {
                        return o;
                    }
                }

                static System::String^ stream_marshal(System::IO::Stream^ stream)
                {
                    System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(stream, System::Text::Encoding::UTF8);
                    return reader->ReadToEnd();
                }

                System::Tuple < System::String^, System::String^ >^ cpp_generate_find_templates(uc::build::tasks::shader_pipeline_stage stage, Backend b)
                {
                    using namespace uc::build::tasks;

                    array<System::String^>^ cpp_name_backend_private    = gcnew array<System::String^>(7);
                    array<System::String^>^ header_name_backend_private = gcnew array<System::String^>(7);

                    array<System::String^>^ cpp_name_backend_public     = gcnew array<System::String^>(7);
                    array<System::String^>^ header_name_backend_public  = gcnew array<System::String^>(7);

                    array<array<System::String^>^>^ backend_cpp         = gcnew array< array<System::String^> ^ >(2);
                    array<array<System::String^>^>^ backend_header      = gcnew array< array<System::String^> ^ >(2);

                    backend_cpp[0]      = cpp_name_backend_private;
                    backend_cpp[1]      = cpp_name_backend_public;

                    backend_header[0]   = header_name_backend_private;
                    backend_header[1]   = header_name_backend_public;
                   
                    cpp_name_backend_private[shader_pipeline_stage::pixel]             = gcnew System::String("dx12_gpu_shaders_myshader_type.cpp.pixel.txt");
                    cpp_name_backend_private[shader_pipeline_stage::vertex]            = gcnew System::String("dx12_gpu_shaders_myshader_type.cpp.vertex.txt");
                    cpp_name_backend_private[shader_pipeline_stage::geometry]          = gcnew System::String("dx12_gpu_shaders_myshader_type.cpp.geometry.txt");

                    cpp_name_backend_private[shader_pipeline_stage::hull]              = gcnew System::String("dx12_gpu_shaders_myshader_type.cpp.hull.txt");
                    cpp_name_backend_private[shader_pipeline_stage::domain]            = gcnew System::String("dx12_gpu_shaders_myshader_type.cpp.domain.txt");
                    cpp_name_backend_private[shader_pipeline_stage::compute]           = gcnew System::String("dx12_gpu_shaders_myshader_type.cpp.compute.txt");
                    cpp_name_backend_private[shader_pipeline_stage::root_signature]    = gcnew System::String("dx12_gpu_root_signatures_myshader_type.cpp.txt");
                    //--------------------------------------------------------------------------------------------------------------------------------------------
                    header_name_backend_private[shader_pipeline_stage::pixel]          = gcnew System::String("dx12_gpu_shaders_myshader_type.h.pixel.txt");
                    header_name_backend_private[shader_pipeline_stage::vertex]         = gcnew System::String("dx12_gpu_shaders_myshader_type.h.vertex.txt");
                    header_name_backend_private[shader_pipeline_stage::geometry]       = gcnew System::String("dx12_gpu_shaders_myshader_type.h.geometry.txt");

                    header_name_backend_private[shader_pipeline_stage::hull]           = gcnew System::String("dx12_gpu_shaders_myshader_type.h.hull.txt");
                    header_name_backend_private[shader_pipeline_stage::domain]         = gcnew System::String("dx12_gpu_shaders_myshader_type.h.domain.txt");
                    header_name_backend_private[shader_pipeline_stage::compute]        = gcnew System::String("dx12_gpu_shaders_myshader_type.h.compute.txt");
                    header_name_backend_private[shader_pipeline_stage::root_signature] = gcnew System::String("dx12_gpu_root_signatures_myshader_type.h.txt");
                    //--------------------------------------------------------------------------------------------------------------------------------------------
                    cpp_name_backend_public[shader_pipeline_stage::pixel]               = gcnew System::String("unique_creator_graphics_myshader_type.cpp.pixel.txt");
                    cpp_name_backend_public[shader_pipeline_stage::vertex]              = gcnew System::String("unique_creator_graphics_myshader_type.cpp.vertex.txt");
                    cpp_name_backend_public[shader_pipeline_stage::geometry]            = gcnew System::String("unique_creator_graphics_myshader_type.cpp.geometry.txt");

                    cpp_name_backend_public[shader_pipeline_stage::hull]                = gcnew System::String("unique_creator_graphics_myshader_type.cpp.hull.txt");
                    cpp_name_backend_public[shader_pipeline_stage::domain]              = gcnew System::String("unique_creator_graphics_myshader_type.cpp.domain.txt");
                    cpp_name_backend_public[shader_pipeline_stage::compute]             = gcnew System::String("unique_creator_graphics_myshader_type.cpp.compute.txt");
                    cpp_name_backend_public[shader_pipeline_stage::root_signature]      = gcnew System::String("unique_creator_graphics_root_signatures_myshader_type.cpp.txt");
                    //--------------------------------------------------------------------------------------------------------------------------------------------
                    header_name_backend_public[shader_pipeline_stage::pixel]            = gcnew System::String("unique_creator_graphics_myshader_type.h.pixel.txt");
                    header_name_backend_public[shader_pipeline_stage::vertex]           = gcnew System::String("unique_creator_graphics_myshader_type.h.vertex.txt");
                    header_name_backend_public[shader_pipeline_stage::geometry]         = gcnew System::String("unique_creator_graphics_myshader_type.h.geometry.txt");

                    header_name_backend_public[shader_pipeline_stage::hull]             = gcnew System::String("unique_creator_graphics_myshader_type.h.hull.txt");
                    header_name_backend_public[shader_pipeline_stage::domain]           = gcnew System::String("unique_creator_graphics_myshader_type.h.domain.txt");
                    header_name_backend_public[shader_pipeline_stage::compute]          = gcnew System::String("unique_creator_graphics_myshader_type.h.compute.txt");
                    header_name_backend_public[shader_pipeline_stage::root_signature]   = gcnew System::String("unique_creator_graphics_root_signatures_myshader_type.h.txt");
                    //--------------------------------------------------------------------------------------------------------------------------------------------
                    auto index                                                          = static_cast<uint32_t> (stage);
                    auto backend                                                        = static_cast<uint32_t> (b);

                    return gcnew System::Tuple<  System::String^, System::String^ >(backend_cpp[backend][index], backend_header[backend][index] );
                }

                static System::Tuple < System::String^, System::String^ >^ cpp_generate_load_templates(uc::build::tasks::shader_pipeline_stage stage, Backend b)
                {
                    auto files          = cpp_generate_find_templates(stage, b);
                    auto assembly       = System::Reflection::Assembly::GetExecutingAssembly();
                    auto cpp_name       = stream_marshal(assembly->GetManifestResourceStream(files->Item1));
                    auto header_name    = stream_marshal(assembly->GetManifestResourceStream(files->Item2));

                    return gcnew System::Tuple<  System::String^, System::String^ >( cpp_name, header_name );
                }

                static std::tuple< std::string, std::string > cpp_generate_get_templates( uc::build::tasks::shader_pipeline_stage stage, Backend b)
                {
                    auto   files        = cpp_generate_load_templates(stage, b);
                    auto&& cpp_name     = marshal(files->Item1);
                    auto&& header_name  = marshal(files->Item2);
                    return std::make_tuple(cpp_name, header_name);
                }

                static std::string replace_string(const std::string& base, const::std::string& search, const::std::string& replace)
                {
                    if (search == replace)
                    {
                        return base;
                    }
                    else
                    {
                        const std::string s = search;
                        std::string header = base;
                        const std::string r = replace;

                        auto pos = header.find(s);

                        while (pos != std::string::npos)
                        {
                            header = header.replace(pos, s.length(), replace);
                            pos = header.find(s);
                        }

                        return header;
                    }
                }

                static std::string file_name(const std::string path)
                {
                    std::experimental::filesystem::path p(path);
                    auto p2 = p.filename();
                    return p2.string();
                }

                static std::tuple< std::string, std::string > cpp_generate_prepare_templates(const std::tuple < std::string , std::string>& files, const std::string& byte_code_header, const::std::string& cpp_type_name )
                {
                    auto&& cpp_file_name        = std::get<0>(files);
                    auto&& cpp_file_name_header = std::get<1>(files);

                    auto&& header       = replace_string(cpp_file_name_header, "my_shader_type", cpp_type_name);
                    auto&& cpp_file     = replace_string(cpp_file_name, "my_shader_type",        cpp_type_name);
                    auto&& cpp_file2    = replace_string(cpp_file, "shader.hlsl.dxbc.h", file_name( byte_code_header ));

                    return std::make_tuple(cpp_file2, header);
                }

                static void cpp_generate_write_files( const std::tuple < std::string, std::string>& files, const std::string& cpp_file_name, const::std::string& cpp_file_name_header  )
                {
                    auto&& contents_cpp = std::get<0>(files);
                    auto&& contents_h   = std::get<1>(files);
                    
                    std::ofstream f_cpp(cpp_file_name, std::ios_base::binary);
                    if ( f_cpp.good() )
                    {
                        f_cpp << contents_cpp;
                    }
                    else
                    {
                        raise_error("cannot open cpp generated template file");
                    }

                    std::ofstream f_h(cpp_file_name_header, std::ios_base::binary);
                    if (f_h.good())
                    {
                        f_h << contents_h;
                    }
                    else
                    {
                        raise_error("cannot open cpp generated header template file");
                    }
                }

                static std::vector< uc::build::tasks::item_meta_data > build_meta_data(array<ITaskItem^>^ items)
                {
                    std::vector< item_meta_data > result;

                    auto size = items->Length;

                    result.reserve(size);

                    for (auto i = 0; i < size; ++i)
                    {
                        item_meta_data item;

                        auto it = items[i];

                        item.m_file_name                      = marshal( System::IO::Path::GetFullPath(it->ItemSpec));
                        item.m_dxbc                           = optional(marshal(it->GetMetadata("DXBC")), append_extension(item.m_file_name, "dxbc"));
                        item.m_pdb_file                       = optional(marshal(it->GetMetadata("PDBFile")), append_extension(item.m_dxbc, "pdb"));
                        item.m_dxbc_header                    = optional(marshal(it->GetMetadata("DXBCHeader")), append_extension(item.m_dxbc, ".h"));
                        item.m_dxbc_variable_name             = optional(marshal(it->GetMetadata("DXBCVariableName")), "main");
                        
                        item.m_additional_include_directories = split(marshal(it->GetMetadata("AdditionalIncludeDirectories")));
                        item.m_preprocessor_directives        = split(marshal(it->GetMetadata("PreprocessorDefinitions")));
                        item.m_pipeline_stage                 = to_pipeline_stage(marshal(it->GetMetadata("ShaderPipelineStage")));

                        item.m_cpp_generated_type_name        = optional(marshal(it->GetMetadata("CPPGeneratedTypeName")), "myshader_type");
                        item.m_cpp_generated_file_name        = optional(marshal(it->GetMetadata("CPPGeneratedFileName")), item.m_cpp_generated_type_name + ".cpp");
                        item.m_cpp_generated_file_name_header = optional(marshal(it->GetMetadata("CPPGeneratedFileNameHeader")), item.m_cpp_generated_type_name + ".h");

                        result.emplace_back(std::move(item));
                    }

                    return result;
                }
            }

            HLSLCompiler::HLSLCompiler()
            {
                m_output_cpp_files          = gcnew List<ITaskItem^>();
                m_output_cpp_files_headers  = gcnew List<ITaskItem^>();
                m_dxbc                      = gcnew List<ITaskItem^>();
                m_dxbc_headers              = gcnew List<ITaskItem^>();
                m_pdb_files                 = gcnew List<ITaskItem^>();

                m_sources                   = gcnew array<ITaskItem^>(0);
                m_files_written             = gcnew List<ITaskItem^>();
                m_tool_path                 = System::String::Empty;
                m_tracker_log_directory     = System::String::Empty;

                m_preprocessor_definitions  = System::String::Empty;
                m_additional_include_directories = System::String::Empty;
            }

            std::vector<char> code(const std::string& file_name)
            {
                std::ifstream instream(file_name, std::ios_base::binary);

                if (instream.good())
                {
                    std::vector<char> v( std::istreambuf_iterator<char>(instream.rdbuf()), std::istreambuf_iterator<char>());
                    return v;
                }

                std::vector<char> v;
                return v;
            }

            static inline array<System::String^>^ marshal_as( std::vector< std::string >&& s )
            {
                array<System::String^>^ r = gcnew array<System::String^>( s.size() );
                auto j = 0;

                for (auto&& i : s)
                {
                    r[j++] = marshal( i );
                }

                return r;
            }

            static System::Tuple< CanonicalTrackedOutputFiles^, CanonicalTrackedOutputFiles^>^ buildTrackingRecords(const std::vector<uc::build::tasks::item_meta_data>& items, const file_cache& cache, System::String^ tracker_log_directory)
            {
                array<ITaskItem^>^ write_tlogs  = gcnew array<ITaskItem^>(1);
                array<ITaskItem^>^ read_tlogs   = gcnew array<ITaskItem^>(1);

                auto write_tlog     = gcnew TaskItem(tracker_log_directory + "hlsl_compiler.write.1.tlog");
                auto read_tlog      = gcnew TaskItem(tracker_log_directory + "hlsl_compiler.read.1.tlog");

                write_tlogs[0]      = write_tlog;
                read_tlogs[0]       = read_tlog;
                                
                auto tracked_output = gcnew CanonicalTrackedOutputFiles(nullptr, read_tlogs, true);

                for ( auto&& i : items )
                {
                    auto&& cache_item  = cache.m_cache_data.find(i.m_file_name);
                    
                    auto file_name2 = marshal(i.m_file_name);

                    tracked_output->RemoveEntriesForSource(gcnew TaskItem(file_name2));

                    if (cache_item != cache.m_cache_data.end())
                    {
                        auto&& dependencies = cache_item->second;

                        for (auto&& j : dependencies.m_files)
                        {
                            if (j.m_key != i.m_file_name && j.m_type == dependency_type::file)
                            {
                                tracked_output->AddComputedOutputForSourceRoot(file_name2, marshal(j.m_key) );
                            }
                        }
                    }
                }

                auto tracked_output2 = gcnew CanonicalTrackedOutputFiles(nullptr, write_tlogs, true);

                //track outputs of the metadata;
                for (auto&& i : items)
                {
                    auto file_name2 = marshal(i.m_file_name);
                    tracked_output2->RemoveEntriesForSource(gcnew TaskItem(file_name2));

                    std::vector< std::string> strings = { i.m_cpp_generated_file_name, i.m_cpp_generated_file_name_header, i.m_dxbc, i.m_dxbc_header, i.m_pdb_file };

                    for (auto&& s : strings)
                    {
                        tracked_output2->AddComputedOutputForSourceRoot(marshal(i.m_file_name), marshal(s) );
                    }
                }

                return gcnew System::Tuple <CanonicalTrackedOutputFiles^, CanonicalTrackedOutputFiles^  > (tracked_output, tracked_output2);
            }

            bool HLSLCompiler::Execute()
            {
                try
                {
                    auto&& meta_data = build_meta_data(m_sources);
                    std::string   file_cache = m_state_file != nullptr ? marshal(m_state_file->ItemSpec) : "";

                    //change this if you want to rebuild the shaders when deploying new version
                    std::vector< uc::build::tasks::dependency_cache_item > static_dependencies;
                    static_dependencies.emplace_back(uc::build::tasks::dependency_cache_item ( "uc_build_tasks", "27e5437d42117ba9a9162d0a07754bb4"  )) ;

                    //load the cache and check which files need rebuilding
                    auto&& result = uc::build::tasks::build_caches(meta_data, file_cache, static_dependencies);
                    auto&& files_to_build = std::get<0>(result);
                    auto&& cache_to_save = std::get<1>(result);

                    using namespace System::IO;
                    using namespace System::Reflection;

                    auto trackRecords = buildTrackingRecords(meta_data, cache_to_save, m_tracker_log_directory);

                    if (!files_to_build.empty())
                    {
                        for (auto i = 0U; i < files_to_build.size(); ++i)   //todo: this in parallel
                        {
                            auto&& it = files_to_build[i];

                            auto r0 = build_command_line(it.m_file_name, it.m_dxbc, it.m_pdb_file, it.m_dxbc_header, it.m_pipeline_stage, it.m_additional_include_directories, it.m_preprocessor_directives);
                            auto spawner = gcnew FXCSpawner(m_tool_path);
                            auto compile_result = spawner->RunCompiler( r0);

                            if (compile_result->Item1 != 0)
                            {
                                auto error = compile_result->Item3;
                                for ( auto i = 0; i < error->Count; ++i)
                                {
                                    Log->LogMessageFromText( error[i], Microsoft::Build::Framework::MessageImportance::High );
                                }

                                ProcessOutputs(meta_data);
                                return false;
                            }
                            else
                            {
                                auto message = compile_result->Item2;

                                for (auto i = 0; i < message->Count; ++i)
                                {
                                    Log->LogMessage(message[i]);
                                }
                            }

                            auto&& generated_files = cpp_generate_get_templates( it.m_pipeline_stage, m_backend );
                            auto&& files           = cpp_generate_prepare_templates(generated_files, it.m_dxbc_header, it.m_cpp_generated_type_name);

                            cpp_generate_write_files(files, it.m_cpp_generated_file_name, it.m_cpp_generated_file_name_header);
                        }

                        save_cache(file_cache, cache_to_save);
                        trackRecords->Item1->SaveTlog();
                        trackRecords->Item2->SaveTlog();
                    }

                    ProcessOutputs(meta_data);
                    //record all files that are supposed to be written for the clean target
                    RecordFilesWritten(meta_data);
                    return true;
                }
                catch (const boost::wave::preprocess_exception & e)
                {
                    auto sub_category = gcnew System::String("preprocessor");
                    auto error_code   = gcnew System::String( "error" );
                    auto file         = gcnew System::String( e.file_name() );
                    auto help         = System::String::Empty;
                    
                    auto line_number = e.line_no();
                    auto column_number = e.column_no();
                    auto end_line_number = line_number;
                    auto end_column_number = column_number;;
                    auto message = gcnew System::String(e.description());

                    Log->LogError(sub_category, error_code, help, file, line_number, column_number, end_line_number, end_column_number, message);
                    return false;
                }
                catch (const std::exception& e)
                {
                    Log->LogError("ucdev_shader_compiler : {0}", marshal(std::string(e.what())));
                    return false;
                }
                catch ( System::Exception^ e )
                {
                    Log->LogErrorFromException(e);
                    return false;
                }
                catch (...)
                {
                    Log->LogError("ucdev_shader_compiler:{0}", "Unexpected exception");
                    return false;
                }
            }

            void HLSLCompiler::ProcessOutputs(const std::vector<uc::build::tasks::item_meta_data>& items)
            {
                auto s = items.size();

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];

                    this->m_output_cpp_files->Add(gcnew TaskItem(marshal( it.m_cpp_generated_file_name )));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    this->m_output_cpp_files_headers->Add(gcnew TaskItem(marshal(it.m_cpp_generated_file_name_header)));
                }

                for ( auto i = 0U; i < s; ++i )
                {
                    auto&& it = items[i];
                    this->m_dxbc->Add(gcnew TaskItem(marshal(it.m_dxbc)));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    this->m_dxbc_headers->Add(gcnew TaskItem(marshal(it.m_dxbc_header)));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    this->m_pdb_files->Add(gcnew TaskItem(marshal(it.m_pdb_file)));
                }
            }

            void HLSLCompiler::RecordFilesWritten(const std::vector<uc::build::tasks::item_meta_data>& items)
            {
                m_files_written->Clear();

                if (m_state_file != nullptr && m_state_file->ItemSpec->Length > 0)
                {
                    m_files_written->Add(m_state_file);
                }

                auto s = items.size();

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    m_files_written->Add(gcnew TaskItem(marshal(it.m_cpp_generated_file_name)));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    m_files_written->Add(gcnew TaskItem(marshal(it.m_cpp_generated_file_name_header)));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    m_files_written->Add(gcnew TaskItem(marshal(it.m_dxbc)));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    m_files_written->Add(gcnew TaskItem(marshal(it.m_dxbc_header)));
                }

                for (auto i = 0U; i < s; ++i)
                {
                    auto&& it = items[i];
                    m_files_written->Add(gcnew TaskItem(marshal(it.m_pdb_file)));
                }
            }
        }
    }
}

