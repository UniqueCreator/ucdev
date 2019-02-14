#pragma once

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_includes.h>
#include <uc_pso_ast_adapted.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct so_declaration_entry : karma::grammar< iterator, ast::so_declaration_entry() >
            {

                using base = karma::grammar< iterator, ast::so_declaration_entry() >;

            public:

                so_declaration_entry() : base(m_start, "so_declaration_entry")
                {
                    karma::uint_type  uint_;
                    karma::float_type float_;
                    karma::string_type string_;

                    m_quoted_string = '"' << string_ << '"'; 
                    m_start = '{'
                        << uint_
                        << ',' << m_quoted_string
                        << ',' << uint_
                        << ',' << uint_
                        << ',' << uint_
                        << ',' << uint_
                        << '}'
                        ;
                }

                karma::rule<iterator, ast::so_declaration_entry()>   m_start;
                karma::rule<iterator, std::string()>                 m_quoted_string;
            };

            template <typename iterator>
            struct so_declaration_entry_element_vector : karma::grammar< iterator, std::vector< ast::so_declaration_entry >() >
            {
                using base = karma::grammar< iterator, std::vector< ast::so_declaration_entry >() >;

            public:
                so_declaration_entry_element_vector() : base(m_start, "so_declaration_entry_element_vector")
                {
                    m_start = '{'

                        << -(m_element % ',')

                        << '}'
                        ;
                }

                so_declaration_entry<iterator>                      m_element;
                karma::rule<iterator, std::vector< ast::so_declaration_entry>() >  m_start;
            };
        }
    }
}


