#include "pch.h"

#include <uc_public/assets/animations/joint_animations.h>
#include <uc_public/uc_pimpl_impl.h>

#include "joint_animations_impl.h"

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<JointAnimations::Impl>;

        JointAnimations::JointAnimations()
        {

        }

        JointAnimations::~JointAnimations() = default;
        JointAnimations& JointAnimations::operator=(JointAnimations&&) = default;

        JointAnimations::Impl*   JointAnimations::GetImpl()
        {
            return m_impl.get();
        }

        const JointAnimations::Impl*   JointAnimations::GetImpl() const
        {
            return m_impl.get();
        }
    }
}
