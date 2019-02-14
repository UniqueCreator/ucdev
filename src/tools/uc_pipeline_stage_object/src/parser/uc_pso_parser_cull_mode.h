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
            struct cull_mode : qi::grammar< iterator, D3D12_CULL_MODE(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_CULL_MODE(), ascii::space_type  >;

            public:
                cull_mode() : base(m_start, "cull_mode")
                {
                    m_value.add
                    ("None", D3D12_CULL_MODE_NONE)
                        ("Front", D3D12_CULL_MODE_FRONT)
                        ("Back", D3D12_CULL_MODE_BACK)
                        ;

                    qi::lit_type    lit;
                    m_start = lit(".CullMode") > '=' > m_value;
                }

                qi::symbols<char, D3D12_CULL_MODE>                              m_value;
                qi::rule<iterator, D3D12_CULL_MODE(), ascii::space_type >       m_start;
            };
        }
    }
}


