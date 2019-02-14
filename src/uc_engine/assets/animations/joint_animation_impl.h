#pragma once

#include <uc_public/assets/animations/joint_animation.h>
#include <uc_dev/gx/lip/animation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class JointAnimation::Impl
        {
        public:

            Impl() {}

            Impl(uc::lip::unique_lip_pointer<uc::lip::joint_animation> && animation) : m_animation(std::move(animation))
            {

            }

            uc::lip::unique_lip_pointer<uc::lip::joint_animation> m_animation;
        };
    }
}


