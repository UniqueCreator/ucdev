#pragma once
#include <parser/uc_pso_parser_includes.h>
#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"

#include "uc_pso_parser_named_value.h"
#include "uc_pso_parser_blend_ops.h"
#include "uc_pso_parser_render_target_write_mask.h"


namespace uc
{
    namespace pso
    {
        namespace parser
        {
            using qi::uint_;
            using qi::char_;
            using qi::raw;
            using qi::lexeme;
            using namespace qi::ascii;

   
            template <typename iterator, typename error_handler>
            struct render_target_blend_desc : public qi::grammar< iterator, ast::render_target_blend_desc(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::render_target_blend_desc(), ascii::space_type  >;

                render_target_blend_desc(error_handler& e) : base(m_start, "render_target_blend_desc")
                    ,m_blend_enable(".BlendEnable")
                    ,m_logic_op_enable(".LogicOpEnable")
                {
                    using qi::lit;
                    using qi::lexeme;
                    using qi::char_;
                    using qi::int_;

                    m_start = lit("RenderTargetBlendDesc") > '=' 
                        > '{'
                        > m_blend_enable
                        > m_logic_op_enable
                        > m_src_blend
                        > m_dst_blend
                        > m_blend_op
                        > m_src_blend_alpha
                        > m_dst_blend_alpha
                        > m_blend_op_alpha
                        > m_logic_op
                        > m_render_target_write_mask
                        > '}';

                        m_start.name("render_target_blend_desc");

                    {

                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        qi::_1_type     _1;
                        //qi::_3_type     _3;
                        qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        //on_error<fail>(m_start, error_handler_function(e)(_3));
                        auto success = annotation_function(annotation<iterator>(e.m_iters, e.m_first));
                        on_success(m_start, success(_val, _1));
                    }
                }

                named_value_bool<iterator>                                             m_blend_enable;
                named_value_bool<iterator>                                             m_logic_op_enable;
                src_blend<iterator>                                                    m_src_blend;
                dest_blend<iterator>                                                   m_dst_blend;
                blend_op<iterator>                                                     m_blend_op;
                src_blend_alpha<iterator>                                              m_src_blend_alpha;
                dest_blend_alpha<iterator>                                             m_dst_blend_alpha;
                blend_op_alpha<iterator>                                               m_blend_op_alpha;
                logic_op<iterator>                                                     m_logic_op;
                render_target_write_mask<iterator>                                     m_render_target_write_mask;

                qi::rule<iterator, ast::render_target_blend_desc(), ascii::space_type  >  m_start;
            };
        }
    }
}