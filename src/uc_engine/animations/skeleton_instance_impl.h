#pragma once

#include <uc_public/animations/skeleton_instance.h>
#include <uc_dev/gx/anm/skeleton_instance.h>
#include <uc_dev/gx/anm/transforms.h>

namespace UniqueCreator
{
    namespace Animations
    {
        class SkeletonInstance::Impl
        {
        public:

            Impl() {}

            Impl(std::unique_ptr<uc::gx::anm::skeleton_instance>&& instance) : m_skeleton_instance(std::move(instance))
            {
                auto s = m_skeleton_instance->local_transforms().size();
                m_concatenated_transforms.reserve(s);

                for (auto&& i = 0U; i < s; ++i)
                {
                    m_concatenated_transforms.push_back(uc::math::identity_matrix());
                }
            }

            std::unique_ptr<uc::gx::anm::skeleton_instance> m_skeleton_instance;
            std::vector<uc::math::float4x4 >                m_concatenated_transforms;
        };
    }
}


