#pragma once

#include <string>

#include <parser/uc_pso_parser_includes.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_error_handler.h"
#include "uc_pso_parser_annotation.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator, typename error_handler > struct import : public qi::grammar < iterator, ast::identifier(), ascii::space_type  >
            {
                using base = qi::grammar < iterator, ast::identifier(), ascii::space_type >;

                import( error_handler& e )  : base (m_start)
                {
                    //qi::char_type   char_;
                    //qi::uint_type   uint_;
                    //qi::raw_type    raw;
                    //qi::lexeme_type lexeme;
                    //qi::alpha_type  alpha;
                    //qi::digit_type  digit;
                    qi::lit_type    lit;
       
                    m_identifier = lexeme[(alpha | '_') > *( alpha | digit | '_')];
                    m_start = lit("import") > m_identifier;

                    {

                        using boost::phoenix::function;
                        using error_handler_function    = function < error_handler >;
                        using annotation_function       = function < annotation<iterator> >;

                        auto success = annotation_function( annotation<iterator>(e.m_iters, e.m_first) );

                        qi::_1_type     _1;
                        //qi::_3_type     _3;
                        qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        //on_error<fail>(m_start, error_handler_function(e)( _3));
                        on_success(m_start, success(_val, _1));
                    }
                }

                qi::rule < iterator, std::string(), ascii::space_type >      m_identifier;
                qi::rule < iterator, ast::identifier(), ascii::space_type >  m_start;
            };

            template <typename iterator, typename error_handler > inline auto make_import(const error_handler& e)
            {
                return identifier<iterator, error_handler>(e);
            }

         }

    }
}






