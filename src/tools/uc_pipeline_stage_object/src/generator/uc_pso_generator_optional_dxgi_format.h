#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast.h>
#include <generator/uc_pso_generator_nil.h>
#include <generator/uc_pso_generator_dxgi_format.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct optional_dxgi_format : karma::grammar< iterator, ast::optional_dxgi_format() >
            {

                using base = karma::grammar< iterator, ast::optional_dxgi_format() >;

            public:

                optional_dxgi_format() : base(m_value)
                {
                    m_value = (m_cut_value | "DXGI_FORMAT_UNKNOWN");// m_nil);
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                dxgi_format<iterator>                                        m_cut_value;
                nil<iterator>                                                m_nil;
                karma::rule<iterator, ast::optional_dxgi_format()  >         m_value;
            };

            template <typename iterator >
            struct optional_dxgi_vector : karma::grammar< iterator, ast::optional_dxgi_vector() >
            {

                using base = karma::grammar< iterator, ast::optional_dxgi_vector() >;

            public:

                optional_dxgi_vector() : base(m_value)
                {
                    m_value = (m_cut_value | "{}");// m_nil);
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                dxgi_format_vector<iterator>                                 m_cut_value;
                nil<iterator>                                                m_nil;
                karma::rule<iterator, ast::optional_dxgi_vector()  >         m_value;
            };
        }
    }
}


