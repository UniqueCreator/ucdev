#pragma once

#include <parser/uc_pso_parser_includes.h>

#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_dxgi_format.h"
#include "uc_pso_parser_classification.h"
#include "uc_pso_parser_named_value.h"


namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator, typename error_handler>
            struct input_layout_element : public qi::grammar< iterator, ast::input_layout_element(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::input_layout_element(), ascii::space_type  >;

                input_layout_element(error_handler& e) : base(m_start, "input_layout_element"),
                    m_semantic(std::string(".SemanticName"))
                    , m_semantic_index(".SemanticIndex")
                    , m_input_slot(".InputSlot")
                    , m_aligned_byte_offset(".AlignedByteOffset")
                {
                    using qi::lit;
                    using qi::lexeme;
                    using qi::char_;
                    using qi::int_;

                    m_start = lit("InputLayoutElement") > '=' > '{' > m_semantic
                        > m_semantic_index
                        > m_dxgi_format
                        > m_input_slot
                        > m_aligned_byte_offset
                        > m_classification 
                        > '}';

                    m_start.name("input_layout_element");

                    //todo: m_instance_data_step_rate

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

                named_value_string<iterator>                            m_semantic;
                named_value_uint32<iterator>                            m_semantic_index;
                named_value_uint32<iterator>                            m_input_slot;
                named_value_uint32<iterator>                            m_aligned_byte_offset;
                dxgi_format<iterator>                                   m_dxgi_format;
                classification<iterator>                                m_classification;
                qi::rule<iterator, ast::input_layout_element(), ascii::space_type  >  m_start;
            };
        }
    }
}