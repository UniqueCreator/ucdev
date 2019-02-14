#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast.h>
#include <generator/uc_pso_generator_nil.h>


namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct optional_uint32 : karma::grammar< iterator, ast::optional_uint32() >
            {
                using base = karma::grammar< iterator, ast::optional_uint32() >;

            public:

                optional_uint32() : base(m_value)
                {
                    karma::uint_type uint_;
                    m_value = ( uint_ | "0" );
                }

                nil<iterator>                                                    m_nil;
                karma::rule<iterator, ast::optional_uint32()  >                  m_value;
            };

            template <typename iterator >
            struct optional_string : karma::grammar< iterator, ast::optional_string() >
            {
                using base = karma::grammar< iterator, ast::optional_string() >;

            public:

                optional_string() : base(m_value)
                {
                    m_quoted_string = '"' << karma::string << '"';
                    m_value = ( m_quoted_string | "\"\"" );
                }

                nil<iterator>                                                    m_nil;
                karma::rule<iterator, ast::optional_string()  >                  m_value;
                karma::rule<iterator, std::string()>                             m_quoted_string;
            };
            
            template <typename iterator>
            using optional_base = optional_string<iterator>;

            template <typename iterator>
            using optional_pipeline = optional_string<iterator>;
            
            template <typename iterator>
            using optional_stream_output = optional_string<iterator>;

            template <typename iterator>
            using optional_blend_state = optional_string<iterator>;

            template <typename iterator>
            using optional_rasterizer_desc = optional_string<iterator>;
            
            template <typename iterator>
            using optional_depth_stencil_state = optional_string<iterator>;

            template <typename iterator>
            using optional_input_layout = optional_string<iterator>;
        }
    }
}


