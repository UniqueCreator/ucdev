#pragma once

#include <array>

#include "amd_transform.h"

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        enum PrimitiveTopology : uint32_t
        {
            List = 0,
            Strip,
            Fan
        };

        struct Primitive
        {
            PrimitiveTopology       m_type;
            uint32_t                m_base_index;
            std::vector<uint32_t>   m_indices;
        };

        class Mesh : public Transform
        {
            public:

            std::array<char, 64>            m_name;
            UInt32                          m_material_index;

            UInt32                          m_base_vertex;
            UInt32                          m_num_vertices;
            UInt32                          m_base_triangle;
            UInt32                          m_num_triangles;

            std::vector<UInt32>             m_bones;
            std::vector<Primitive>          m_primitives;
        };
    }
}

