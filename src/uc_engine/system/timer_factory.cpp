#include "pch.h"

#include <uc_public/system/timer_factory.h>
#include <uc_public/uc_pimpl_impl.h>

#include "timer_factory_impl.h"
#include "timer_impl.h"


namespace UniqueCreator
{
    namespace System
    {
        template details::pimpl<TimerFactory::Impl>;

        class TimerInternal : public Timer
        {
        public:
            TimerInternal(std::unique_ptr<uc::sys::profile_timer>&& t)
            {
                m_impl = std::move(t);
            }
        };

        TimerFactory::TimerFactory()
        {

        }

        TimerFactory::~TimerFactory() = default;
        TimerFactory& TimerFactory::operator=(TimerFactory&&) = default;

        TimerFactory::Impl*   TimerFactory::GetImpl()
        {
            return m_impl.get();
        }

        const TimerFactory::Impl*   TimerFactory::GetImpl() const
        {
            return m_impl.get();
        }

        UC_ENGINE_API std::unique_ptr<Timer> TimerFactory::Create()
        {
            return std::make_unique<TimerInternal>(std::make_unique<uc::sys::profile_timer>());
        }
    }
}


