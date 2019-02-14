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
            struct classification : karma::grammar< iterator, D3D12_INPUT_CLASSIFICATION() >
            {
                using base = karma::grammar< iterator, D3D12_INPUT_CLASSIFICATION() >;

            public:
                classification() : base(m_start, "classification")
                {
                    m_value.add
                        (D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, STRINGIFY_TYPE(D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA))
                        (D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, STRINGIFY_TYPE(D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_INPUT_CLASSIFICATION, const char* >                      m_value;
                karma::rule<iterator, D3D12_INPUT_CLASSIFICATION()>                           m_start;
            };
        }
    }
}


