#include "pch.h"

#include <uc_public/animations/skeleton_instance_factory.h>
#include <uc_public/uc_pimpl_impl.h>

#include "../assets/animations/skeleton_impl.h"
#include "skeleton_instance_factory_impl.h"
#include "skeleton_instance_impl.h"
#include <uc_dev/gx/lip/file.h>

namespace UniqueCreator
{
    namespace Animations
    {
        template details::pimpl<SkeletonInstanceFactory::Impl>;

        class SkeletonInstanceInternal : public SkeletonInstance
        {
            public:
            SkeletonInstanceInternal(std::unique_ptr<uc::gx::anm::skeleton_instance>&& instance)
            {
                m_impl = std::move(instance);
            }
        };

        SkeletonInstanceFactory::SkeletonInstanceFactory()
        {

        }

        SkeletonInstanceFactory::~SkeletonInstanceFactory() = default;
        SkeletonInstanceFactory& SkeletonInstanceFactory::operator=(SkeletonInstanceFactory&&) = default;

        SkeletonInstanceFactory::Impl*   SkeletonInstanceFactory::GetImpl()
        {
            return m_impl.get();
        }

        const SkeletonInstanceFactory::Impl*   SkeletonInstanceFactory::GetImpl() const
        {
            return m_impl.get();
        }

        UC_ENGINE_API std::unique_ptr<SkeletonInstance> SkeletonInstanceFactory::Create( const Assets::Skeleton* skeleton )
        {
            auto s = skeleton->GetImpl()->m_skeleton.get();
            return std::make_unique<SkeletonInstanceInternal>(std::make_unique<uc::gx::anm::skeleton_instance>(s));
        }
    }
}


