#pragma once

#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"

#include "uc_pso_parser_so_desc.h"
#include "uc_pso_parser_rasterizer_desc.h"
#include "uc_pso_parser_pipeline_state_object.h"
#include "uc_pso_parser_depth_stencil_desc.h"
#include "uc_pso_parser_blend_desc.h"
#include "uc_pso_parser_input_layout.h"
#include "uc_pso_parser_imports.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {

            template <typename iterator, typename error_handler>
            struct compile_graphics_unit : public qi::grammar< iterator, ast::graphics_compile_unit(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::graphics_compile_unit(), ascii::space_type >;

                compile_graphics_unit(error_handler& e) : base(m_start, "compile_unit")
                ,m_pso(e)
                ,m_so_desc(e)
                ,m_depth_stencil_desc(e)
                ,m_rasterizer(e)
                ,m_blend_desc(e)
                ,m_input_layout(e)
                ,m_imports(e)
                {
                    using qi::attr;
                    //m_start = *(m_imports) >> *( m_pso );
                    m_start = *(m_imports) > +(m_pso | m_so_desc | m_depth_stencil_desc | m_rasterizer | m_blend_desc | m_input_layout);
                    {

                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        //qi::_1_type     _1;
                        qi::_3_type     _3;
                        //qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        on_error<fail>(m_start, error_handler_function(e)(_3));
                        //on_success(m_start, annotation_function(e.m_iters)(_val, _1));
                    }

                    BOOST_SPIRIT_DEBUG_NODE(m_start);
                }

                graphics_pipeline_state_object<iterator, error_handler>      m_pso;
                so_desc<iterator, error_handler>                             m_so_desc;
                depth_stencil_desc<iterator, error_handler>                  m_depth_stencil_desc;
                rasterizer_desc<iterator, error_handler>                     m_rasterizer;
                blend_desc<iterator, error_handler>                          m_blend_desc;
                input_layout<iterator, error_handler>                        m_input_layout;
                import<iterator, error_handler>                              m_imports;

                qi::rule<iterator, ast::graphics_compile_unit(), ascii::space_type >  m_start;
            };

            template <typename iterator, typename error_handler>
            struct compile_compute_unit : public qi::grammar< iterator, ast::compute_compile_unit(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::compute_compile_unit(), ascii::space_type >;

                compile_compute_unit(error_handler& e) : base(m_start, "compile_unit")
                    , m_pso(e)
                    , m_imports(e)
                {
                    using qi::attr;
                    //m_start = *(m_imports) >> *( m_pso );
                    m_start = *(m_imports) > +(m_pso);
                    {

                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        qi::_3_type     _3;
                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        on_error<fail>(m_start, error_handler_function(e)(_3));
                    }

                    BOOST_SPIRIT_DEBUG_NODE(m_start);
                }

                compute_pipeline_state_object<iterator, error_handler>                  m_pso;
                import<iterator, error_handler>                                         m_imports;
                qi::rule<iterator, ast::compute_compile_unit(), ascii::space_type >     m_start;
            };
        }
    }
}