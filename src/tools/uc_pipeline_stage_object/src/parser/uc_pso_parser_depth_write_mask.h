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
            struct depth_write_mask : qi::grammar< iterator, D3D12_DEPTH_WRITE_MASK(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_DEPTH_WRITE_MASK(), ascii::space_type  >;

            public:
                depth_write_mask() : base(m_start, "depth_write_mask")
                {
                    m_value.add
                    ("Zero", D3D12_DEPTH_WRITE_MASK_ZERO)
                        ("All", D3D12_DEPTH_WRITE_MASK_ALL)
                        ;

                    qi::lit_type    lit;
                    m_start = lit(".DepthWriteMask") > '=' > m_value;
                }

                qi::symbols<char, D3D12_DEPTH_WRITE_MASK>                              m_value;
                qi::rule<iterator, D3D12_DEPTH_WRITE_MASK(), ascii::space_type >       m_start;
            };
        }
    }
}


