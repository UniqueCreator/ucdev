#pragma once

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_includes.h>
#include <uc_pso_ast_adapted.h>

#include <generator/uc_pso_generator_so_declaration_entry.h>


BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::so_desc_proxy,
    m_entries,
    m_entries_count,
    m_strides,
    m_strides_count,
    m_rasterized_stream
)

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator>
            struct so_desc_strides_vector : karma::grammar< iterator, std::vector< uint32_t >() >
            {
                using base = karma::grammar< iterator, std::vector< uint32_t >() >;

            public:
                so_desc_strides_vector() : base(m_start, "so_desc_strides_vector")
                {
                    karma::uint_type uint_;
                    m_start = '{'

                        << -(uint_ % ',')

                        << '}'
                        ;
                }

                karma::rule<iterator, std::vector< uint32_t>() >  m_start;
            };


            template <typename iterator >
            struct so_desc : karma::grammar< iterator, ast::so_desc_proxy() >
            {
                using base = karma::grammar< iterator, ast::so_desc_proxy() >;

            public:
                so_desc() : base(m_start, "so_desc")
                {
                    karma::uint_type uint_;
                    karma::float_type float_;
                    karma::skip_type skip_;
                    karma::string_type string_;

                    m_start = "{{"
                        << m_elements
                        << "},"
                        << uint_
                        << ",{" << m_strides
                        << "},"
                        << uint_
                        << ',' << uint_
                        << '}'
                        ;
                }

                karma::rule<iterator, ast::so_desc_proxy()>        m_start;
                so_declaration_entry_element_vector< iterator >    m_elements;
                so_desc_strides_vector<iterator>                   m_strides;
            };
        }
    }
}


