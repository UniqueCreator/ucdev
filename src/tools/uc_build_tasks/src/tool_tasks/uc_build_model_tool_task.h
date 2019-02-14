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

            enum BooleanOptionValues
            {
                OptionCalcTangentSpace = 0,
                OptionJoinIdenticalVertices,
                OptionMakeLeftHanded,
                OptionTriangulate,
                OptionRemoveComponent,
                OptionGenNormals,
                OptionGenSmoothNormals,
                OptionSplitLargeMeshes,

                OptionPreTransformVertices,
                OptionLimitBoneWeights,
                OptionValidateDataStructure,
                OptionImproveCacheLocality,

                OptionRemoveRedundantMaterials,
                OptionFixInfacingNormals,
                OptionSortByPType,
                OptionFindDegenerates,

                OptionFindInvalidData,
                OptionGenUVCoords,
                OptionTransformUVCoords,
                OptionFindInstances,

                OptionOptimizeMeshes,
                OptionOptimizeGraph,
                OptionFlipUVs,
                OptionFlipWindingOrder,

                OptionSplitByBoneCount,
                OptionDebone
            };


            public ref class Model : BaseToolTask
            {
                using base = BaseToolTask;

                public:

                Model();

           
                property System::String^ ToolName 
                {
                    System::String^ get() override
                    {
                        return gcnew System::String("ucdev_model_r.exe");
                    }
                }

                [Output]
                [Required]
                virtual property System::String^ OutputModel
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("OutputModel"))
                        {
                            return base::ActiveToolSwitches["OutputModel"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("OutputModel");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "OutputModel";
                        s->Description = "Output Model File Name";
                        s->Required = true;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--output_model";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("OutputModel", s);
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
                        s->SwitchValue  = "--input_model";
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

                virtual property System::String^ ModelType
                {
                    System::String^ get()
                    {
                        if (base::IsPropertySet("ModelType"))
                        {
                            return base::ActiveToolSwitches["ModelType"]->Value;
                        }
                        else
                        {
                            return nullptr;
                        }
                    }

                    void set(System::String^ value)
                    {
                        base::ActiveToolSwitches->Remove("ModelType");

                        auto s = gcnew ToolSwitch(ToolSwitchType::String);

                        s->DisplayName = "ModelType";
                        s->Description = "Model Type ( default, parametrized, textured, multi_textured, skinned)";
                        s->Required = false;
                        s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                        s->Value = value;
                        s->SwitchValue = "--type";
                        s->Separator = gcnew System::String(" ");

                        base::ActiveToolSwitches->Add("ModelType", s);
                        base::AddActiveSwitchToolValue(s);
                    }
                }

                property array<ITaskItem^>^ Textures
                {
                    array<ITaskItem^>^ get()
                    {
                        return m_textures->ToArray();
                    }

                    void set(array<ITaskItem^>^ value)
                    {
                        m_textures->Clear();
                        m_textures->AddRange(value);
                    }
                }

                System::Boolean booleanPropertyRead(System::Int32 index)
                {
                    auto name = ((BooleanOption^)m_boolean_options[index])->m_name;

                    if (base::IsPropertySet(name))
                    {
                        return System::Boolean::Parse(base::ActiveToolSwitches[name]->Value);
                    }
                    else
                    {
                        return false;
                    }
                }

                void booleanPropertyWrite( System::Boolean value, System::Int32 index )
                {
                    auto option = (BooleanOption^)m_boolean_options[index];

                    base::ActiveToolSwitches->Remove(option->m_name);

                    auto s = gcnew ToolSwitch(ToolSwitchType::String);

                    s->DisplayName = option->m_name;
                    s->Description = option->m_description;
                    s->Required = false;
                    s->ArgumentRelationList = gcnew System::Collections::ArrayList();
                    s->Value = value ? "true" : "false";
                    s->SwitchValue = option->m_command_line_option;
                    s->Separator = gcnew System::String(" ");

                    base::ActiveToolSwitches->Add(option->m_name, s);
                    base::AddActiveSwitchToolValue(s);

                }

                virtual property System::Boolean OptionCalcTangentSpace
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionCalcTangentSpace);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionCalcTangentSpace);
                    }
                }

                virtual property System::Boolean OptionJoinIdenticalVertices
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionJoinIdenticalVertices);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionJoinIdenticalVertices);
                    }
                }

                virtual property System::Boolean OptionMakeLeftHanded
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionMakeLeftHanded);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionMakeLeftHanded);
                    }
                }

                virtual property System::Boolean OptionTriangulate
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionTriangulate);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionTriangulate);
                    }
                }

                virtual property System::Boolean OptionRemoveComponent
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionRemoveComponent);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionRemoveComponent);
                    }
                }

                virtual property System::Boolean OptionGenNormals
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionGenNormals);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionGenNormals);
                    }
                }

                virtual property System::Boolean OptionGenSmoothNormals
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionGenSmoothNormals);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionGenSmoothNormals);
                    }
                }

                virtual property System::Boolean OptionSplitLargeMeshes
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionSplitLargeMeshes);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionSplitLargeMeshes);
                    }
                }

                virtual property System::Boolean OptionPreTransformVertices
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionPreTransformVertices);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionPreTransformVertices);
                    }
                }

                virtual property System::Boolean OptionLimitBoneWeights
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionLimitBoneWeights);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionLimitBoneWeights);
                    }
                }

                virtual property System::Boolean OptionValidateDataStructure
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionValidateDataStructure);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionValidateDataStructure);
                    }
                }

                virtual property System::Boolean OptionImproveCacheLocality
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionImproveCacheLocality);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionImproveCacheLocality);
                    }
                }

                virtual property System::Boolean OptionRemoveRedundantMaterials
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionRemoveRedundantMaterials);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionRemoveRedundantMaterials);
                    }
                }

                virtual property System::Boolean OptionFixInfacingNormals
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionFixInfacingNormals);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionFixInfacingNormals);
                    }
                }

                virtual property System::Boolean OptionSortByPType
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionSortByPType);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionSortByPType);
                    }
                }

                virtual property System::Boolean OptionFindDegenerates
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionFindDegenerates);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionFindDegenerates);
                    }
                }

                virtual property System::Boolean OptionFindInvalidData
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionFindInvalidData);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionFindInvalidData);
                    }
                }

                virtual property System::Boolean OptionGenUVCoords
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionGenUVCoords);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionGenUVCoords);
                    }
                }

                virtual property System::Boolean OptionTransformUVCoords
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionTransformUVCoords);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionTransformUVCoords);
                    }
                }

                virtual property System::Boolean OptionFindInstances
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionFindInstances);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionFindInstances);
                    }
                }

                virtual property System::Boolean OptionOptimizeMeshes
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionOptimizeMeshes);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionOptimizeMeshes);
                    }
                }

                virtual property System::Boolean OptionOptimizeGraph
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionOptimizeGraph);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionOptimizeGraph);
                    }
                }

                virtual property System::Boolean OptionFlipUVs
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionFlipUVs);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionFlipUVs);
                    }
                }

                virtual property System::Boolean OptionFlipWindingOrder
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionFlipWindingOrder);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionFlipWindingOrder);
                    }
                }

                virtual property System::Boolean OptionSplitByBoneCount
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionSplitByBoneCount);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionSplitByBoneCount);
                    }
                }

                virtual property System::Boolean OptionDebone
                {
                    System::Boolean get()
                    {
                        return booleanPropertyRead(BooleanOptionValues::OptionDebone);
                    }

                    void set(System::Boolean value)
                    {
                        booleanPropertyWrite(value, BooleanOptionValues::OptionDebone);
                    }
                }

                protected:

                System::String^ GenerateCommandLineCommands() override;

                private:

                System::Collections::ArrayList^                  m_boolean_options;
                System::Collections::Generic::List<ITaskItem^>^  m_textures;

            };
        }
    }
}

