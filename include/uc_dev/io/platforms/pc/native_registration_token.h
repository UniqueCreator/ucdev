#pragma once

#include <EventToken.h>

#include <winrt/base.h>

namespace uc
{
    namespace io
    {
        struct registration_token
        {
            EventRegistrationToken m_token;

            static registration_token null()
            {
                return registration_token();
            }

            registration_token()
            {
                m_token.value = 0xFFFFFFFFFFFFFFFFUL;
            }

            registration_token(EventRegistrationToken token) : m_token(token)
            {

            }

            operator EventRegistrationToken() const
            {
                return m_token;
            }

			operator winrt::event_token() const
			{
				return winrt::event_token{ m_token.value };
			}
        };

        inline bool operator==(const registration_token& a, const registration_token& b)
        {
            return a.m_token.value == b.m_token.value;
        }

        inline bool operator!=(const registration_token& a, const registration_token& b)
        {
            return a.m_token.value != b.m_token.value;
        }
    }
}
