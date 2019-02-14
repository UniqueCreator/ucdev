// UniqueCreator.Build.Tasks.h

#pragma once
#include <vector>
#include "uc_build_tasks_build_caches.h"
#include "uc_build_backends.h"

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            using namespace Microsoft::Build::Framework;
            using namespace Microsoft::Build::Utilities;
            using namespace System::Collections::Generic;

            public ref class HLSLCompiler : public AppDomainIsolatedTask
            {
                public:

                HLSLCompiler();

                [Required]
                property array<ITaskItem^>^ Sources
                {
                    array<ITaskItem^>^ get()
                    {
                        return m_sources;
                    }

                    void set(array<ITaskItem^>^ value)
                    {
                        m_sources = value;
                    }
                }

                property ITaskItem^ StateFile
                {
                    ITaskItem^ get()
                    {
                        return m_state_file;
                    }

                    void set(ITaskItem^ value)
                    {
                        m_state_file = value;
                    }
                }

                [Output]
                property array<ITaskItem^>^ DXBC
                {
                    array<ITaskItem^>^ get()
                    {
                        return this->m_dxbc->ToArray();
                    }
                };

                [Output]
                property array<ITaskItem^>^ DXBCHeaders
                {
                    array<ITaskItem^>^ get()
                    {
                        return this->m_dxbc_headers->ToArray();
                    }
                };

                [Output]
                property array<ITaskItem^>^ PDBFiles
                {
                    array<ITaskItem^>^ get()
                    {
                        return this->m_pdb_files->ToArray();
                    }
                };

                [Output]
                property array<ITaskItem^>^ CPPFilesHeaders
                {
                    array<ITaskItem^>^ get()
                    {
                        return this->m_output_cpp_files_headers->ToArray();
                    }
                };

                [Output]
                property array<ITaskItem^>^ CPPFiles
                {
                    array<ITaskItem^>^ get()
                    {
                        return this->m_output_cpp_files->ToArray();
                    }
                };

                /// <summary>
                /// Storage for names of *all files* written to disk.  This is part of the implementation
                /// for Clean, and contains the OutputResources items and the StateFile item.
                /// Includes any output files that were already up to date, but not any output files
                /// that failed to be written due to an error. 
                /// </summary>
                [Output]
                property array<ITaskItem^>^ FilesWritten
                {
                    array<ITaskItem^>^ get()
                    {
                        return m_files_written->ToArray();
                    }
                }

                property System::String^ ToolPath
                {
                    System::String^ get()
                    {
                        return m_tool_path;
                    }

                    void set(System::String^ value)
                    {
                        m_tool_path = value;
                    }
                }

                property System::String^ TrackerLogDirectory
                {
                    System::String^ get()
                    {
                        return m_tracker_log_directory;
                    }

                    void set(System::String^ value)
                    {
                        m_tracker_log_directory = value;
                    }
                }

                property System::String^ AdditionalIncludeDirectories
                {
                    System::String^ get()
                    {
                        return m_additional_include_directories;
                    }

                    void set(System::String^ value)
                    {
                        m_additional_include_directories = value;
                    }
                }

                property System::String^ PreprocessorDefinitions
                {
                    System::String^ get()
                    {
                        return m_preprocessor_definitions;
                    }

                    void set(System::String^ value)
                    {
                        m_preprocessor_definitions = value;
                    }
                }

                property System::String^ GeneratedCodeBackend
                {
                    System::String^ get()
                    {
                        return m_backend_string;
                    }

                    void set(System::String^ b)
                    {
                        m_backend = ParseStringBackend(b);
                        m_backend_string = b;
                    }
                }

                bool Execute() override;

                private:

                    array<ITaskItem^>^ m_sources;

                    List<ITaskItem^>^  m_output_cpp_files;
                    List<ITaskItem^>^  m_output_cpp_files_headers;
                    List<ITaskItem^>^  m_files_written;
                    List<ITaskItem^>^  m_dxbc;
                    List<ITaskItem^>^  m_dxbc_headers;
                    List<ITaskItem^>^  m_pdb_files;
                    ITaskItem^         m_state_file;
                    System::String^    m_tool_path;
                    System::String^    m_tracker_log_directory;
                    System::String^    m_additional_include_directories;
                    System::String^    m_preprocessor_definitions;
                    Backend            m_backend        = Backend::UniqueCreatorPublic;
                    System::String^    m_backend_string = gcnew System::String("UniqueCreatorPublic");


                    void RecordFilesWritten(const std::vector<uc::build::tasks::item_meta_data>&);  //build lists with all side effects from the build system, used for clean
                    void ProcessOutputs(const std::vector<uc::build::tasks::item_meta_data>& ); //build lists with side effects from the build process for msbuild usage
            };
        }
    }
}

