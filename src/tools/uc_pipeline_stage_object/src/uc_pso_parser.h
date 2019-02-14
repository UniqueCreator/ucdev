#pragma once

#include "uc_pso_parser_struct.h"

namespace uc
{
    namespace pso
    {
        graphics_parsed_unit parse_graphics_pso(std::string& s, const std::string& file_name);
        compute_parsed_unit  parse_compute_pso(std::string& s, const std::string& file_name);
    }
}






