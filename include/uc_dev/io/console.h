#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iterator>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace io
    {
        namespace console
        {
            struct message
            {
                std::chrono::time_point<std::chrono::steady_clock>   m_time;
                std::wstring                                         m_text;
            };

            inline bool operator<( const message& l, const message& r )
            {
                return (l.m_time < r.m_time);
            }

            class notifier : private util::noncopyable
            {
                public:

                virtual ~notifier()
                {

                }

                void handle_messages(const message* message_begin, const message* message_end)
                {
                    do_handle_messages(message_begin, message_end);
                }

                private:

                virtual void do_handle_messages(const message* message_begin, const message* message_end) = 0;
            };

            class buffer
            {
                public:

                using iterator = std::vector<message>::iterator;
                using const_iterator = std::vector<message>::const_iterator;

                iterator begin()
                {
                    return m_messages.begin();
                }

                iterator end()
                {
                    return m_messages.end();
                }

                const_iterator cbegin() const
                {
                    return m_messages.cbegin();
                }

                const_iterator cend() const
                {
                    return m_messages.cend();
                }

                void    register_message(message&& m)
                {
                    m_messages.emplace_back(std::move(m));
                }

                void    reset()
                {
                    std::vector < message > temp;
                    m_messages.swap(temp);
                }

                bool empty() const
                {
                    return m_messages.empty();
                }

                private:
                std::vector < message >	m_messages;
            };

            class  console_updater;

            //public interfaces
            class runner : private util::noncopyable
            {
                public:

                runner(console_updater* u);
                ~runner();

                private:
                void stop();
                bool                m_stop_thread;
                std::thread         m_thread;
            };

            struct thread_info : private util::noncopyable
            {
                buffer      m_buffers;

                thread_info();
                ~thread_info();
            };
            
            class console_updater : private util::noncopyable
            {
                public:

                console_updater();

                void register_thread_info(thread_info*      info);
                void unregister_thread_info(thread_info*    info);
                void register_notifier(notifier* n);
                void unregister_notifier(notifier* n);

                void update();

            private:
                std::vector< thread_info* >             m_infos;
                std::vector< notifier*>                 m_notifiers;
                std::mutex                              m_lock;
                runner                                  m_runner;
            };

            inline console_updater* get_updater()
            {
                static thread_local console_updater u;
                return &u;
            }

            inline thread_info* get_thread_info()
            {
                static thread_local thread_info ti;
                return &ti;
            }

            inline void	write(const wchar_t* s)
            {
                message m = { std::chrono::steady_clock::now(), std::wstring(s) };
                get_thread_info()->m_buffers.register_message(std::move(m));
            }
       }
    }
}

