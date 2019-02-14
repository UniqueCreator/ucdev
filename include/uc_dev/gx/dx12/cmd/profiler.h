#pragma once

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            template <typename t> class profile_event;

            template <typename t> class profile_event<t*>
            {
                public:
                profile_event(t* e, const wchar_t* name) : m_t(e)
                {
                    m_t->pix_begin_event(name);
                }

                profile_event(profile_event&& o)
                {
                    m_t = o.m_t;
                    o.m_t = nullptr;
                }

                profile_event& operator=(profile_event&& o)
                {
                    m_t = o.m_t;
                    o.m_t = nullptr;
                    return *this;
                }

                ~profile_event()
                {
                    if (m_t)
                    {
                        m_t->pix_end_event();
                    }
                }

                private:
                t* m_t = nullptr;
                profile_event(const profile_event&);
                profile_event& operator=(const profile_event&);
            };

            template <typename t> inline profile_event<t*> make_profile_event(t* e, const wchar_t* name)
            {
                return profile_event<t*>(e, name);
            }
        }
    }
}
