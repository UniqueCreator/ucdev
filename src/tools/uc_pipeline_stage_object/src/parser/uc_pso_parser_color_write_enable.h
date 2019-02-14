#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"
#include <cstdint>
#include <boost/spirit/include/phoenix_operator.hpp>


namespace uc
{
    namespace pso
    {
        namespace parser
        {
            struct color_write_enable : qi::symbols<char, D3D12_COLOR_WRITE_ENABLE>
            {
                color_write_enable()
                {
                    this->add
                    ("Red", D3D12_COLOR_WRITE_ENABLE_RED)
                    ("Green", D3D12_COLOR_WRITE_ENABLE_GREEN)
                    ("Blue", D3D12_COLOR_WRITE_ENABLE_BLUE)
                    ("Alpha", D3D12_COLOR_WRITE_ENABLE_ALPHA)
                    ("All", D3D12_COLOR_WRITE_ENABLE_ALL)
                    ;
                    this->name("color_write_enable");
                }
            };

            template <typename iterator >
            struct color_write_enable_flag : qi::grammar< iterator, uint32_t(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, uint32_t(), ascii::space_type  >;

            public:

                color_write_enable_flag() : base(m_expression, "color_write_enable_flag")
                {
                    qi::_val_type _val;
                    qi::_1_type   _1;
                    //qi::uint_type uint_;

                    m_expression =
                                m_term[_val = _1]
                                > *( ('|' > m_term ) [_val |= _1])
                        ;
                }

                color_write_enable  m_term;
                qi::rule<iterator, uint32_t(), ascii::space_type >          m_expression;
            };
        }
    }
}
