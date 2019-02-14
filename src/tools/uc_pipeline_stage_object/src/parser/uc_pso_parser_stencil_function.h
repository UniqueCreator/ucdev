#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

#include "uc_pso_parser_comparison_function.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator >
            struct stencil_function : qi::grammar< iterator, D3D12_COMPARISON_FUNC(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_COMPARISON_FUNC(), ascii::space_type  >;

                public:
                stencil_function() : base(m_start, "stencil_function")
                {
                    qi::lit_type    lit;
                    m_start = lit(".StencilFunction") > '=' > m_value;
                }

                comparison_func                                                 m_value;
                qi::rule<iterator, D3D12_COMPARISON_FUNC(), ascii::space_type > m_start;
            };
        }
    }
}


