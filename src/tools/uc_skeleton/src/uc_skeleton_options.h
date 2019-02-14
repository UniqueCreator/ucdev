#pragma once

#include <cstdint>
#include <unordered_set>

#include <uc_dev/gx/lip/animation.h>
#include <uc_dev/gx/import/geo/skinned_mesh.h>

namespace uc
{
    namespace skeleton
    {
        enum options : uint32_t
        {
            none = 0
        };

        using options_flags = uint32_t;
    }
}

