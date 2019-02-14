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
            struct cull_mode : karma::grammar< iterator, D3D12_CULL_MODE() >
            {
                using base = karma::grammar< iterator, D3D12_CULL_MODE() >;

            public:
                cull_mode() : base(m_start, "cull_mode")
                {
                    m_value.add
                        (D3D12_CULL_MODE_NONE, STRINGIFY_TYPE(D3D12_CULL_MODE_NONE))
                        (D3D12_CULL_MODE_FRONT, STRINGIFY_TYPE(D3D12_CULL_MODE_FRONT))
                        (D3D12_CULL_MODE_BACK, STRINGIFY_TYPE(D3D12_CULL_MODE_BACK))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_CULL_MODE, const char* >                      m_value;
                karma::rule<iterator, D3D12_CULL_MODE()>                           m_start;
            };
        }
    }
}


