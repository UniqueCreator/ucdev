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
            struct primitive_topology_type : qi::grammar< iterator, D3D12_PRIMITIVE_TOPOLOGY_TYPE(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_PRIMITIVE_TOPOLOGY_TYPE(), ascii::space_type  >;

            public:

                primitive_topology_type() : base(m_start, "primitive_topology_type")
                {
                    m_value.add

                    ("None", D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED)
                    ("Point", D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT)
                    ("Line", D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE)
                    ("Triangle", D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
                    ("Patch", D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH)
                    ;

                    qi::lit_type    lit;
                    m_start = lit(".PrimitiveTopology") > '=' > m_value;
                }

                qi::symbols<char, D3D12_PRIMITIVE_TOPOLOGY_TYPE>                              m_value;
                qi::rule<iterator, D3D12_PRIMITIVE_TOPOLOGY_TYPE(), ascii::space_type >       m_start;
            };
        }
    }
}


