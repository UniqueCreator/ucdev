#pragma once

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast_adapted.h>

#include <generator/uc_pso_generator_classification.h>
#include <generator/uc_pso_generator_dxgi_format.h>


namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct input_layout_element : karma::grammar< iterator, ast::input_layout_element() >
            {
                using base = karma::grammar< iterator, ast::input_layout_element() >;

            public:
                input_layout_element() : base(m_start, "input_layout_element")
                {
                    karma::uint_type uint_;
                    karma::float_type float_;
                    karma::string_type string_;

                    m_quoted_string = '"' << karma::string << '"';

                    m_start = '{'

                        << m_quoted_string
                        << ',' << uint_
                        << ',' << m_dxgi_format

                        << ',' << uint_
                        << ',' << uint_

                        << ',' << m_classification
                        << ',' << uint_

                        << '}'
                        ;
                }

                classification<iterator>                            m_classification;
                dxgi_format<iterator>                               m_dxgi_format;
                karma::rule<iterator, ast::input_layout_element()>  m_start;
                karma::rule<iterator, std::string()>                m_quoted_string;
            };

            template <typename iterator>
            struct input_layout_element_vector : karma::grammar< iterator, std::vector< ast::input_layout_element >() >
            {
                using base = karma::grammar< iterator, std::vector< ast::input_layout_element >() >;

            public:

                input_layout_element_vector() : base(m_start, "input_layout_element_vector")
                {
                    m_start = '{'

                        << -(m_element % ',')

                        << '}'
                        ;
                }

                input_layout_element<iterator>                      m_element;
                karma::rule<iterator, std::vector< ast::input_layout_element>() >  m_start;
            };

            
        }
    }
}


