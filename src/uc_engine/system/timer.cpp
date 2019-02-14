#include "pch.h"

#include <uc_public/system/timer.h>
#include <uc_public/uc_pimpl_impl.h>

#include "timer_impl.h"

namespace UniqueCreator
{
    namespace System
    {
        template details::pimpl<Timer::Impl>;

        Timer::Timer()
        {

        }

        Timer::~Timer() = default;
        Timer& Timer::operator=(Timer&&) = default;

        Timer::Impl*   Timer::GetImpl()
        {
            return m_impl.get();
        }

        const Timer::Impl*   Timer::GetImpl() const
        {
            return m_impl.get();
        }

        void Timer::Reset()
        {
            GetImpl()->m_timer->reset();
        }

        double Timer::Seconds() const
        {
            return GetImpl()->m_timer->seconds();
        }

        double Timer::MilliSeconds() const
        {
            return GetImpl()->m_timer->milliseconds();
        }
    }
}
