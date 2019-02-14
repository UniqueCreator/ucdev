// UniqueCreator.Build.Tasks.h

#pragma once

#include "uc_build_base_tool_task.h"

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


            public ref class Skeleton : BaseToolTask
            {
                using base = BaseToolTask;

                public:

                Skeleton();

                property System::String^ ToolName 
                {
                    System::String^ get() override
                    {
                        return gcnew System::String("ucdev_skeleton_r.exe");
                    }
                }

                [Output]
                [Required]
                virtual property System::String^ OutputSkeleton
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("OutputSkeleton"))
                        {
                            return base::ActiveToolSwitches["OutputSkeleton"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("OutputSkeleton");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "OutputSkeleton";
                        s->Description = "Output Skeleton File Name";
                        s->Required = true;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--output_skeleton";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("OutputSkeleton", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                [Required]
                virtual property ITaskItem^ Source
                {
                    ITaskItem^ get() override
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

                    void set(ITaskItem^ value) override
                    {
                        base::ActiveToolSwitches->Remove("Source");

                        auto s = gcnew ToolSwitch(ToolSwitchType::ITaskItem);

                        s->DisplayName  = "Source";
                        s->TaskItem     = value;
                        s->Description  = "Source file";
                        s->Required     = true;
                        s->SwitchValue  = "--input_skeleton";
                        s->Separator    = gcnew System::String(" ");
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        
                        base::ActiveToolSwitches->Add("Source", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                virtual property System::Boolean OptionsSwapYZ
                {
                    System::Boolean get()
                    {
                        if (base::IsPropertySet("OptionsSwapYZ"))
                        {
                            return System::Boolean::Parse(base::ActiveToolSwitches["OptionsSwapYZ"]->Value );
                        }
                        else
                        {
                            return false;
                        }
                    }

                    void set(System::Boolean value)
                    {
                        base::ActiveToolSwitches->Remove("OptionsSwapYZ");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "OptionsSwapYZ";
                        s->Description = "Swap Y and Z coordinate of a mesh positions";
                        s->Required = false;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value ? "true" : "false";
                        s->SwitchValue = "--swap_y_z";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("OptionsSwapYZ", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }


                virtual property System::Boolean OptionMakeLeftHanded
                {
                    System::Boolean get()
                    {
                        if (base::IsPropertySet("OptionsSwapYZ"))
                        {
                            return System::Boolean::Parse(base::ActiveToolSwitches["OptionMakeLeftHanded"]->Value);
                        }
                        else
                        {
                            return false;
                        }
                    }

                    void set(System::Boolean value)
                    {
                        base::ActiveToolSwitches->Remove("OptionMakeLeftHanded");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "OptionMakeLeftHanded";
						s->Description = "Negates Z Position";
                        s->Required = false;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value ? "true" : "false";
                        s->SwitchValue = "--make_left_handed";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("OptionMakeLeftHanded", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                protected:

                System::String^ GenerateCommandLineCommands() override;

            };
        }
    }
}

