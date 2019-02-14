#pragma once

#include <generator/uc_pso_generator_includes.h>
#include <uc_pso_ast_adapted.h>

#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct dxgi_sample_desc : karma::grammar< iterator, DXGI_SAMPLE_DESC() >
            {
                using base = karma::grammar< iterator, DXGI_SAMPLE_DESC() >;

            public:
                dxgi_sample_desc() : base(m_start)
                {
                    karma::uint_type uint_;
                    m_start = "{" << uint_ << "," << uint_ << "}";
                }

                karma::rule<iterator, DXGI_SAMPLE_DESC()>                         m_start;
            };
        }
    }
}


