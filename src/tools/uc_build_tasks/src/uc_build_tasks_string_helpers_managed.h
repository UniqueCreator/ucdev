// UniqueCreator.Build.Tasks.h

#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            static inline std::string marshal(System::String^ managed_string)
            {
                return msclr::interop::marshal_as<std::string>(managed_string);
            }

            static inline System::String^ marshal(const std::string& s)
            {
                return msclr::interop::marshal_as<System::String^>(s);
            }
        }
    }
}

