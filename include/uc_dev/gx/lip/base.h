#pragma once

#include <uc_dev/lip/lip.h>

namespace uc
{
    namespace lip
    {
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < uint8_t >)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < uint16_t >)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < uint32_t >)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < uint64_t >)
    }
}
