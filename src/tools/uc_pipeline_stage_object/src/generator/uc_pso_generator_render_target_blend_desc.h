#pragma once

#include <generator/uc_pso_generator_includes.h>
#include "pipeline_state_types.h"

#include <uc_pso_ast_adapted.h>

#include <generator/uc_pso_generator_blend.h>
#include <generator/uc_pso_generator_blend_op.h>
#include <generator/uc_pso_generator_logic_op.h>

#if defined(UC_BACKEND_DEV)
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_RENDER_TARGET_BLEND_DESC,
    BlendEnable,
    LogicOpEnable,
    SrcBlend,
    DestBlend,
    BlendOp,
    SrcBlendAlpha,
    DestBlendAlpha,
    BlendOpAlpha,
    LogicOp,
    RenderTargetWriteMask
)
#else
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_RENDER_TARGET_BLEND_DESC,
    m_blendEnable,
    m_logicOperationEnable,
    m_sourceBlend,
    m_destinationBlend,
    m_blendOperation,
    m_sourceBlendAlpha,
    m_destinationBlendAlpha,
    m_blendOperationAlpha,
    m_logicOperation,
    m_renderTargetWriteMask
)
#endif

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct render_target_blend_desc : karma::grammar< iterator, D3D12_RENDER_TARGET_BLEND_DESC() >
            {
                using base = karma::grammar< iterator, D3D12_RENDER_TARGET_BLEND_DESC() >;

            public:

                render_target_blend_desc() : base(m_start, "render_target_blend_desc")
                {
                    karma::uint_type uint_;

                    m_start = '{'
                        << uint_
                        << ',' << uint_
                        << ',' << m_blend
                        << ',' << m_blend
                        << ',' << m_blend_op
                        << ',' << m_blend
                        << ',' << m_blend
                        << ',' << m_blend_op
                        << ',' << m_logic_op
                        << ',' << uint_
                        << '}'
                        ;
                }

                blend<iterator>                                         m_blend;
                blend_op<iterator>                                      m_blend_op;
                logic_op<iterator>                                      m_logic_op;

                karma::rule<iterator, D3D12_RENDER_TARGET_BLEND_DESC()> m_start;
            };
        }
    }
}


