#pragma once

#include <generator/uc_pso_generator_includes.h>

#include <boost/fusion/include/adapt_struct.hpp>

#include <generator/uc_pso_generator_stencil_op.h>
#include <generator/uc_pso_generator_depth_write_mask.h>
#include <generator/uc_pso_generator_comparison_function.h>

#include "pipeline_state_types.h"

#if defined(UC_BACKEND_DEV)
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_DEPTH_STENCIL_DESC,
    DepthEnable,
    DepthWriteMask,
    DepthFunc,
    StencilEnable,
    StencilReadMask,
    StencilWriteMask,
    FrontFace,
    BackFace
)
#else
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_DEPTH_STENCIL_DESC,
    m_depthEnable,
    m_depthWriteMask,
    m_depthFunc,
    m_stencilEnable,
    m_stencilReadMask,
    m_stencilWriteMask,
    m_frontFace,
    m_backFace
)
#endif


namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct depth_stencil_desc : karma::grammar< iterator, D3D12_DEPTH_STENCIL_DESC() >
            {
                using base = karma::grammar< iterator, D3D12_DEPTH_STENCIL_DESC() >;

            public:

                depth_stencil_desc() : base(m_start, "depth_stencil_desc")
                {
                    karma::uint_type uint_;

                    m_start = '{'
                        << uint_
                        << ',' << m_depth_write_mask
                        << ',' << m_comparison_func
                        << ',' << uint_
                        << ',' << uint_
                        << ',' << uint_
                        << ',' << m_stencil_op_desc
                        << ',' << m_stencil_op_desc
                        << '}'
                        ;
                }

                depth_write_mask<iterator>                          m_depth_write_mask;
                comparison_function<iterator>                       m_comparison_func;
                stencil_op_desc<iterator>                           m_stencil_op_desc;
                karma::rule<iterator, D3D12_DEPTH_STENCIL_DESC()>   m_start;
            };
        }
    }
}


