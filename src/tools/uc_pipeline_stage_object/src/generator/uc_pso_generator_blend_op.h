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
            struct blend_op : karma::grammar< iterator, D3D12_BLEND_OP() >
            {
                using base = karma::grammar< iterator, D3D12_BLEND_OP() >;

            public:
                blend_op() : base(m_start, "blend_op")
                {
                    m_value.add
                        (D3D12_BLEND_OP_ADD, STRINGIFY_TYPE(D3D12_BLEND_OP_ADD))
                        (D3D12_BLEND_OP_SUBTRACT, STRINGIFY_TYPE(D3D12_BLEND_OP_SUBTRACT))
                        (D3D12_BLEND_OP_REV_SUBTRACT, STRINGIFY_TYPE(D3D12_BLEND_OP_REV_SUBTRACT))
                        (D3D12_BLEND_OP_MIN, STRINGIFY_TYPE(D3D12_BLEND_OP_MIN))
                        (D3D12_BLEND_OP_MAX, STRINGIFY_TYPE(D3D12_BLEND_OP_MAX))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_BLEND_OP, const char* >                   m_value;
                karma::rule<iterator, D3D12_BLEND_OP()>                        m_start;
            };
        }
    }
}


