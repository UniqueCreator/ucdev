#pragma once

#include "uc_pso_parser_struct.h"

namespace uc
{
    namespace pso
    {
        using row = uint32_t;
        using column = uint32_t;

        struct location
        {
            row     m_row = { 0 };
            column  m_column = { 0 };
        };

        location get_location(const parsed_unit& unit, uint32_t annotation_id);

    }
}






