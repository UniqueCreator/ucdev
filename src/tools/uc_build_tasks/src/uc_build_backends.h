// UniqueCreator.Build.Tasks.h

#pragma once

namespace UniqueCreator
{
    namespace Build
    {
        namespace Tasks
        {
            public enum class Backend 
            { 
                UniqueCreatorDev    = 0,
                UniqueCreatorPublic = 1
            };


            inline Backend ParseStringBackend(System::String^s)
            {
                if (s == "UniqueCreatorDev")
                {
                    return Backend::UniqueCreatorDev;
                }
                else
                {
                    return Backend::UniqueCreatorPublic;
                }
            }
        }
    }
}

