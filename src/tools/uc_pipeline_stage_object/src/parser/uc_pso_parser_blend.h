#pragma once

#include "pipeline_state_types.h"
#include <parser/uc_pso_parser_includes.h>

namespace uc
{
    namespace pso
    {
        namespace parser
        {


            struct blend : qi::symbols<char, D3D12_BLEND>
            {
                blend()
                {
                    this->add
                        ("Zero", D3D12_BLEND_ZERO)
                        ("One", D3D12_BLEND_ONE)
                        ("SrcColor", D3D12_BLEND_SRC_COLOR)
                        ("InvSrcColor", D3D12_BLEND_INV_SRC_COLOR)
                        ("SrcAlpha", D3D12_BLEND_SRC_ALPHA)
                        ("InvSrcAlpha", D3D12_BLEND_INV_SRC_ALPHA)
                        ("DestAlpha", D3D12_BLEND_DEST_ALPHA)
                        ("InvDestAlpha", D3D12_BLEND_INV_DEST_ALPHA)
                        ("DestColor", D3D12_BLEND_DEST_COLOR)
                        ("InvDestColor", D3D12_BLEND_INV_DEST_COLOR)
                        ("SrcAlphaSat", D3D12_BLEND_SRC_ALPHA_SAT)
                        ("BlendFactor", D3D12_BLEND_BLEND_FACTOR)
                        ("InvBlendFactor", D3D12_BLEND_INV_BLEND_FACTOR)
                        ("Src1Color", D3D12_BLEND_SRC1_COLOR)
                        ("InvSrc1Color", D3D12_BLEND_INV_SRC1_COLOR)
                        ("Src1Alpha", D3D12_BLEND_SRC1_ALPHA)
                        ("InvSrc1Alpha", D3D12_BLEND_INV_SRC1_ALPHA)
                        ;
                    this->name("blend");
                }
            };

            struct blend_op_ : qi::symbols<char, D3D12_BLEND_OP>
            {
                blend_op_()
                {
                    this->add
                        ("Add", D3D12_BLEND_OP_ADD)
                        ("Subtract", D3D12_BLEND_OP_SUBTRACT)
                        ("RevSubtract", D3D12_BLEND_OP_REV_SUBTRACT)
                        ("Min", D3D12_BLEND_OP_MIN)
                        ("Max", D3D12_BLEND_OP_MAX)
                        ;
                    this->name("blend_op");
                }
            };
        }
    }
}
