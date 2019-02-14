#pragma once

#include "uc_pso_parser_struct.h"

namespace uc
{
    namespace pso
    {
        struct unit_data
        {
            std::vector<std::string> m_includes;
            std::string              m_pipeline_state;
            std::string              m_header;
            std::string              m_cpp;
        };

        unit_data generate_graphics_pso_unit(const graphics_parsed_unit& p, const std::string& main_function, const std::string& header_name);
        unit_data generate_compute_pso_unit(const compute_parsed_unit& p, const std::string& main_function, const std::string& header_name);
    }
}






