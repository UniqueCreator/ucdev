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
            template <typename iterator, typename error_handler>
            struct so_declaration_entry : public qi::grammar< iterator, ast::so_declaration_entry(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::so_declaration_entry(), ascii::space_type >;

                so_declaration_entry(error_handler& e) : base(m_start, "so_declaration_entry")
                    , m_stream(".Stream")
                    , m_semantic_name(".SemanticName")
                    , m_semantic_index(".SemanticIndex")
                    , m_start_component(".StartComponent")
                    , m_component_count(".ComponentCount")
                    , m_output_slot(".OutputSlot")
                {
                    using qi::lit;
                    using qi::lexeme;
                    using qi::char_;
                    using qi::int_;

                    m_start = lit("SODeclarationEntry") > '=' > '{'
                    > m_stream
                    > m_semantic_name
                    > m_semantic_index
                    > m_start_component
                    > m_component_count
                    > m_output_slot
                    > '}';

                    m_start.name("so_declaration_entry");

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

                named_value_uint32<iterator> m_stream;
                named_value_string<iterator> m_semantic_name;
                named_value_uint32<iterator> m_semantic_index;
                named_value_uint32<iterator> m_start_component;
                named_value_uint32<iterator> m_component_count;
                named_value_uint32<iterator> m_output_slot;

                qi::rule<iterator, ast::so_declaration_entry(), ascii::space_type >  m_start;
            };
        }
    }
}