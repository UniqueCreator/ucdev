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
            struct blend : karma::grammar< iterator, D3D12_BLEND() >
            {
                using base = karma::grammar< iterator, D3D12_BLEND() >;

            public:
                blend() : base(m_start, "blend")
                {
                    m_value.add
                        (D3D12_BLEND_ZERO, STRINGIFY_TYPE(D3D12_BLEND_ZERO))
                        (D3D12_BLEND_ONE, STRINGIFY_TYPE(D3D12_BLEND_ONE))
                        (D3D12_BLEND_SRC_COLOR, STRINGIFY_TYPE(D3D12_BLEND_SRC_COLOR))
                        (D3D12_BLEND_INV_SRC_COLOR, STRINGIFY_TYPE(D3D12_BLEND_INV_SRC_COLOR))
                        (D3D12_BLEND_SRC_ALPHA, STRINGIFY_TYPE(D3D12_BLEND_SRC_ALPHA))
                        (D3D12_BLEND_INV_SRC_ALPHA, STRINGIFY_TYPE(D3D12_BLEND_INV_SRC_ALPHA))
                        (D3D12_BLEND_DEST_ALPHA, STRINGIFY_TYPE(D3D12_BLEND_DEST_ALPHA))
                        (D3D12_BLEND_INV_DEST_ALPHA, STRINGIFY_TYPE(D3D12_BLEND_INV_DEST_ALPHA))
                        (D3D12_BLEND_DEST_COLOR, STRINGIFY_TYPE(D3D12_BLEND_DEST_COLOR))
                        (D3D12_BLEND_INV_DEST_COLOR, STRINGIFY_TYPE(D3D12_BLEND_INV_DEST_COLOR))
                        (D3D12_BLEND_SRC_ALPHA_SAT, STRINGIFY_TYPE(D3D12_BLEND_SRC_ALPHA_SAT))
                        (D3D12_BLEND_BLEND_FACTOR, STRINGIFY_TYPE(D3D12_BLEND_BLEND_FACTOR))
                        (D3D12_BLEND_INV_BLEND_FACTOR, STRINGIFY_TYPE(D3D12_BLEND_INV_BLEND_FACTOR))
                        (D3D12_BLEND_SRC1_COLOR, STRINGIFY_TYPE(D3D12_BLEND_SRC1_COLOR))
                        (D3D12_BLEND_INV_SRC1_COLOR, STRINGIFY_TYPE(D3D12_BLEND_INV_SRC1_COLOR))
                        (D3D12_BLEND_SRC1_ALPHA, STRINGIFY_TYPE(D3D12_BLEND_SRC1_ALPHA))
                        (D3D12_BLEND_INV_SRC1_ALPHA, STRINGIFY_TYPE(D3D12_BLEND_INV_SRC1_ALPHA))

                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_BLEND, const char* >                      m_value;
                karma::rule<iterator, D3D12_BLEND()>                           m_start;
            };
        }
    }
}


