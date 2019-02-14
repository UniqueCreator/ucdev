#pragma once

#include <string>

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            enum shader_pipeline_stage : uint32_t
            {
                pixel       = 0,
                vertex      = 1,
                geometry    = 2,
                hull        = 3,
                domain      = 4,
                compute     = 5,
                root_signature = 6
            };

        }
    }
}