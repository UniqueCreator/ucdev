#pragma once

#include <generator/uc_pso_generator_includes.h>

#include "pipeline_state_types.h"

#include <generator/uc_pso_generator_stencil_function.h>
#include <uc_pso_ast_adapted.h>



namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct stencil_op : karma::grammar< iterator, D3D12_STENCIL_OP() >
            {
                using base = karma::grammar< iterator, D3D12_STENCIL_OP() >;

            public:
                stencil_op() : base(m_start, "stencil_op")
                {
                    m_value.add
                        (D3D12_STENCIL_OP_KEEP,          STRINGIFY_TYPE(D3D12_STENCIL_OP_KEEP))
                        (D3D12_STENCIL_OP_ZERO,          STRINGIFY_TYPE(D3D12_STENCIL_OP_ZERO))
                        (D3D12_STENCIL_OP_REPLACE,       STRINGIFY_TYPE(D3D12_STENCIL_OP_REPLACE))

                        (D3D12_STENCIL_OP_INCR_SAT,     STRINGIFY_TYPE(D3D12_STENCIL_OP_INCR_SAT))
                        (D3D12_STENCIL_OP_DECR_SAT,     STRINGIFY_TYPE(D3D12_STENCIL_OP_DECR_SAT))
                        (D3D12_STENCIL_OP_INVERT,       STRINGIFY_TYPE(D3D12_STENCIL_OP_INVERT))

                        (D3D12_STENCIL_OP_INCR,         STRINGIFY_TYPE(D3D12_STENCIL_OP_INCR))
                        (D3D12_STENCIL_OP_DECR,         STRINGIFY_TYPE(D3D12_STENCIL_OP_DECR))

                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_STENCIL_OP, const char* >                      m_value;
                karma::rule<iterator, D3D12_STENCIL_OP()>                           m_start;
            };

            template <typename iterator >
            struct stencil_op_desc : karma::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC()>
            {
                using base = karma::grammar< iterator, D3D12_DEPTH_STENCILOP_DESC()>;

            public:
                stencil_op_desc() : base(m_start, "stencil_op_desc")
                {
                    m_start = '{'
                    << m_pass
                    << ',' << m_fail
                    << ',' << m_depth_fail
                    << ',' << m_function
                    << '}'
                    ;
                }

                stencil_op<iterator>                                                  m_pass;
                stencil_op<iterator>                                                  m_fail;
                stencil_op<iterator>                                                  m_depth_fail;
                stencil_function<iterator>                                            m_function;
                karma::rule<iterator, D3D12_DEPTH_STENCILOP_DESC()>                   m_start;
            };
        }
    }
}


