#pragma once

#include <cstdint>

namespace uc
{
    namespace gx
    {
        namespace geo
        {
            struct default_geometry_position
            {
                enum stride
                {
                    value = static_cast<uint32_t>(3 * sizeof(float))
                };
            };

            struct default_geometry_uv
            {
                enum stride
                {
                    value = static_cast<uint32_t>(2 * sizeof(float))
                };
            };

            struct default_geometry_normal
            {
                enum stride
                {
                    value = static_cast<uint32_t>(3 * sizeof(float))
                };
            };

            struct default_geometry_tangent
            {
                enum stride
                {
                    value = static_cast<uint32_t>(3 * sizeof(float))
                };
            };

            struct default_geometry_index
            {
                enum stride
                {
                    value = static_cast<uint32_t>(4)
                };
            };

            struct default_geometry_blend_weight
            {
                enum stride
                {
                    value = static_cast<uint32_t>(4 * sizeof(float))
                };
            };

            struct default_geometry_blend_index
            {
                enum stride
                {
                    value = static_cast<uint32_t>(1 * sizeof(uint32_t))
                };
            };
        }
    }
}

