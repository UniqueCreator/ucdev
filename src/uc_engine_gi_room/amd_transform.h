#pragma once

#include <array>
#include <vector>

#include "amd_animation.h"

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {

        class Transform
        {
            public:

            Int32 m_id              = 0;

            Float4x4                m_transform_matrix = Identity();
            Animation               m_animation;


            Int32                   m_parent = -1;
            std::vector<uint32_t >  m_children;
        };

    }
        
}

