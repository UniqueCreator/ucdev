#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            struct logic_op_ : qi::symbols<char, D3D12_LOGIC_OP>
            {
                logic_op_()
                {
                    this->add
                    ("Clear", D3D12_LOGIC_OP_CLEAR)
                    ("Set", D3D12_LOGIC_OP_SET)
                    ("Copy", D3D12_LOGIC_OP_COPY)
                    ("CopyInverted", D3D12_LOGIC_OP_COPY_INVERTED)
                    ("Noop", D3D12_LOGIC_OP_NOOP)
                    ("Invert", D3D12_LOGIC_OP_INVERT)
                    ("And", D3D12_LOGIC_OP_AND)
                    ("Nand", D3D12_LOGIC_OP_NAND)
                    ("Or", D3D12_LOGIC_OP_OR)
                    ("Nor", D3D12_LOGIC_OP_NOR)
                    ("Xor", D3D12_LOGIC_OP_XOR)
                    ("Equiv", D3D12_LOGIC_OP_EQUIV)
                    ("Revers", D3D12_LOGIC_OP_AND_REVERSE)
                    ("Inverted", D3D12_LOGIC_OP_AND_INVERTED)
                    ("Reverse", D3D12_LOGIC_OP_OR_REVERSE)
                    ("Inverted", D3D12_LOGIC_OP_OR_INVERTED)
                        ;
                    this->name("logic_op_");
                }
            };
        }
    }
}
