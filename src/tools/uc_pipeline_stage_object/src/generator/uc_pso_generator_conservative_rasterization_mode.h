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
            struct conservative_rasterization_mode : karma::grammar< iterator, D3D12_CONSERVATIVE_RASTERIZATION_MODE() >
            {
                using base = karma::grammar< iterator, D3D12_CONSERVATIVE_RASTERIZATION_MODE() >;

            public:
                conservative_rasterization_mode() : base(m_start, "cull_mode")
                {
                    m_value.add
                        (D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF, STRINGIFY_TYPE(D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF))
                        (D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON, STRINGIFY_TYPE(D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_CONSERVATIVE_RASTERIZATION_MODE, const char* > m_value;
                karma::rule<iterator, D3D12_CONSERVATIVE_RASTERIZATION_MODE()>      m_start;
            };
        }
    }
}


