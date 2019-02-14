#pragma once

#include <string>
#include <cstdint>

namespace uc
{
    namespace pso
    {
        enum class error_category : uint32_t
        {
            general = 1,
            io = 2,
            phase_1 = 3,
            phase_2 = 4,
            input_parameters = 5
        };

        inline std::string format_message( error_category )
        {
            return std::string("pso error");
        }
    }
}
