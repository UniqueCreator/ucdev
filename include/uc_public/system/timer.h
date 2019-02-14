#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace System
    {
        class Timer : private NonCopyable
        {
        public:


            class Impl;

            UC_ENGINE_API ~Timer();

            UC_ENGINE_API Timer(Timer&&) = default;
            UC_ENGINE_API Timer& operator=(Timer&&);

            UC_ENGINE_API void Reset();

            UC_ENGINE_API double Seconds() const;
            UC_ENGINE_API double MilliSeconds() const;

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            Timer();
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<Timer::Impl>;
    }
}

