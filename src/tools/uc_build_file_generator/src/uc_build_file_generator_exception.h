#pragma once

#include <exception>
#include <string>

namespace uc
{
    namespace build_file_generator
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

        class file_not_found_exception : public exception
        {
            using base = exception;

            public:

            file_not_found_exception( const std::string& file_name ) : base( "file not found : " + file_name )
                , m_file_name(file_name)
            {

            }

            const std::string& file_name() const
            {
                return m_file_name;
            }

            private:
            std::string m_file_name;
        };

        class missing_input_exception : public exception
        {
            using base = exception;

            public:

            enum struct missing_part
            {
                mode
            };

            std::string part_to_string(missing_part p)
            {
                switch (p)
                {
                    case missing_part::mode:                return "missing mode";
                    /*
                    case missing_part::output_file_name:    return "missing output file name";
                    case missing_part::textures:            return "missing textures";
                    case missing_part::textures_formats:    return "missing textures formats";
                    */
                    default: return "";
                 
                }
            }

            missing_input_exception( missing_part part ) : base(part_to_string(part) )
            , m_part(part)
            {

            }

            missing_part part() const
            {
                return m_part;
            }

            private:
            missing_part    m_part;
        };

        template <typename e, typename ...args > void raise_error( args&&... a )
        {
            throw e(std::forward<args>(a)...);
        }
    }
}
