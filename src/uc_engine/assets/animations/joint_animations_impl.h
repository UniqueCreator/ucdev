#pragma once

#include <uc_public/assets/animations/joint_animations.h>
#include <uc_dev/gx/lip/animation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class JointAnimations::Impl
        {
        public:

            Impl() {}

            Impl(uc::lip::unique_lip_pointer<uc::lip::joint_animations> && animations) : m_animations(std::move(animations))
            {

            }

            uc::lip::unique_lip_pointer<uc::lip::joint_animations> m_animations;
        };
    }
}


