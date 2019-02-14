#pragma once

#include <parser/uc_pso_parser_includes.h>

#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_depth_write_mask.h"
#include "uc_pso_parser_depth_function.h"
#include "uc_pso_parser_stencil_op.h"
#include "uc_pso_parser_named_value.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {

            template <typename iterator, typename error_handler>
            struct depth_stencil_desc : public qi::grammar< iterator, ast::depth_stencil_desc(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::depth_stencil_desc(), ascii::space_type >;

                depth_stencil_desc(error_handler& e) : base(m_start, "depth_stencil_desc")
                , m_depth_enable(".DepthEnable")
                , m_stencil_enable(".StencilEnable")
                , m_front_face(e)
                , m_back_face(e)
                , m_stencil_read_mask(".StencilReadMask")
                , m_stencil_write_mask(".StencilWriteMask")
                {
                    using qi::lit;
                    //qi::raw_type    raw;
                    //qi::lexeme_type lexeme;
                    //qi::alpha_type  alpha;
                    //qi::alnum_type  alnum;
                    //qi::uint_type   uint_;

                    m_identifier = raw[lexeme[(alpha | '_') > *(alnum | '_')]];
                    m_start = lit("DepthStencilDescription") > m_identifier > '=' > '{'
                    > m_depth_enable
                    > m_depth_write_mask
                    > m_depth_function
                    > m_stencil_enable
                    > m_stencil_read_mask
                    > m_stencil_write_mask
                    > m_front_face
                    > m_back_face
                    > '}';

                    m_identifier.name("identifier");

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

                named_value_bool<iterator>                                         m_depth_enable;
                named_value_bool<iterator>                                         m_stencil_enable;
                depth_write_mask<iterator>                                         m_depth_write_mask;
                depth_function<iterator>                                           m_depth_function;
                front_face<iterator, error_handler>                                m_front_face;
                back_face<iterator, error_handler>                                 m_back_face;
                named_value_uint32<iterator>                                       m_stencil_read_mask;
                named_value_uint32<iterator>                                       m_stencil_write_mask;

                qi::rule<iterator, std::string(), ascii::space_type >              m_identifier;
                qi::rule<iterator, ast::depth_stencil_desc(), ascii::space_type >  m_start;
            };
        }
    }
}