// This is the main DLL file.

#include "uc_build_custom_tool_task.h"

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            namespace
            {
                inline System::Resources::ResourceManager^ CreateResourceManager()
                {
                    auto resources = System::Reflection::Assembly::Load("Microsoft.Build.CPPTasks.Common");
                    return gcnew System::Resources::ResourceManager("Microsoft.Build.CPPTasks.Strings", resources );
                }
            }

            PipelineStateObject::PipelineStateObject() : base(CreateResourceManager())
            {
                m_switchOrderList = gcnew System::Collections::ArrayList();

                m_switchOrderList->Add("EntryPointName");
                m_switchOrderList->Add("CPPHeaderOutput");
                m_switchOrderList->Add("CPPOutput");
                m_switchOrderList->Add("Source");

                MinimalRebuildFromTracking = true;
                TrackFileAccess = true;

                ToolArchitecture = gcnew System::String("Native64Bit");
            }

            System::String^ PipelineStateObject::GenerateFullPathToTool()
            {
                auto path = ToolPath;
                if (System::String::IsNullOrEmpty(path))
                {
                    return base::GenerateFullPathToTool();
                }
                else
                {
                    auto p = VCToolTask::EnsureTrailingSlash(path);
                    return p + ToolName;
                }
            }

            System::String^ PipelineStateObject::GenerateCommandLineCommands()
            {
                CommandLineBuilder^ builder = gcnew CommandLineBuilder();

                if (Source != nullptr)
                {
                    builder->AppendTextUnquoted(gcnew System::String("--file "));
                    builder->AppendFileNameIfNotNull(Source);
                }

                auto r = base::GenerateResponseFileCommands() + " " + builder->ToString();
                //auto r = builder->ToString();
                return r;
            }


            System::String^ GraphicsPipelineStateObject::GenerateCommandLineCommands()
            {
                return base::GenerateCommandLineCommands() + " --graphics";
            }

            System::String^ ComputePipelineStateObject::GenerateCommandLineCommands()
            {
                return base::GenerateCommandLineCommands() + " --compute";
            }


        }
    }
}

