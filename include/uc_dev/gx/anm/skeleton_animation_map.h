#pragma once

#include <vector>

#include <uc_dev/gx/lip/animation.h>

namespace uc
{
    namespace gx
    {
        namespace anm
        {
            struct skeleton_animation_map
            {
                //todo: std move
                std::vector<uint16_t> m_data; //indices of joints in a skeleton
            };

            skeleton_animation_map make_skeleton_animation_map(const lip::skeleton* s, const lip::joint_animations* a);
        }
    }
}
