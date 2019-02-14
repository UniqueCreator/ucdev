#pragma once

#include <parser/uc_pso_parser_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            struct comparison_func : qi::symbols<char, D3D12_COMPARISON_FUNC>
            {
                comparison_func()
                {
                    this->add
                    ("Never", D3D12_COMPARISON_FUNC_NEVER)
                        ("Less", D3D12_COMPARISON_FUNC_LESS)
                        ("Equal", D3D12_COMPARISON_FUNC_EQUAL)
                        ("LessEqual", D3D12_COMPARISON_FUNC_LESS_EQUAL)
                        ("Greater", D3D12_COMPARISON_FUNC_GREATER)
                        ("NotEqual", D3D12_COMPARISON_FUNC_NOT_EQUAL)
                        ("GreaterEqual", D3D12_COMPARISON_FUNC_GREATER_EQUAL)
                        ("Always", D3D12_COMPARISON_FUNC_ALWAYS)
                        ;
                    this->name("comparison_func");
                }
            };
        }
    }
}
