#pragma once

#include <generator/uc_pso_generator_includes.h>

#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator>
            struct nil : public karma::grammar< iterator, ast::nil() >
            {
                using base = karma::grammar< iterator, ast::nil() >;

                nil() : base(m_value)
                {
                    
                }

                karma::rule<iterator, ast::nil()  > m_value;
            };
        }
    }
}


