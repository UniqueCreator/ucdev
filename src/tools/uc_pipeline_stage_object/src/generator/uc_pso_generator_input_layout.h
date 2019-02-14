#pragma once

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_includes.h>
#include <uc_pso_ast_adapted.h>

#include <generator/uc_pso_generator_input_layout_element.h>


BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::input_layout_proxy,
    m_elements,
    m_element_count
)

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct input_layout : karma::grammar< iterator, ast::input_layout_proxy() >
            {
                using base = karma::grammar< iterator, ast::input_layout_proxy() >;

            public:
                input_layout() : base(m_start, "input_layout")
                {
                    karma::uint_type   uint_;
                    karma::float_type  float_;
                    karma::skip_type   skip_;
                    karma::string_type string_;

                    m_start = 
                        "{{"
                        << m_elements
                        << "},"
                        << uint_
                        << "}"
                        ;
                }

                karma::rule<iterator, ast::input_layout_proxy()>   m_start;
                input_layout_element_vector< iterator >            m_elements;
            };
        }
    }
}


