#pragma once

#include <cstdint>

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        struct Float2
        {
            float x, y;
        };

        struct Float3
        {
            float x, y, z;
        };

        struct Float4
        {
            float x, y, z, w;
        };

        struct Float4x4
        {
            Float4 m_r0;
            Float4 m_r1;
            Float4 m_r2;
            Float4 m_r3;
        };

        using Float     = float;
        using Int32     = int32_t;
        using UInt32    = uint32_t;

        using Int8      = int8_t;
        using UInt8     = uint8_t;

        using UInt16    = uint16_t;
        using Int16     = int16_t;

        using UInt64    = uint64_t;
        using Int64     = int64_t;
        

        constexpr Float4x4 Identity()
        {
            return { { 1.0f, 0.0, 0.0, 0.0f },{ 0.0f, 1.0, 0.0, 0.0f },{ 0.0f, 0.0, 1.0, 0.0f },{ 0.0f, 0.0, 0.0, 1.0f } };
        }
    }
}

