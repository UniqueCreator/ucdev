// This is the main DLL file.

#include "uc_build_skeleton_tool_task.h"

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            Skeleton::Skeleton()
            {
                m_switchOrderList->Add("Source");
                m_switchOrderList->Add("OutputSkeleton");
                m_switchOrderList->Add("OptionsSwapYZ");
                m_switchOrderList->Add("OptionMakeLeftHanded");
            }

            System::String^ Skeleton::GenerateCommandLineCommands()
            {
                CommandLineBuilder^ builder = gcnew CommandLineBuilder();

                if (Source != nullptr)
                {
                    builder->AppendTextUnquoted(gcnew System::String(" --input_skeleton "));
                    builder->AppendFileNameIfNotNull(Source);
                }

                auto r = base::GenerateResponseFileCommands() + " " + builder->ToString();
                return r;
            }
        }
    }
}

