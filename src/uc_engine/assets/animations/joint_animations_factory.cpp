#include "pch.h"

#include <uc_public/assets/animations/joint_animations_factory.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/assets/animations/joint_animations.h>

#include "joint_animations_factory_impl.h"
#include "joint_animations_impl.h"
#include <uc_dev/gx/lip/file.h>

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<JointAnimationsFactory::Impl>;

        class JointAnimationsInternal : public JointAnimations
        {
        public:

            JointAnimationsInternal(uc::lip::unique_lip_pointer<uc::lip::joint_animations> && animation)
            {
                m_impl = std::move(animation);
            }
        };

        JointAnimationsFactory::JointAnimationsFactory()
        {

        }

        JointAnimationsFactory::~JointAnimationsFactory() = default;
        JointAnimationsFactory& JointAnimationsFactory::operator=(JointAnimationsFactory&&) = default;

        JointAnimationsFactory::Impl*   JointAnimationsFactory::GetImpl()
        {
            return m_impl.get();
        }

        const JointAnimationsFactory::Impl*   JointAnimationsFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<JointAnimations> JointAnimationsFactory::CreateFromFile(const wchar_t* fileName)
        {
            auto animation = uc::lip::create_from_compressed_lip_file<uc::lip::joint_animations>(fileName);
            return std::make_unique<JointAnimationsInternal>(std::move(animation));
        }
    }
}


