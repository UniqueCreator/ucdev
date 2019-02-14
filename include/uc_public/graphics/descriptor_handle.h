#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class DescriptorHandle : private NonCopyable
        {
            public:

            class Impl;

            UC_ENGINE_API ~DescriptorHandle();

            UC_ENGINE_API DescriptorHandle(DescriptorHandle&&) = default;
            UC_ENGINE_API DescriptorHandle& operator=(DescriptorHandle&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:

            DescriptorHandle();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<DescriptorHandle::Impl>;
    }
}


