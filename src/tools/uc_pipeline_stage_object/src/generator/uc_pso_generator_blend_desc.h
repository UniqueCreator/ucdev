#pragma once

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_includes.h>
#include <generator/uc_pso_generator_render_target_blend_desc.h>

#include <boost/fusion/include/adapt_struct.hpp>


#if defined(UC_BACKEND_DEV)
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_BLEND_DESC,
    AlphaToCoverageEnable,
    IndependentBlendEnable,
    RenderTarget[0],
    RenderTarget[1],
    RenderTarget[2],
    RenderTarget[3],
    RenderTarget[4],
    RenderTarget[5],
    RenderTarget[6],
    RenderTarget[7]
)
#else
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_BLEND_DESC,
    m_alphaToCoverageEnable,
    m_independentBlendEnable,
    m_renderTarget[0],
    m_renderTarget[0],
    m_renderTarget[1],
    m_renderTarget[2],
    m_renderTarget[3],
    m_renderTarget[4],
    m_renderTarget[5],
    m_renderTarget[6],
    m_renderTarget[7]
)
#endif


namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct blend_desc : karma::grammar< iterator, D3D12_BLEND_DESC() >
            {
                using base = karma::grammar< iterator, D3D12_BLEND_DESC() >;

            public:
                blend_desc() : base(m_start, "blend_desc")
                {
                    karma::uint_type uint_;

                    m_start = '{'
                        << uint_
                        << ',' << uint_
                        << ',' << '{'
                        << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << ',' << m_render_target_blend_desc
                        << '}'
                        << '}'
                        ;
                }

                render_target_blend_desc<iterator>          m_render_target_blend_desc;
                karma::rule<iterator, D3D12_BLEND_DESC()>   m_start;
            };
        }
    }
}


