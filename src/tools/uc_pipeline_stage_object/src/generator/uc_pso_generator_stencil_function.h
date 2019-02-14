#pragma once

#include <generator/uc_pso_generator_comparison_function.h>

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct stencil_function : comparison_function<iterator>
            {

            };
        }
    }
}


