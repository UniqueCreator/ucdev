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
            struct classification : qi::grammar< iterator, D3D12_INPUT_CLASSIFICATION(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_INPUT_CLASSIFICATION(), ascii::space_type  >;

            public:
                classification() : base(m_start, "classification")
                {
                    m_value.add
                    ("PER_VERTEX_DATA", D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
                    ("PER_INSTANCE_DATA", D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA)
                    ;

                    qi::lit_type    lit;
                    m_start = lit(".Classification") > '=' > m_value;
                }

                qi::symbols<char, D3D12_INPUT_CLASSIFICATION>                              m_value;
                qi::rule<iterator, D3D12_INPUT_CLASSIFICATION(), ascii::space_type >       m_start;
            };
        }
    }
}


