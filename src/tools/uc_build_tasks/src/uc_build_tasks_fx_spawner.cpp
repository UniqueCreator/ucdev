// This is the main DLL file.

#include "uc_build_tasks_fx_spawner.h"
#include "uc_build_tasks_string_helpers.h"
#include "uc_build_tasks_string_helpers_managed.h"


namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
                using namespace System;
                using namespace System::Diagnostics;
                using namespace uc::build::tasks;

                namespace
                {
                    static System::String^ build_tool_executable(System::String^ toolPath)
                    {
                        auto strings = toolPath->Split(';');
                        size_t count = strings->Length;

                        for (auto i = 0U; i < count; ++i)
                        {
                            auto dir = strings[i];
                            if (dir != System::String::Empty)
                            {
                                auto last = dir->Length;
                                System::Char slash = '/';
                                System::Char back_slash = '\\';

                                if (dir[last - 1] != slash  || dir[last - 1] != back_slash )
                                {
                                    dir = dir + back_slash;
                                }

                                auto full_name = System::IO::Path::GetFullPath(dir + "fxc.exe");

                                if (System::IO::File::Exists(full_name))
                                {
                                    return full_name;
                                }
                            }
                        }

                        throw gcnew System::IO::FileNotFoundException("fxc not found");
                    }
                }

                FXCSpawner::FXCSpawner( System::String^ toolPath )
                {
                    using namespace System::Collections::Generic;
                    m_std_output = gcnew List<System::String^>();
                    m_std_error  = gcnew List<System::String^>();
                    m_tool_executable = build_tool_executable(toolPath);
                }

                System::Tuple < Int32, FXCSpawner::CompilerOutput, FXCSpawner::CompilerOutput >^  FXCSpawner::RunCompiler(System::String^ arguments )
                {
                    Process^ process = gcnew Process();

                    ProcessStartInfo^ startInfo = gcnew ProcessStartInfo(m_tool_executable);

                    startInfo->UseShellExecute = false;
                    startInfo->Arguments = arguments;

                    process->StartInfo = startInfo;
                    startInfo->RedirectStandardError = true;
                    startInfo->RedirectStandardOutput = true;

                    process->ErrorDataReceived  += gcnew DataReceivedEventHandler(this, &FXCSpawner::OnErrorDataHandler);
                    process->OutputDataReceived += gcnew DataReceivedEventHandler(this, &FXCSpawner::OnOutputDataHandler);

                    process->Start();

                    process->BeginOutputReadLine();
                    process->BeginErrorReadLine();

                    process->WaitForExit();
                   
                    return gcnew System::Tuple<Int32, FXCSpawner::CompilerOutput, FXCSpawner::CompilerOutput >(process->ExitCode, m_std_output, m_std_error);
                }

                void FXCSpawner::OnErrorDataHandler(System::Object^ /*sendingProcess*/, DataReceivedEventArgs^ outLine)
                {
                    // Collect the net view command output.
                    if (!System::String::IsNullOrEmpty(outLine->Data))
                    {
                        m_std_error->Add(outLine->Data);
                    }
                }

                void FXCSpawner::OnOutputDataHandler(System::Object^ /*sendingProcess*/, DataReceivedEventArgs^ outLine)
                {
                    // Collect the net view command output.
                    if (!System::String::IsNullOrEmpty(outLine->Data))
                    {
                        m_std_output->Add(outLine->Data);
                    }
                }
        }
    }
}

