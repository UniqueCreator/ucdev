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
            struct pipeline_state_flags : karma::grammar< iterator, D3D12_PIPELINE_STATE_FLAGS() >
            {
                using base = karma::grammar< iterator, D3D12_PIPELINE_STATE_FLAGS() >;

            public:
                pipeline_state_flags() : base(m_start, "pipeline_state_flags")
                {
                    m_value.add
                        (D3D12_PIPELINE_STATE_FLAG_NONE, STRINGIFY_TYPE(D3D12_PIPELINE_STATE_FLAG_NONE))
                        (D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG, STRINGIFY_TYPE(D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_PIPELINE_STATE_FLAGS, const char* >                      m_value;
                karma::rule<iterator, D3D12_PIPELINE_STATE_FLAGS()>                           m_start;
            };
        }
    }
}


