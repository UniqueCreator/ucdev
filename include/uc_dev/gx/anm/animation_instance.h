#pragma once

#include <vector>
#include <uc_dev/math/math.h>

#include "skeleton_animation_map.h"

namespace uc
{
    namespace lip {
        struct joint_animation;
    }
}

namespace uc {
    namespace gx {
        namespace anm {

            class skeleton_instance;

            math::float4 animate_translation(const lip::joint_animation* a, double time, double animation_duration);
            math::float4 animate_rotation(const lip::joint_animation* a, double time, double animation_duration);
            
            class animation_instance
            {
                public:

                animation_instance(const lip::joint_animations* a, const lip::skeleton* s, double start_time = 0.0);

                void accumulate(skeleton_instance* result, double delta_time);

                void reset();

                private:
                skeleton_animation_map         m_skeleton_map;
                const lip::joint_animations*   m_animations  = nullptr;
                double                         m_time;
                double                         m_start_time;
            };
        }
    }
}
