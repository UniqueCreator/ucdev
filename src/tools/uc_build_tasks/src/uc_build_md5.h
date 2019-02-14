// UniqueCreator.Build.Tasks.h

#pragma once

#include <string>

namespace uc
{
    namespace build
    {
        namespace tasks
        {

            std::string md5(const std::string& s);
            std::string md5(const char* s);
        }
    }
}
