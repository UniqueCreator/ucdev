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
            struct consertvative_rasterization_mode : qi::grammar< iterator, D3D12_CONSERVATIVE_RASTERIZATION_MODE(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_CONSERVATIVE_RASTERIZATION_MODE(), ascii::space_type  >;

            public:
                consertvative_rasterization_mode() : base(m_start, "consertvative_rasterization_mode")
                {
                    m_value.add
                    ("Off", D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF)
                    ("On", D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON)
                    ;

                    qi::lit_type    lit;
                    m_start = lit(".ConservativeRasterizationMode") > '=' > m_value;
                }

                qi::symbols<char, D3D12_CONSERVATIVE_RASTERIZATION_MODE>                              m_value;
                qi::rule<iterator, D3D12_CONSERVATIVE_RASTERIZATION_MODE(), ascii::space_type >       m_start;
            };
        }
    }
}


