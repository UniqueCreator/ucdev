#pragma once

#include <uc_dev/error/error.h>

namespace uc
{
    namespace io
    {
        using base_exception = ::uc::exception::exception;
        using com_base_exception = ::uc::exception::windows::com_exception;

        class exception : public base_exception
        {
            using base = base_exception;

        public:
            exception(const char* c) : base(c)
            {

            }
        };

        class com_exception : public com_base_exception
        {
            using base = com_base_exception;

        public:

            com_exception(HRESULT hr) : base(hr)
            {

            }
        };

        class out_of_memory_exception : public exception
        {
            using base = exception;

        public:

            out_of_memory_exception() : base("out of memory")
            {

            }
        };

        inline void throw_if_failed(HRESULT hr)
        {
            if (FAILED(hr))
            {
                throw com_exception(hr);
            }
        }

        inline void raise_out_of_memory_exception()
        {
            throw out_of_memory_exception();
        }

    }
}
