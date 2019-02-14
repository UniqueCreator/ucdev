#include "pch.h"

#include <uc_public/assets/animations/joint_animation.h>
#include <uc_public/uc_pimpl_impl.h>

#include "joint_animation_impl.h"

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<JointAnimation::Impl>;

        JointAnimation::JointAnimation()
        {

        }

        JointAnimation::~JointAnimation() = default;
        JointAnimation& JointAnimation::operator=(JointAnimation&&) = default;

        JointAnimation::Impl*   JointAnimation::GetImpl()
        {
            return m_impl.get();
        }

        const JointAnimation::Impl*   JointAnimation::GetImpl() const
        {
            return m_impl.get();
        }
    }
}
