#pragma once
#include <parser/uc_pso_parser_includes.h>
#include "uc_pso_parser_color_write_enable.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator >
            struct render_target_write_mask : qi::grammar< iterator, uint32_t(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, uint32_t(), ascii::space_type  >;

            public:

                render_target_write_mask() : base(m_start, "render_target_write_mask")
                {
                    qi::lit_type    lit;
                    m_start = lit(".RenderTargetWriteMask") > '=' > m_flag;
                }

                color_write_enable_flag<iterator>                           m_flag;
                qi::rule<iterator, uint32_t(), ascii::space_type >          m_start;
            };
        }
    }
}
