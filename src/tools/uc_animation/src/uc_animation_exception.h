#pragma once

#include <exception>
#include <string>

namespace uc
{
    namespace animation
    {
        class exception : public std::exception
        {
            using base = std::exception;

            public:

            exception( const std::string& message) : base(message.c_str() ), m_message(message)
            {

            }

            private:
            std::string m_message;
        };

        template <typename e, typename ...args > void raise_error( args&&... a )
        {
            throw e(std::forward<args>(a)...);
        }
    }
}
