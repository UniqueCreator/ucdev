#include "pch.h"
#include "uc_pso_parser_utils.h"

namespace uc
{
    namespace pso
    {
        static parsed_unit::iterator_type get_pos( parsed_unit::iterator_type err_pos, parsed_unit::iterator_type first, int32_t& line )
        {
            line = 1;
            auto i = first;
            auto line_start = first;
            while (i != err_pos)
            {
                bool eol = false;
                if (i != err_pos && *i == '\r') // CR
                {
                    eol = true;
                    line_start = ++i;
                }
                if (i != err_pos && *i == '\n') // LF
                {
                    eol = true;
                    line_start = ++i;
                }
                if (eol)
                    ++line;
                else
                    ++i;
            }

            return line_start;
        }

        location get_location(const parsed_unit& unit, uint32_t annotation_id )
        {
            auto iterator = unit.m_source.cbegin() + unit.m_annotations[annotation_id];

            int32_t line;
            auto line_start = get_pos(iterator, unit.m_source.begin(), line);
            auto column     = static_cast<int32_t>( iterator - line_start );
            location r;

            r.m_row         = line;
            r.m_column      = column;

            return r;
        }
    }
}






