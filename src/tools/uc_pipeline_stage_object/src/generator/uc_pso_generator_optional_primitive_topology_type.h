#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast.h>
#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_nil.h>
#include <generator/uc_pso_generator_primitive_topology_type.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator>
            struct optional_primitive_topology_type : public karma::grammar< iterator, ast::optional_primitive_topology_type() >
            {
                using base = karma::grammar< iterator, ast::optional_primitive_topology_type() >;

                optional_primitive_topology_type() : base(m_value)
                {
                    karma::lit_type lit;
                    m_value = ( m_cut_value | "D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED" );
                    
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                primitive_topology_type<iterator>                                m_cut_value;
                nil<iterator>                                                    m_nil;
                karma::rule<iterator, ast::optional_primitive_topology_type()  > m_value;
            };
        }
    }
}


