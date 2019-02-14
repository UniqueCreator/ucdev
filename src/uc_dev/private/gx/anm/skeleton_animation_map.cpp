#include "pch.h"

#include <uc_dev/gx/anm/skeleton_animation_map.h>

namespace uc {
    namespace gx {
        namespace anm {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            skeleton_animation_map make_skeleton_animation_map(const lip::skeleton* s, const lip::joint_animations* a)
            {
                skeleton_animation_map r;

                r.m_data.reserve(s->joint_count());

                for (auto&& anm : a->m_joint_animations)
                {
                    auto joint_index = s->joint_index(anm.m_joint_name);
                    r.m_data.push_back(joint_index);
                }

                return r;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        }
    }
}
