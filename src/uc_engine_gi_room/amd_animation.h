#pragma once

#include <vector>

#include "amd_base_types.h"

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        struct AnimationKey
        {
            Int32       m_start_time;
            Int32       m_end_time;
            Float4      m_value;
        };

        struct AnimationKeys
        {
            std::vector< AnimationKey > m_keys;
        };

        class Animation
        {

            public:

            Float4x4        m_transform_matrix      = Identity();

            AnimationKeys   m_position_keys;
            AnimationKeys   m_rotation_keys;
            AnimationKeys   m_scale_keys;

            Float3          m_current_key           = { 0.0f,0.0f,0.0f };
            Float3          m_current_key_lerp      = { 0.0f,0.0f,0.0f };

            Float           m_current_time          = 0.0f;

            Float           m_range_start           = 0.0f;
            Float           m_range_end             = 0.0f;



            

        };

    }
}

