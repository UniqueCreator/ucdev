// This is the main DLL file.

#include "uc_build_texture_tool_task.h"

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            Texture::Texture()
            {
                m_switchOrderList->Add("Source");
                m_switchOrderList->Add("OutputTexture");
                m_switchOrderList->Add("OutputTextureFormat");
            }

            System::String^ Texture::GenerateCommandLineCommands()
            {
                CommandLineBuilder^ builder = gcnew CommandLineBuilder();

                if (Source != nullptr)
                {
                    builder->AppendTextUnquoted(gcnew System::String(" --input_texture "));
                    builder->AppendFileNameIfNotNull(Source);
                }

                auto r = base::GenerateResponseFileCommands() + " " + builder->ToString();
                return r;
            }
        }
    }
}

