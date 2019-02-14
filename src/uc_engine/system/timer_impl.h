#pragma once

#include <uc_public/system/timer.h>
#include <uc_dev/sys/profile_timer.h>

namespace UniqueCreator
{
    namespace System
    {
        class Timer::Impl
        {
        public:
            Impl() {}
            Impl(std::unique_ptr<uc::sys::profile_timer>&& t) : m_timer(std::move(t)) {}

            std::unique_ptr<uc::sys::profile_timer> m_timer;
        };
    }
}


