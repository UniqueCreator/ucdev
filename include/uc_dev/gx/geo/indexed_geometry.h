#pragma once

#include <d3d12.h>
#include <vector>

#include <uc_dev/gx/dx12/gpu/managed_buffer.h>
#include "vertex_strides.h"

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            struct primitive_range
            {
                uint32_t m_begin = 0;
                uint32_t m_end   = 0;

                primitive_range(uint32_t begin, uint32_t end) : m_begin(begin), m_end(end)
                {

                }

                uint32_t size() const
                {
                    return m_end - m_begin;
                }
            };
        }
    }
}

