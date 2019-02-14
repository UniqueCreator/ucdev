#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle : private NonCopyable
        {
            public:
            class Impl;
            UC_ENGINE_API ~FenceHandle();

            UC_ENGINE_API FenceHandle(FenceHandle&&) = default;
            UC_ENGINE_API FenceHandle& operator=(FenceHandle&&);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            protected:
            FenceHandle();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<FenceHandle::Impl>;
    }
}


