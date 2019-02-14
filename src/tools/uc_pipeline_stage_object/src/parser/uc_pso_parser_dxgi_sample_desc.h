#pragma once

#include <parser/uc_pso_parser_includes.h>

#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_named_value.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator>
            struct dxgi_sample_desc : public qi::grammar< iterator, DXGI_SAMPLE_DESC(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, DXGI_SAMPLE_DESC(), ascii::space_type >;

                dxgi_sample_desc() : base(m_start, "dxgi_sample_desc")
                    , m_count(".Count")
                    , m_quality(".Quality")
                {
                    using qi::lit;

                    m_start = lit(".SampleDescription")
                    > '=' > '{'
                    > m_count
                    > m_quality
                    > '}';

                    m_start.name("dxgi_sample_desc");
                }

                named_value_uint32<iterator> m_count;
                named_value_uint32<iterator> m_quality;
                qi::rule<iterator, DXGI_SAMPLE_DESC(), ascii::space_type > m_start;
            };
        }
    }
}