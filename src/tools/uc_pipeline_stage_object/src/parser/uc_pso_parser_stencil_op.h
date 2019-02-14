#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

#include "uc_pso_parser_stencil_function.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {

            struct stencil_operation : qi::symbols<char, D3D12_STENCIL_OP>
            {
                stencil_operation()
                {
                    this->add
                    ("Keep", D3D12_STENCIL_OP_KEEP)
                     ("Zero", D3D12_STENCIL_OP_ZERO)
                        ("Replace", D3D12_STENCIL_OP_REPLACE)
                        ("IncrementSaturate", D3D12_STENCIL_OP_INCR_SAT)
                        ("DecrementSaturate", D3D12_STENCIL_OP_DECR_SAT)
                        ("Invert", D3D12_STENCIL_OP_INVERT)
                        ("Increment", D3D12_STENCIL_OP_INCR)
                        ("Decrement", D3D12_STENCIL_OP_DECR)
                        ;
                    this->name("stencil_operation");
                }
            };

            template <typename iterator >
            struct stencil_named_op : qi::grammar< iterator, D3D12_STENCIL_OP(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_STENCIL_OP(), ascii::space_type  >;

            public:
                stencil_named_op(const std::string& name ) : base(m_start, "stencil_named_op")
                {
                    qi::lit_type    lit;
                    m_start = lit(name) > '=' > m_value;
                }

                stencil_operation                                                m_value;
                qi::rule<iterator, D3D12_STENCIL_OP(), ascii::space_type >       m_start;
            };

            template <typename iterator >
            struct stencil_fail_op : stencil_named_op<iterator>
            {
                using base = stencil_named_op<iterator>;
                public:
                stencil_fail_op() : base(".StencilFailOp")
                {
                }
            };

            template <typename iterator >
            struct stencil_depth_fail_op : stencil_named_op<iterator>
            {
                using base = stencil_named_op<iterator>;
            public:
                stencil_depth_fail_op() : base(".StencilDepthFailOp")
                {
                }
            };

            template <typename iterator >
            struct stencil_pass_op : stencil_named_op<iterator>
            {
                using base = stencil_named_op<iterator>;
            public:
                stencil_pass_op() : base(".StencilPassOp")
                {
                }
            };


            template <typename iterator >
            struct stencil_op_desc : qi::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type  >;

            public:
                stencil_op_desc() : base(m_start, "stencil_op_desc")
                {
                    qi::lit_type    lit;
                    m_start = m_fail > m_depth_fail > m_pass > m_function;
                }

                stencil_pass_op<iterator>                                                  m_pass;
                stencil_fail_op<iterator>                                                  m_fail;
                stencil_depth_fail_op<iterator>                                            m_depth_fail;
                stencil_function<iterator>                                                 m_function;
                qi::rule<iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type >       m_start;
            };


            template <typename iterator, typename error_handler  >
            struct front_face : qi::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type  >;

            public:

                front_face(error_handler& ) : base(m_start, "front_face")
                {
                    qi::lit_type    lit;
                    m_start = lit(".FrontFace") > '=' > '{' > m_op > '}';


                    {
                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        //qi::_1_type     _1;
                        //qi::_3_type     _3;
                        //qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        //on_error<fail>(m_start, error_handler_function(e)(_3));
                        //on_success(m_start, annotation_function(e.m_iters)(_val, _1));
                    }
                }

                stencil_op_desc < iterator> m_op;
                qi::rule<iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type >       m_start;
            };

            template <typename iterator, typename error_handler >
            struct back_face : qi::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type  >;

            public:

                back_face(error_handler& ) : base(m_start, "back_face")
                {
                    qi::lit_type    lit;
                    m_start = lit(".BackFace") > '=' > '{' > m_op > '}';

                    {
                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        //qi::_1_type     _1;
                        //qi::_3_type     _3;
                        //qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        //on_error<fail>(m_start, error_handler_function(e)(_3));
                        //on_success(m_start, annotation_function(e.m_iters)(_val, _1));
                    }
                }

                stencil_op_desc < iterator> m_op;
                qi::rule<iterator, D3D12_DEPTH_STENCILOP_DESC(), ascii::space_type >       m_start;
            };

            
        }
    }
}


