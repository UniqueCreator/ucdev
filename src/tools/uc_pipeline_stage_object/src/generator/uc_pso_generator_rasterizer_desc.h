#pragma once

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_includes.h>
#include <generator/uc_pso_generator_fill_mode.h>
#include <generator/uc_pso_generator_cull_mode.h>

#include <generator/uc_pso_generator_conservative_rasterization_mode.h>

#include <boost/fusion/include/adapt_struct.hpp>

#if defined(UC_BACKEND_DEV)
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_RASTERIZER_DESC,
    FillMode,
    CullMode,
    FrontCounterClockwise,
    DepthBias,
    DepthBiasClamp,
    SlopeScaledDepthBias,
    DepthClipEnable,
    MultisampleEnable,
    AntialiasedLineEnable,
    ForcedSampleCount,
    ConservativeRaster
)
#else
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_RASTERIZER_DESC,
    m_fillMode,
    m_cullMode,
    m_frontCounterClockwise,
    m_depthBias,
    m_depthBiasClamp,
    m_slopeScaledDepthBias,
    m_depthClipEnable,
    m_multisampleEnable,
    m_antialiasedLineEnable,
    m_forcedSampleCount,
    m_conservativeRaster
)
#endif

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct rasterizer_desc : karma::grammar< iterator, D3D12_RASTERIZER_DESC() >
            {
                using base = karma::grammar< iterator, D3D12_RASTERIZER_DESC() >;

            public:
                rasterizer_desc() : base(m_start, "rasterizer_desc")
                {
                    karma::uint_type uint_;
                    karma::float_type float_;

                    m_start = '{'
                        << m_fill_mode
                        << ',' << m_cull_mode
                        << ',' << uint_
                        << ',' << uint_

                        << ',' << float_
                        << ',' << float_

                        << ',' << uint_
                        << ',' << uint_
                        << ',' << uint_
                        << ',' << uint_

                        << ',' << m_rasterization_mode

                        << '}'
                        ;
                }

                fill_mode<iterator>                              m_fill_mode;
                cull_mode<iterator>                              m_cull_mode;
                conservative_rasterization_mode<iterator>        m_rasterization_mode;
                karma::rule<iterator, D3D12_RASTERIZER_DESC()>   m_start;
            };
        }
    }
}


