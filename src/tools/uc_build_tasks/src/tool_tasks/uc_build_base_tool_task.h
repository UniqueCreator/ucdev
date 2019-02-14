// UniqueCreator.Build.Tasks.h

#pragma once


namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            using namespace Microsoft::Build::Framework;
            using namespace Microsoft::Build::Utilities;
            using namespace Microsoft::Build::Tasks;
            using namespace Microsoft::Build::CPPTasks;
            using namespace System::Collections::Generic;

            namespace details
            {
                inline System::Resources::ResourceManager^ CreateResourceManager()
                {
                    auto resources = System::Reflection::Assembly::Load("Microsoft.Build.CPPTasks.Common");
                    return gcnew System::Resources::ResourceManager("Microsoft.Build.CPPTasks.Strings", resources);
                }
            }

            public ref class BaseToolTask abstract : TrackedVCToolTask
            {
                using base = TrackedVCToolTask;

                public:

                BaseToolTask::BaseToolTask() : base(details::CreateResourceManager())
                {
                    m_switchOrderList = gcnew System::Collections::ArrayList();
                    MinimalRebuildFromTracking = true;
                    TrackFileAccess = true;
                    ToolArchitecture = gcnew System::String("Native64Bit");
                }


                System::String^ GenerateFullPathToTool() override
                {
                    auto path = ToolPath;
                    if (System::String::IsNullOrEmpty(path))
                    {
                        return base::GenerateFullPathToTool();
                    }
                    else
                    {
                        auto p = VCToolTask::EnsureTrailingSlash(path);
                        return System::IO::Path::GetFullPath(p + ToolName);
                    }
                }

                /*
                property System::String^ ToolName 
                {
                    System::String^ get() override
                    {
                        //todo: toolname
                        return gcnew System::String("ucdev_model_r.exe");
                    }
                }
                */

                property array<System::String ^>^ ReadTLogNames
                {
                    array<System::String ^>^ get() override
                    {
                        array<System::String ^>^ r = gcnew array<System::String^>(1);

                        auto fileName = System::IO::Path::GetFileNameWithoutExtension( ToolExe );
                        r[0] = fileName + gcnew System::String(".read.1.tlog");
                        return r;
                    }
                }

                property array<System::String ^>^ WriteTLogNames
                {
                    array<System::String ^>^ get() override
                    {
                        array<System::String ^>^ r = gcnew array<System::String^>(1);

                        auto fileName = System::IO::Path::GetFileNameWithoutExtension(ToolExe);

                        r[0] = fileName + gcnew System::String(".write.1.tlog");
                        return r;
                    }
                }

                property System::String^ CommandTLogName
                {
                    System::String ^ get() override
                    {
                        auto fileName = System::IO::Path::GetFileNameWithoutExtension(ToolExe);
                        return fileName + gcnew System::String(".command.1.tlog");
                    }
                }

                property System::String^ TrackerIntermediateDirectory
                {
                    System::String ^ get() override
                    {
                        auto log = TrackerLogDirectory;
                        if ( log != nullptr)
                        {
                            return log;
                        }
                        else
                        {
                            return System::String::Empty;
                        }
                    }
                }

                virtual property System::String^ TrackerLogDirectory
                {
                    System::String^ get() 
                    {
                        if (base::IsPropertySet("TrackerLogDirectory"))
                        {
                            return base::ActiveToolSwitches["TrackerLogDirectory"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("TrackerLogDirectory");

                        auto s = gcnew ToolSwitch(ToolSwitchType::Directory);

                        s->DisplayName = "Tracker Log Directory";
                        s->Description = "Tracker Log Directory";
                        
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = VCToolTask::EnsureTrailingSlash(value);
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("TrackerLogDirectory", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }


                [Required]
                property ITaskItem^ Source
                {
                    virtual ITaskItem^ get() = 0;
                    virtual void set(ITaskItem^ value) = 0;
                }

                virtual property System::String^ ToolPath
                {
                    System::String^ get()
                    {
                        return m_toolPath;
                    }

                    void set(System::String^ value)
                    {
                        m_toolPath = value;
                    }
                }

                property array<ITaskItem^>^ TrackedInputFiles
                {
                    array<ITaskItem^>^ get() override
                    {
                        auto r = gcnew array<ITaskItem^>(1);
                        r[0] = Source;
                        return r;
                    }
                }

                property System::Collections::ArrayList^ SwitchOrderList
                {
                    System::Collections::ArrayList^ get() override
                    {
                        return m_switchOrderList;
                    }
                }

                protected:

                System::Collections::ArrayList^                  m_switchOrderList;
                System::String^                                  m_toolPath;
            };
        }
    }
}

