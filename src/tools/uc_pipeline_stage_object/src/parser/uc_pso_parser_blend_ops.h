#pragma once

#include "pipeline_state_types.h"
#include <parser/uc_pso_parser_includes.h>

#include "uc_pso_parser_blend.h"
#include "uc_pso_parser_logic_op.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
   

            template <typename iterator >
            struct src_blend : qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >;

                public:
                src_blend() : base(m_start, "src_blend")
                {
                    qi::lit_type    lit;
                    m_start = lit(".SrcBlend") > '=' > m_value;
                }

                blend                                                           m_value;
                qi::rule<iterator, D3D12_BLEND(), ascii::space_type >           m_start;
            };

            template <typename iterator >
            struct dest_blend : qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >;

            public:
                dest_blend() : base(m_start, "dest_blend")
                {
                    qi::lit_type    lit;
                    m_start = lit(".DestBlend") > '=' > m_value;
                }

                blend                                                           m_value;
                qi::rule<iterator, D3D12_BLEND(), ascii::space_type >           m_start;
            };

            template <typename iterator >
            struct src_blend_alpha : qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >;

            public:
                src_blend_alpha() : base(m_start, "src_blend_alpha")
                {
                    qi::lit_type    lit;
                    m_start = lit(".SrcBlendAlpha") > '=' > m_value;
                }

                blend                                                           m_value;
                qi::rule<iterator, D3D12_BLEND(), ascii::space_type >           m_start;
            };


            template <typename iterator >
            struct dest_blend_alpha : qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_BLEND(), ascii::space_type  >;

            public:
                dest_blend_alpha() : base(m_start, "dest_blend_alpha")
                {
                    qi::lit_type    lit;
                    m_start = lit(".DestBlendAlpha") > '=' > m_value;
                }

                blend                                                           m_value;
                qi::rule<iterator, D3D12_BLEND(), ascii::space_type >           m_start;
            };

            //================================================================================

            template <typename iterator >
            struct blend_op: qi::grammar< iterator, D3D12_BLEND_OP(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_BLEND_OP(), ascii::space_type  >;

            public:
                blend_op() : base(m_start, "blend_op")
                {
                    qi::lit_type    lit;
                    m_start = lit(".BlendOp") > '=' > m_value;
                }

                blend_op_                                                       m_value;
                qi::rule<iterator, D3D12_BLEND_OP(), ascii::space_type >        m_start;
            };

            template <typename iterator >
            struct blend_op_alpha : qi::grammar< iterator, D3D12_BLEND_OP(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_BLEND_OP(), ascii::space_type  >;

            public:
                blend_op_alpha() : base(m_start, "blend_op_alpha")
                {
                    qi::lit_type    lit;
                    m_start = lit(".BlendOpAlpha") > '=' > m_value;
                }

                blend_op_                                                       m_value;
                qi::rule<iterator, D3D12_BLEND_OP(), ascii::space_type >        m_start;
            };

            template <typename iterator >
            struct logic_op : qi::grammar< iterator, D3D12_LOGIC_OP(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_LOGIC_OP(), ascii::space_type  >;

            public:

                logic_op() : base(m_start, "logic_op")
                {
                    qi::lit_type    lit;
                    m_start = lit(".LogicOp") > '=' > m_value;
                }

                logic_op_                                                       m_value;
                qi::rule<iterator, D3D12_LOGIC_OP(), ascii::space_type >        m_start;
            };


            /*
            D3D12_BLEND_OP BlendOp;
            D3D12_BLEND_OP BlendOpAlpha;
            */
        }
    }
}


