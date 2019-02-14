#pragma once
#include <parser/uc_pso_parser_includes.h>
#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_input_layout_element.h"


namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator, typename error_handler>
            struct input_layout : public qi::grammar< iterator, ast::input_layout(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::input_layout(), ascii::space_type >;

                input_layout(error_handler& e) : base(m_start, "input_layout")
                , m_elements(e)
                {
                    using qi::lit;
                    //qi::raw_type raw;
                    //qi::lexeme_type lexeme;
                    ///qi::alpha_type alpha;
                    //qi::alnum_type alnum;

                    m_identifier = raw[lexeme[(alpha | '_') > *(alnum | '_')]];
                    m_start = lit("InputLayout") > m_identifier > '=' > '{' > +m_elements > '}';
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

                        auto success = annotation_function(annotation<iterator>(e.m_iters, e.m_first));
                        //on_error<fail>(m_start, error_handler_function(e)(_3));
                        on_success(m_start, success(_val, _1));
                    }
                }

                input_layout_element<iterator, error_handler>                m_elements;
                qi::rule<iterator, std::string(), ascii::space_type >        m_identifier;
                qi::rule<iterator, ast::input_layout(), ascii::space_type >  m_start;
            };
        }
    }
}