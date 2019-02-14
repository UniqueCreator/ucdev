// UniqueCreator.Build.Tasks.h

#pragma once
#include <exception>

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            inline void raise_error(const char* message)
            {
                throw std::exception(message);
            }
        }
    }
}

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            inline void raise_error(const char* message)
            {
                throw std::exception(message);
            }
        }
    }
}

