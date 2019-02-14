#pragma once

#include <uc_public/uc_implementation.h>

namespace UniqueCreator
{
    namespace System
    {
        class Timer;

        class TimerFactory : private NonCopyable
        {
        public:

            class Impl;

            UC_ENGINE_API TimerFactory();
            UC_ENGINE_API ~TimerFactory();

            UC_ENGINE_API TimerFactory(TimerFactory&&) = default;
            UC_ENGINE_API TimerFactory& operator=(TimerFactory&&);

            UC_ENGINE_API std::unique_ptr<Timer> Create();

            Impl*         GetImpl();
            const Impl*   GetImpl() const;

        protected:
            details::pimpl<Impl> m_impl;
        };

        //private data, todo: fix visual assist warning
        extern template details::pimpl<TimerFactory::Impl>;
    }
}


