#pragma once

#include "uc_pso_exception.h"

#include <resources_resource.h>

#include <Windows.h>

namespace uc
{
    namespace pso
    {
        inline HGLOBAL get_resource(size_t resource)
        {
            auto hrsrc = FindResourceA(nullptr, reinterpret_cast<const char*>(resource), "TEXT_FILE");
            if ( hrsrc)
            {
                auto r = LoadResource(nullptr, hrsrc);

                if (!r)
                {
                    raise_error();
                }
                else
                {
                    return r;
                }
            }
            else
            {
                raise_error();
            }
        }

        struct scoped_resource
        {
            explicit scoped_resource(HGLOBAL r) : m_r(r)
            {
                m_data = LockResource(r);
                if (!m_data)
                {
                    raise_error();
                }
            }

            ~scoped_resource()
            {
                UnlockResource(m_r);
            }

            template <typename t> t* data() const
            {
                return reinterpret_cast<t*>(m_data);
            }

            const char* c_str() const
            {
                return reinterpret_cast<const char*>(m_data);
            }

            private:

            HGLOBAL m_r;
            void* m_data;

            scoped_resource(const scoped_resource&) = delete;
            scoped_resource& operator=(const scoped_resource&) = delete;

        };
    }
}
