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
            struct fill_mode : qi::grammar< iterator, D3D12_FILL_MODE(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_FILL_MODE(), ascii::space_type  >;

            public:
                fill_mode() : base(m_start, "fill_mode")
                {
                    m_value.add
                    ("Wireframe", D3D12_FILL_MODE_WIREFRAME)
                    ("Solid", D3D12_FILL_MODE_SOLID)
                    ;

                    qi::lit_type    lit;
                    m_start = lit(".FillMode") > '=' > m_value;
                }

                qi::symbols<char, D3D12_FILL_MODE>                              m_value;
                qi::rule<iterator, D3D12_FILL_MODE(), ascii::space_type >       m_start;
            };
        }
    }
}


