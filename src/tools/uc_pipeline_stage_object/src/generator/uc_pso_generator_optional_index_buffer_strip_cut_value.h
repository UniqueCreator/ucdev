#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast.h>
#include <generator/uc_pso_generator_nil.h>
#include <generator/uc_pso_generator_index_buffer_strip_cut_value.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct optional_index_buffer_strip_cut_value : karma::grammar< iterator, ast::optional_strip_cut_value() >
            {

                using base = karma::grammar< iterator, ast::optional_strip_cut_value() >;

            public:

                optional_index_buffer_strip_cut_value() : base(m_value)
                {
                    m_value = (m_cut_value | "D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED");// m_nil);
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                index_buffer_strip_cut_value<iterator>                           m_cut_value;
                nil<iterator>                                                    m_nil;
                karma::rule<iterator, ast::optional_strip_cut_value()  >         m_value;
            };
        }
    }
}


