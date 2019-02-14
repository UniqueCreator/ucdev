#pragma once

#include <parser/uc_pso_parser_includes.h>

#include "uc_pso_ast.h"
#include "uc_pso_parser_named_value.h"
#include "uc_pso_parser_so_declaration_entry.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator, typename error_handler>
            struct so_desc : public qi::grammar< iterator, ast::so_desc(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::so_desc(), ascii::space_type >;

                so_desc(error_handler& e) : base(m_start, "so_desc")
                    , m_rasterized_stream(".RasterizedStream")
                    , m_element(e)
                {
                    using qi::lit;
                    using qi::uint_;

                    //qi::raw_type raw;
                    //qi::lexeme_type lexeme;
                    //qi::alpha_type alpha;
                    //qi::alnum_type alnum;

                    m_buffer_strides = lit(".BufferStrides") > '=' > '{' > +uint_  > '}';

                    m_elements = lit(".SODeclaration") > '=' > '{' > +m_element > '}';

                    m_identifier = raw[lexeme[(alpha | '_') > *(alnum | '_')]];

                    m_start %= lit("StreamOutDescription")
                    > m_identifier
                    > '=' > '{'
                    > m_elements
                    > m_buffer_strides
                    > m_rasterized_stream
                    > '}';

                    m_buffer_strides.name("buffer_strides");
                    m_elements.name("elements");
                    m_identifier.name("identifier");
                    m_start.name("so_desc");
                }

                named_value_uint32<iterator>                                                    m_rasterized_stream;
                so_declaration_entry<iterator, error_handler>                                   m_element;
                qi::rule<iterator, std::vector<ast::so_declaration_entry>, ascii::space_type  > m_elements;
                qi::rule<iterator, std::vector<uint32_t>, ascii::space_type  >                  m_buffer_strides;
                qi::rule<iterator, std::string(), ascii::space_type  >                          m_identifier;
                qi::rule<iterator, ast::so_desc(), ascii::space_type >                          m_start;
            };
        }
    }
}