#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast.h>
#include <generator/uc_pso_generator_nil.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename t, uint32_t >
            std::string optional_shader_type_name()
            {
                return "{nullptr,0U}";
            }

            template <typename iterator, typename optional_shader_type, uint32_t ordinal >
            struct optional_shader : karma::grammar< iterator, optional_shader_type() >
            {
                using base = karma::grammar< iterator, optional_shader_type() >;

                public:
                optional_shader() : base(m_value)
                {
                    m_string = karma::string << "()";
                    m_value  = ( m_string | optional_shader_type_name<optional_shader_type, ordinal>() );
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                karma::rule<iterator, std::string()>            m_string;
                karma::rule<iterator, optional_shader_type()  >   m_value;
            };
        }
    }
}


