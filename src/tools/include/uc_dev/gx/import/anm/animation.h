#pragma once

#include <vector>
#include <string>

#include <uc_dev/math/math.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace anm
            {
                struct joint_rotation_key
                {
                    math::float4 m_transform;
                    double       m_time;        //in ticks
                };

                struct joint_translation_key
                {
                    math::float4 m_transform;
                    double       m_time;        //in ticks
                };

                struct joint_animation
                {
                    std::vector < joint_rotation_key >      m_rotation_keys;
                    std::vector < joint_translation_key >   m_translation_keys;
                    std::string                             m_joint_name;
                };

                struct joint_animations
                {
                    std::vector< joint_animation >          m_joint_animations;
                    double                                  m_duration; //in ticks
                    double                                  m_ticks_per_second;
                    std::string                             m_name;
                };
            }
        }
    }
}
 







