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
            struct pipeline_state_flags : qi::symbols<char, D3D12_PIPELINE_STATE_FLAGS>
            {
                pipeline_state_flags()
                {
                    this->add
                    ("None", D3D12_PIPELINE_STATE_FLAG_NONE)
                    ("ToolDebug", D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG)
                    ;
                    this->name("pipeline_state_flags");
                }
            };

            template <typename iterator >
            struct pipeline_state_enable_flags : qi::grammar< iterator, uint32_t(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, uint32_t(), ascii::space_type  >;

            public:

                pipeline_state_enable_flags() : base(m_expression, "pipeline_state_enable_flags")
                {
                    qi::_val_type _val;
                    qi::_1_type   _1;
                    qi::uint_type uint_;

                    m_expression =
                                m_term[_val = _1]
                                > *( ('|' > m_term ) [_val |= _1])
                        ;
                }

                pipeline_state_flags  m_term;
                qi::rule<iterator, uint32_t(), ascii::space_type >          m_expression;
            };

            template <typename iterator >
            struct pipeline_state_mask : qi::grammar< iterator, uint32_t(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, uint32_t(), ascii::space_type  >;

            public:

                pipeline_state_mask() : base(m_start, "pipeline_state_mask")
                {
                    qi::lit_type    lit;
                    m_start = lit(".Flags") > '=' > m_flag;
                }

                pipeline_state_enable_flags<iterator>                       m_flag;
                qi::rule<iterator, uint32_t(), ascii::space_type >          m_start;
            };
        }
    }
}
