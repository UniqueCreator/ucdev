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

            private ref class BooleanOption
            {
                public:

                BooleanOption(System::String^ name, System::String^ option, System::String^ description) :
                    m_name(name)
                    , m_command_line_option(option)
                    , m_description(description)
                {

                }


                System::String^ m_name;
                System::String^ m_command_line_option;
                System::String^ m_description;

            };
        }
    }
}

