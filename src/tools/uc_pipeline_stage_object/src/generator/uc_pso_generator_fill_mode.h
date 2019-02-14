#pragma once

#include <generator/uc_pso_generator_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct fill_mode : karma::grammar< iterator, D3D12_FILL_MODE() >
            {
                using base = karma::grammar< iterator, D3D12_FILL_MODE() >;

            public:
                fill_mode() : base(m_start, "fill_mode")
                {
                    m_value.add
                        (D3D12_FILL_MODE_WIREFRAME, STRINGIFY_TYPE(D3D12_FILL_MODE_WIREFRAME))
                        (D3D12_FILL_MODE_SOLID, STRINGIFY_TYPE(D3D12_FILL_MODE_SOLID))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_FILL_MODE, const char* >                      m_value;
                karma::rule<iterator, D3D12_FILL_MODE()>                           m_start;
            };
        }
    }
}


