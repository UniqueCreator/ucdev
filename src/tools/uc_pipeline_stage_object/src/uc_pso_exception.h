#pragma once

#include <exception>

#include "uc_error_messages.h"

namespace uc
{
    namespace pso
    {
        class exception : public std::exception
        {
            using base = std::exception;

            public:

            exception( error_category category ) : base("pso_exception")
            , m_category(category)
            {

            }

            private:

            error_category m_category;
        };

        class file_not_found_exception : public exception
        {
            using base = exception;

        public:

            file_not_found_exception(const std::string& file_name) : base(error_category::io)
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

        class input_exception : public exception
        {
            using base = exception;

            public:

            input_exception( const std::string& o) : base(error_category::input_parameters)
            , m_option(o)
            {

            }

            const std::string& option() const
            {
                return m_option;
            }

            private:
            std::string m_option;

        };

        class missing_input_exception : public exception
        {
            using base = exception;

        public:
            enum struct missing_part
            {
                pipeline_state_object,
                rasterizer_description,
                blend_description,
                depth_stencil_description
            };

            missing_input_exception( missing_part part ) : base(error_category::input_parameters)
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

        class missing_variable_input_exception : public missing_input_exception
        {
            using base = missing_input_exception;

        public:

            missing_variable_input_exception( missing_input_exception::missing_part part, const std::string& variable ) : base( part)
            , m_variable(variable)
            {

            }

            const std::string& variable() const
            {
                return m_variable;
            }

        private:
            std::string m_variable;
        };

        inline void raise_error(error_category category)
        {
            throw exception(category);
        }

        inline void raise_error()
        {
            throw exception(error_category::general);
        }

        template <typename e, typename ...args > void raise_error( args&&... a )
        {
            throw e(std::forward<args>(a)...);
        }
    }
}
