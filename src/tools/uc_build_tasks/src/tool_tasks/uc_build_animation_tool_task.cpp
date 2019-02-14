// This is the main DLL file.

#include "uc_build_animation_tool_task.h"

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            Animation::Animation()
            {
                m_switchOrderList->Add("Source");
                m_switchOrderList->Add("OutputAnimation");
                m_switchOrderList->Add("OptionsSwapYZ");
                m_switchOrderList->Add("OptionMakeLeftHanded");
            }

            System::String^ Animation::GenerateCommandLineCommands()
            {
                CommandLineBuilder^ builder = gcnew CommandLineBuilder();

                if (Source != nullptr)
                {
                    builder->AppendTextUnquoted(gcnew System::String(" --input_animation "));
                    builder->AppendFileNameIfNotNull(Source);
                }

                auto r = base::GenerateResponseFileCommands() + " " + builder->ToString();
                return r;
            }
        }
    }
}

