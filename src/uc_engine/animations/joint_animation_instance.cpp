#include "pch.h"

#include <uc_public/animations/joint_animation_instance.h>
#include <uc_public/uc_pimpl_impl.h>

#include "joint_animation_instance_impl.h"
#include "skeleton_instance_impl.h"

namespace UniqueCreator
{
    namespace Animations
    {
        template details::pimpl<JointAnimationInstance::Impl>;

        JointAnimationInstance::JointAnimationInstance()
        {

        }

        JointAnimationInstance::~JointAnimationInstance() = default;
        JointAnimationInstance& JointAnimationInstance::operator=(JointAnimationInstance&&) = default;

        JointAnimationInstance::Impl*   JointAnimationInstance::GetImpl()
        {
            return m_impl.get();
        }

        const JointAnimationInstance::Impl*   JointAnimationInstance::GetImpl() const
        {
            return m_impl.get();
        }

        void JointAnimationInstance::Accumulate(SkeletonInstance* s, double time)
        {
            auto si = s->GetImpl()->m_skeleton_instance.get();
            auto ai = GetImpl()->m_instance.get();
            ai->accumulate(si, time);
        }

        void JointAnimationInstance::Reset()
        {
            auto ai = GetImpl()->m_instance.get();
            ai->reset();
        }
    }
}
