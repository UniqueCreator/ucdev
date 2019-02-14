#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <uc_pso_ast.h>
#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_nil.h>
#include <generator/uc_pso_generator_pipeline_state_flags.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator>
            struct optional_pipeline_state_flags : public karma::grammar< iterator, ast::optional_pipeline_state_flags() >
            {
                using base = karma::grammar< iterator, ast::optional_pipeline_state_flags() >;

                optional_pipeline_state_flags() : base(m_value)
                {
                    karma::lit_type lit;
                    m_value = ( m_cut_value | "D3D12_PIPELINE_STATE_FLAG_NONE" );
                    
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                pipeline_state_flags<iterator>                                   m_cut_value;
                nil<iterator>                                                    m_nil;
                karma::rule<iterator, ast::optional_pipeline_state_flags()  >    m_value;
            };
        }
    }
}


