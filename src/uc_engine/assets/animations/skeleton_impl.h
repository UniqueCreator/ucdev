#pragma once

#include <uc_public/assets/animations/skeleton.h>
#include <uc_dev/gx/lip/animation.h>

namespace UniqueCreator
{
    namespace Assets
    {
        class Skeleton::Impl
        {
        public:

            Impl() {}

            Impl(uc::lip::unique_lip_pointer<uc::lip::skeleton> && skeleton) : m_skeleton(std::move(skeleton))
            {

            }

            uc::lip::unique_lip_pointer<uc::lip::skeleton> m_skeleton;
        };
    }
}


