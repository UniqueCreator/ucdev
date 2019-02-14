// UniqueCreator.Build.Tasks.h

#pragma once

#include "uc_build_boolean_option.h"
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

            public ref class Texture : BaseToolTask
            {
                using base = BaseToolTask;

                public:

                Texture();

           
                property System::String^ ToolName 
                {
                    System::String^ get() override
                    {
                        return gcnew System::String("ucdev_texture_r.exe");
                    }
                }

                [Output]
                [Required]
                virtual property System::String^ OutputTexture
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("OutputTexture"))
                        {
                            return base::ActiveToolSwitches["OutputTexture"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("OutputTexture");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "OutputTexture";
                        s->Description = "Output Texture File Name";
                        s->Required = true;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--output_texture";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("OutputTexture", s);
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
                        s->SwitchValue  = "--input_texture";
                        s->Separator    = gcnew System::String(" ");
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        
                        base::ActiveToolSwitches->Add("Source", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                [Required]
                virtual property System::String^ OutputTextureFormat
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("OutputTextureFormat"))
                        {
                            return base::ActiveToolSwitches["OutputTextureFormat"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("OutputTextureFormat");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "OutputTextureFormat";
                        s->Description = "Texture Format ( bc1_unorm, bc1_unorm_srgb, bc2_unorm, bc2_unorm_srgb bc3_unorm, bc3_unorm_srgb, bc4_unorm, bc4_snorm, bc5_unorm, bc5_snorm)";
                        s->Required = false;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--texture_format";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("OutputTextureFormat", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                protected:

                System::String^ GenerateCommandLineCommands() override;


            };
        }
    }
}

