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
            struct comparison_function : karma::grammar< iterator, D3D12_COMPARISON_FUNC() >
            {
                using base = karma::grammar< iterator, D3D12_COMPARISON_FUNC() >;

            public:
                comparison_function() : base(m_start, "comparison_function")
                {
                    m_value.add
                        (D3D12_COMPARISON_FUNC_NEVER, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_NEVER))
                        (D3D12_COMPARISON_FUNC_LESS, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_LESS))
                        (D3D12_COMPARISON_FUNC_EQUAL, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_EQUAL))
                        (D3D12_COMPARISON_FUNC_LESS_EQUAL, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_LESS_EQUAL))
                        (D3D12_COMPARISON_FUNC_GREATER, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_GREATER))
                        (D3D12_COMPARISON_FUNC_NOT_EQUAL, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_NOT_EQUAL))
                        (D3D12_COMPARISON_FUNC_GREATER_EQUAL, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_GREATER_EQUAL))
                        (D3D12_COMPARISON_FUNC_ALWAYS, STRINGIFY_TYPE(D3D12_COMPARISON_FUNC_ALWAYS))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_COMPARISON_FUNC, const char* >     m_value;
                karma::rule<iterator, D3D12_COMPARISON_FUNC()>          m_start;
            };
        }
    }
}


