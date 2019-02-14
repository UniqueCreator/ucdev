#pragma once

#include <uc_public/uc_implementation.h>

#include <uc_public/graphics/graphics_types.h>

namespace UniqueCreator
{
    namespace Graphics
    {
        class FenceHandle;

        class ComputeQueue : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API ~ComputeQueue();

            UC_ENGINE_API ComputeQueue(ComputeQueue&&) = default;
            UC_ENGINE_API ComputeQueue& operator=(ComputeQueue&&);

            UC_ENGINE_API void WaitForIdleGpu();
            UC_ENGINE_API void InsertWaitOn(const FenceHandle* v);
            UC_ENGINE_API void WaitForFence(const FenceHandle* v);

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            ComputeQueue();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<ComputeQueue::Impl>;
    }
}


