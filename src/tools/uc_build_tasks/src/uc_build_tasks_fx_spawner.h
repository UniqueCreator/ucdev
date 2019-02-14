// UniqueCreator.Build.Tasks.h

#pragma once

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            ref class FXCSpawner
            {

            public:

                using CompilerOutput = System::Collections::Generic::List<System::String^>^ ;

                FXCSpawner( System::String^ toolPath );

                System::Tuple < System::Int32, CompilerOutput, CompilerOutput >^  RunCompiler(System::String^ arguments);

            private:

                void OnErrorDataHandler(System::Object^ /*sendingProcess*/, System::Diagnostics::DataReceivedEventArgs^ outLine);
                void OnOutputDataHandler(System::Object^ /*sendingProcess*/, System::Diagnostics::DataReceivedEventArgs^ outLine);

                CompilerOutput      m_std_output;
                CompilerOutput      m_std_error;
                System::String^     m_tool_executable;
            };
        }
    }
}

