#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator >
            struct index_buffer_strip_cut_value : qi::grammar< iterator, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE(), ascii::space_type  >;

            public:

                index_buffer_strip_cut_value() : base(m_start, "index_buffer_strip_cut_value")
                {
                    m_value.add

                    ("Disabled", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED)
                    ("0xFFFF", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF)
                    ("0xFFFFFFFF", D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF)
                    ;

                    qi::lit_type    lit;
                    m_start = lit(".StripCutValue") > '=' > m_value;
                }

                qi::symbols<char, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>                              m_value;
                qi::rule<iterator, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE(), ascii::space_type >       m_start;
            };
        }
    }
}


