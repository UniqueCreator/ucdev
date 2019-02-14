#pragma once

#include <uc_public/animations/joint_animation_instance.h>
#include <uc_dev/gx/anm/animation_instance.h>
#include <uc_dev/gx/lip/animation.h>

namespace UniqueCreator
{
    namespace Animations
    {
        class JointAnimationInstance::Impl
        {
            public:

            Impl() {}

            Impl(const uc::lip::joint_animations* a, const uc::lip::skeleton* s, double start_time ) : m_instance(std::make_unique<uc::gx::anm::animation_instance>(a,s,start_time))
            {

            }

            Impl(std::unique_ptr<uc::gx::anm::animation_instance>&& instance) : m_instance(std::move(instance))
            {

            }

            std::unique_ptr<uc::gx::anm::animation_instance> m_instance;
        };
    }
}


