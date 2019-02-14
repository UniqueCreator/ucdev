#include "pch.h"

#include <uc_public/assets/animations/skeleton_factory.h>
#include <uc_public/uc_pimpl_impl.h>
#include <uc_public/assets/animations/skeleton.h>

#include "skeleton_factory_impl.h"
#include "skeleton_impl.h"
#include <uc_dev/gx/lip/file.h>

namespace UniqueCreator
{
    namespace Assets
    {
        template details::pimpl<SkeletonFactory::Impl>;

        class SkeletonInternal : public Skeleton
        {
        public:

            SkeletonInternal(uc::lip::unique_lip_pointer<uc::lip::skeleton> && skeleton)
            {
                m_impl = std::move(skeleton);
            }
        };

        SkeletonFactory::SkeletonFactory()
        {

        }

        SkeletonFactory::~SkeletonFactory() = default;
        SkeletonFactory& SkeletonFactory::operator=(SkeletonFactory&&) = default;

        SkeletonFactory::Impl*   SkeletonFactory::GetImpl()
        {
            return m_impl.get();
        }

        const SkeletonFactory::Impl*   SkeletonFactory::GetImpl() const
        {
            return m_impl.get();
        }

        std::unique_ptr<Skeleton> SkeletonFactory::CreateFromFile(const wchar_t* fileName)
        {
            auto skeleton = uc::lip::create_from_compressed_lip_file<uc::lip::skeleton>(fileName);
            return std::make_unique<SkeletonInternal>(std::move(skeleton));
        }
    }
}


