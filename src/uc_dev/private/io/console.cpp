#include "pch.h"

#include <uc_dev/io/console.h>

#include <atomic>

namespace uc
{
    namespace io
    {
        namespace console
        {
            thread_info::thread_info()
            {
                get_updater()->register_thread_info(this);
            }

            thread_info::~thread_info()
            {
                get_updater()->unregister_thread_info(this);
            }

            console_updater::console_updater() : m_runner(this)
            {

            }

            void console_updater::register_notifier(notifier* n)
            {
                std::lock_guard<std::mutex> guard(m_lock);
                m_notifiers.push_back(n);
            }

            void console_updater::unregister_notifier(notifier* n)
            {
                std::lock_guard<std::mutex> guard(m_lock);
               
                auto v = std::remove_if(m_notifiers.begin(), m_notifiers.end(), [&n](const auto& p)
                {
                    return p == n;
                });

                v;
            }

            void console_updater::register_thread_info(thread_info*      info)
            {
                std::lock_guard<std::mutex> guard(m_lock);
                m_infos.push_back(info);
            }

            void console_updater::unregister_thread_info(thread_info*    info)
            {
                std::lock_guard<std::mutex> guard(m_lock);

                auto v = std::remove_if(m_infos.begin(), m_infos.end(), [&info](const auto& p)
                {
                    return p == info;
                });

                v;
            }

            void console_updater::update()
            {
                std::vector<message> messages;
                messages.reserve(200);

                {
                    std::lock_guard<std::mutex> guard(m_lock);

                    if (!m_infos.empty())
                    {
                        for (auto&& m : m_infos)
                        {
                            if (!m->m_buffers.empty() )
                            {
                                std::copy(m->m_buffers.cbegin(), m->m_buffers.cend(), std::back_inserter(messages));
                                m->m_buffers.reset();
                            }
                        }
                    }
                }

                if (!messages.empty())
                {
                    std::sort(messages.begin(), messages.end());

                    for (auto&& notifier : m_notifiers)
                    {
                        notifier->handle_messages(&messages[0], &messages[0] + messages.size());
                    }
                }
            }

            void runner_thread(bool& stop_thread, console_updater* updater)
            {
                while (stop_thread == false)
                {
                    //sleep 1 second, notify everybody, sleep again
                    updater->update();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }

            runner::runner(console_updater* u) : m_thread(runner_thread, std::ref(m_stop_thread), u)
            {

            }

            runner::~runner()
            {
                try
                {
                    stop();
                }
                catch (...)
                {

                }
            }

            void runner::stop()
            {
                m_stop_thread = true;
                std::atomic_thread_fence(std::memory_order_release);
                m_thread.join();
            }

         
        }
    }
}

