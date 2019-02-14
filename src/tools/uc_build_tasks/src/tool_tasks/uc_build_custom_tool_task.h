// UniqueCreator.Build.Tasks.h

#pragma once

#include "../uc_build_backends.h"

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

            public ref class PipelineStateObject : TrackedVCToolTask
            {
                using base = TrackedVCToolTask;

                public:

                PipelineStateObject();

           
                property System::String^ ToolName 
                {
                    System::String^ get() override
                    {
                        if (m_backend == Backend::UniqueCreatorDev)
                        {
                            return gcnew System::String("ucdev_pso_r.exe");
                        }
                        else
                        {
                            return gcnew System::String("ucdev_pso2_r.exe");
                        }
                    }
                }

                System::String^ GenerateFullPathToTool() override;

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

                [Output]
                virtual property System::String^ CPPHeaderOutput
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("CPPHeaderOutput"))
                        {
                            return base::ActiveToolSwitches["CPPHeaderOutput"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("CPPHeaderOutput");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "CPPHeaderOutput";
                        s->Description = "CPP Header Output File Name";

                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--header";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("CPPHeaderOutput", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                [Output]
                virtual property System::String^ CPPOutput
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("CPPOutput"))
                        {
                            return base::ActiveToolSwitches["CPPOutput"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("CPPOutput");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "CPPOutput";
                        s->Description = "CPP Output File Name";

                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--cpp";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("CPPOutput", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                [Required]
                virtual property System::String^ EntryPointName
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("EntryPointName"))
                        {
                            return base::ActiveToolSwitches["EntryPointName"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("EntryPointName");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "EntryPointName";
                        s->Description = "Entry Point name ( default is main )";

                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->Required = true;
                        s->SwitchValue = "--main";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("EntryPointName", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }
                    
                [Required]
                virtual property ITaskItem^ Source
                {
                    ITaskItem^ get() 
                    {
                        if (base::IsPropertySet("Source"))
                        {
                            return base::ActiveToolSwitches["Source"]->TaskItem;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(ITaskItem^ value)
                    {
                        base::ActiveToolSwitches->Remove("Source");

                        auto s = gcnew ToolSwitch(ToolSwitchType::ITaskItem);

                        s->DisplayName  = "Source";
                        s->TaskItem     = value;
                        s->Description  = "Source file";
                        s->Required     = true;
                        s->SwitchValue  = "--file";
                        s->Separator    = gcnew System::String(" ");
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        
                        base::ActiveToolSwitches->Add("Source", s);
                        base::AddActiveSwitchToolValue(s);
                    }
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

                protected:

                System::String^ GenerateCommandLineCommands() override;

                private:

                System::Collections::ArrayList^ m_switchOrderList;
                System::String^                 m_toolPath;
                Backend             m_backend = Backend::UniqueCreatorPublic;
                System::String^     m_backend_string = gcnew System::String("UniqueCreatorPublic");
            };

            public ref class GraphicsPipelineStateObject : public PipelineStateObject
            {
                using base = PipelineStateObject;

                protected:
                System::String^ GenerateCommandLineCommands() override;

            };

            public ref class ComputePipelineStateObject : public PipelineStateObject
            {
                using base = PipelineStateObject;

                protected:
                System::String^ GenerateCommandLineCommands() override;
            };


        }
    }
}

