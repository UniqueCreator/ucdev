#pragma once

#include <cstdint>
#include <vector>

#include <uc_dev/math/math_half.h>

namespace uc
{
    namespace gx
    {
        inline std::vector<math::half>  create_positions_x_y_z(const float * positions_x_y_z, uint32_t count, float w = 1.0f)
        {
            auto size = count;
            auto padded_size = 24 * ((size + 23) / 24);

            std::vector<math::half> positions_h(4 * padded_size);

            math::convert_3_x_f32_f16_stream(positions_x_y_z, 3 * padded_size, w, &positions_h[0]);

            return std::move(positions_h);
        }

        inline std::vector<math::half>  create_positions_x_y_z_w(const float * positions_x_y_z_w, uint32_t count)
        {
            auto size = count;

            std::vector<math::half> positions_h(4 * size);

            math::convert_f32_f16_stream(positions_x_y_z_w, size, &positions_h[0]);

            return std::move(positions_h);
        }
    }
}

