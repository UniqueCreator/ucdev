#include "pch.h"

#include "uc_pso_parser.h"

#include <string>

#include <boost/spirit/include/qi.hpp>

#include <parser/uc_pso_parser_annotation.h>
#include <parser/uc_pso_compile_unit.h>

#include "uc_pso_exception.h"

namespace uc
{
    namespace pso
    {
        graphics_parsed_unit parse_graphics_pso( std::string& content, const std::string& file_name )
        {
            namespace qi = boost::spirit::qi;
            using iterator_type = std::string::const_iterator;

            graphics_parsed_unit r;

            r.m_source = content;
            r.m_file_name = file_name;

            auto&& iter = r.m_source.cbegin();
            auto&& end  = r.m_source.cend();

            using error_handler_type         = parser::error_handler<iterator_type, parser::file_parser_handler >;
            error_handler_type error_handler = make_error_handler(iter, end, parser::file_parser_handler(file_name) ) ;                           // Our error handler

            boost::spirit::qi::ascii::space_type space;
            
            ast::graphics_compile_unit mode;
            parser::compile_graphics_unit<iterator_type, error_handler_type> id2(error_handler);
           
            bool p = qi::phrase_parse( iter, end, id2, space, mode);

            r.m_ast         = std::move(mode);
            r.m_annotations = std::move(error_handler.m_iters);

            if (p && iter == end)
            {
                return r;
            }
            else
            {
                raise_error(error_category::io);
                return r;
            }
        }

        compute_parsed_unit parse_compute_pso(std::string& content, const std::string& file_name)
        {
            namespace qi = boost::spirit::qi;
            using iterator_type = std::string::const_iterator;

            compute_parsed_unit r;

            r.m_source = content;
            r.m_file_name = file_name;

            auto&& iter = r.m_source.cbegin();
            auto&& end  = r.m_source.cend();

            using error_handler_type = parser::error_handler<iterator_type, parser::file_parser_handler >;
            error_handler_type error_handler = make_error_handler(iter, end, parser::file_parser_handler(file_name));                           // Our error handler

            boost::spirit::qi::ascii::space_type space;

            ast::compute_compile_unit mode;
            parser::compile_compute_unit<iterator_type, error_handler_type> id2(error_handler);

            bool p = qi::phrase_parse(iter, end, id2, space, mode);

            r.m_ast = std::move(mode);
            r.m_annotations = std::move(error_handler.m_iters);

            if (p && iter == end)
            {
                return r;
            }
            else
            {
                raise_error(error_category::io);
                return r;
            }
        }
    }
}






