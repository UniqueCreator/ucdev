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
            struct index_buffer_strip_cut_value : karma::grammar< iterator, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE() >
            {
                using base = karma::grammar< iterator, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE() >;

            public:
                index_buffer_strip_cut_value() : base(m_start, "index_buffer_strip_cut_value")
                {
                    m_value.add
                        (D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED, STRINGIFY_TYPE(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED))
                        (D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF, STRINGIFY_TYPE(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF))
                        (D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF, STRINGIFY_TYPE(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE, const char* >                      m_value;
                karma::rule<iterator, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE()>                           m_start;
            };
        }
    }
}


