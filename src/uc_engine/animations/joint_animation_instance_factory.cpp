#include "pch.h"

#include <uc_public/animations/joint_animation_instance_factory.h>
#include <uc_public/uc_pimpl_impl.h>

#include "joint_animation_instance_factory_impl.h"
#include "joint_animation_instance_impl.h"

#include "../assets/animations/joint_animations_impl.h"
#include "../assets/animations/skeleton_impl.h"


namespace UniqueCreator
{
    namespace Animations
    {
        template details::pimpl<JointAnimationInstanceFactory::Impl>;

        class JointAnimationInstanceInternal : public JointAnimationInstance
        {
        public:

            JointAnimationInstanceInternal(const uc::lip::joint_animations* a, const uc::lip::skeleton* s, double start_time)
            {
                m_impl = std::move(std::make_unique<uc::gx::anm::animation_instance>(a, s, start_time));
            }
        };

        JointAnimationInstanceFactory::JointAnimationInstanceFactory()
        {

        }

        JointAnimationInstanceFactory::~JointAnimationInstanceFactory() = default;
        JointAnimationInstanceFactory& JointAnimationInstanceFactory::operator=(JointAnimationInstanceFactory&&) = default;

        JointAnimationInstanceFactory::Impl*   JointAnimationInstanceFactory::GetImpl()
        {
            return m_impl.get();
        }

        const JointAnimationInstanceFactory::Impl*   JointAnimationInstanceFactory::GetImpl() const
        {
            return m_impl.get();
        }

        UC_ENGINE_API std::unique_ptr<JointAnimationInstance> JointAnimationInstanceFactory::Create(const Assets::JointAnimations* jointAnimations, const Assets::Skeleton* skeleton, double startTime)
        {
            auto a = jointAnimations->GetImpl()->m_animations.get();
            auto s = skeleton->GetImpl()->m_skeleton.get();
            return std::make_unique<JointAnimationInstanceInternal>(a, s, startTime);
        }
    }
}


