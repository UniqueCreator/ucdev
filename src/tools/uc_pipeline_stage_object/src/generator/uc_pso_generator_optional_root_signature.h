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
            template <typename iterator >
            struct optional_root_signature : karma::grammar< iterator, ast::optional_root_signature() >
            {
                using base = karma::grammar< iterator, ast::optional_root_signature() >;

                public:

                optional_root_signature() : base(m_value)
                {
                    m_string = karma::string << "()";
                    m_value  = ( m_string | "nullptr" );
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                karma::rule<iterator, std::string()>                         m_string;
                karma::rule<iterator, ast::optional_root_signature()  >      m_value;
            };
        }
    }
}


