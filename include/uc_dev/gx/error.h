#pragma once

#include <exception>
#include <string>
#include <assert.h>

namespace uc
{
    namespace gx
    {
        class exception : public std::exception
        {
            using base = std::exception;
        };

        //todo: remove this
        class assimp_exception : public std::exception
        {
            using base = std::exception;

            public:

            assimp_exception(const char* m ) : base(m)
            {

            }

            private:
        };

        //todo: remove this
        class fbx_exception : public std::exception
        {
            using base = std::exception;

            public:

            fbx_exception(const char* m) : m_exception(m)
            {

            }

            char const* what() const override
            {
                return m_exception.c_str();
            }

            private:

            std::string m_exception;
        };

        template < typename t, typename ...args >
        inline void throw_exception( args&&... a )
        {
            throw t(std::forward<args>(a)...);
        }

        
        inline void throw_if_failed( HRESULT )
        {
			assert(false);
        }
    }
}
