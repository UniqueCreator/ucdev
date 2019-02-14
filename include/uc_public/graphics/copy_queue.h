#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle;

        class CopyQueue : private NonCopyable
        {

            public:

            class Impl;

            UC_ENGINE_API ~CopyQueue();

            UC_ENGINE_API CopyQueue(CopyQueue&&) = default;
            UC_ENGINE_API CopyQueue& operator=(CopyQueue&&);

            UC_ENGINE_API void WaitForIdleGpu();
            UC_ENGINE_API void InsertWaitOn(const FenceHandle* v);
            UC_ENGINE_API void WaitForFence(const FenceHandle* v);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

            protected:

            CopyQueue();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<CopyQueue::Impl>;
    }
}


