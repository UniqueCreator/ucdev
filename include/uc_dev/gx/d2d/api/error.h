#pragma once

#include <Windows.h>
#include <exception>

namespace uc
{
    namespace gx
    {
        namespace d2d
        {
            class exception : public std::exception
            {
                using base = std::exception;

            public:

                using base::base;
            };

            class out_of_memory_exception : public exception
            {
                using base = exception;

            public:

                using base::base;
            };


            inline void throw_if_failed(HRESULT hr)
            {
                if (FAILED(hr))
                {
                    throw exception("COM exception: code unkown TODO: get the code");
                }
            }

            inline void raise_out_of_memory_exception()
            {
                throw out_of_memory_exception("out of memory");
            }
        }
    }
}

