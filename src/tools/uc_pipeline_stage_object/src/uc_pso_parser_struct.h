#pragma once

#include <uc_pso_ast.h>
#include <string>
#include <vector>

namespace uc
{
    namespace pso
    {
        struct parsed_unit
        {
            using iterator_type = std::string::const_iterator;
            std::string                  m_source;
            std::string                  m_file_name;
            std::vector< uint32_t >      m_annotations;
        };

        struct graphics_parsed_unit : public parsed_unit
        {
            ast::graphics_compile_unit   m_ast;
        };

        struct compute_parsed_unit : public parsed_unit
        {
            ast::compute_compile_unit    m_ast;
        };
    }
}






