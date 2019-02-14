#pragma once

#include <exception>
#include <ole2.h>

namespace uc
{
    namespace exception
    {
        class exception : public std::exception
        {
            using base = std::exception;

            public:

            exception(const char* e) : base(e)
            {

            }

        };

        namespace windows
        {
            class com_exception : public exception
            {
                using base = exception;
                
                public:

                com_exception(const HRESULT hr) : base("com exception")
                    , m_hr(hr)
                {

                }

                private:

                HRESULT m_hr;
            };

            class win32_exception : public exception
            {
                using base = exception;

                public:

                win32_exception()  : base("win32_exception")
                {

                }
            };

            template < typename exception > void throw_if_failed( HRESULT hr )
            {
                if (hr != S_OK)
                {
                    throw exception(hr);
                }
            }

            template < typename exception > void throw_if_failed( BOOL result)
            {
                if (!result)
                {
                    throw exception();
                }
            }
        }
    }
}

