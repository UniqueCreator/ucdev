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
            struct logic_op : karma::grammar< iterator, D3D12_LOGIC_OP() >
            {
                using base = karma::grammar< iterator, D3D12_LOGIC_OP() >;

            public:
                logic_op() : base(m_start, "logic_op")
                {
                    m_value.add
                        (D3D12_LOGIC_OP_CLEAR,          STRINGIFY_TYPE(D3D12_LOGIC_OP_CLEAR))
                        (D3D12_LOGIC_OP_SET,            STRINGIFY_TYPE(D3D12_LOGIC_OP_SET))
                        (D3D12_LOGIC_OP_COPY,           STRINGIFY_TYPE(D3D12_LOGIC_OP_COPY))

                        (D3D12_LOGIC_OP_COPY_INVERTED,  STRINGIFY_TYPE(D3D12_LOGIC_OP_COPY_INVERTED))
                        (D3D12_LOGIC_OP_NOOP,           STRINGIFY_TYPE(D3D12_LOGIC_OP_NOOP))
                        (D3D12_LOGIC_OP_INVERT,         STRINGIFY_TYPE(D3D12_LOGIC_OP_INVERT))

                        (D3D12_LOGIC_OP_AND,            STRINGIFY_TYPE(D3D12_LOGIC_OP_AND))
                        (D3D12_LOGIC_OP_NAND,           STRINGIFY_TYPE(D3D12_LOGIC_OP_NAND))
                        (D3D12_LOGIC_OP_OR,             STRINGIFY_TYPE(D3D12_LOGIC_OP_OR))

                        (D3D12_LOGIC_OP_NOR,            STRINGIFY_TYPE(D3D12_LOGIC_OP_NOR))
                        (D3D12_LOGIC_OP_XOR,            STRINGIFY_TYPE(D3D12_LOGIC_OP_XOR))
                        (D3D12_LOGIC_OP_EQUIV,          STRINGIFY_TYPE(D3D12_LOGIC_OP_EQUIV))

                        (D3D12_LOGIC_OP_AND_REVERSE,    STRINGIFY_TYPE(D3D12_LOGIC_OP_AND_REVERSE))
                        (D3D12_LOGIC_OP_AND_INVERTED,   STRINGIFY_TYPE(D3D12_LOGIC_OP_AND_INVERTED))
                        (D3D12_LOGIC_OP_OR_REVERSE,     STRINGIFY_TYPE(D3D12_LOGIC_OP_OR_REVERSE))
                        (D3D12_LOGIC_OP_OR_INVERTED,    STRINGIFY_TYPE(D3D12_LOGIC_OP_OR_INVERTED))


                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_LOGIC_OP, const char* >                      m_value;
                karma::rule<iterator, D3D12_LOGIC_OP()>                           m_start;
            };
        }
    }
}


