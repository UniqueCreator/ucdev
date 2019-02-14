#pragma once

#include <memory>

#include "amd_mesh.h"
#include "amd_bone.h"

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        enum AnimationType : uint32_t
        {
            StaticGeometry      = 0,
            SkinningAnimation,
            ShapeAnimation,
            HierarchyAnimation
        };

        struct Material
        {
            std::array<wchar_t, 64> m_diffuse_map_file;
            std::array<wchar_t, 64> m_bump_map_file;
        };

        struct BlendData
        {
            UInt32   m_bone;
            Float3   m_offset;
            Float    m_weight;
        };

        struct SkinningData
        {
            UInt32                      m_num_bones     = 0;
            std::unique_ptr<BlendData>  m_blend_data;
        };

        struct Triangle
        {
            uint32_t m_v[3];
            Float3   m_normal;
        };

        class Model : public Transform
        {
            public:

            std::vector<Float3>             m_positions;
            std::vector<Float3>             m_normal;
            std::vector<Float3>             m_tangent;
            std::vector<Float3>             m_bitangent;
            std::vector<Float2>             m_texture_coord;
            std::vector<Float4>             m_color;


            std::array<wchar_t, 64>         m_texture_path;
            int32_t                         m_animation_type    = 0;
            uint32_t                        m_num_vertices      = 0;

            std::vector<SkinningData>       m_skinning_data;
            std::vector<Triangle>           m_triangle;
            std::vector<Material>           m_material;

            std::vector<Mesh>               m_mesh;

            std::vector< Transform*>        m_mesh_hie;
            
            std::vector< Transform*>        m_bone_hie;
            std::vector< Bone>              m_bones;

            uint32_t                        GetTriangleCount() const { return static_cast<uint32_t>(m_triangle.size()); }
            Triangle*                       GetTriangles() { return &m_triangle[0]; }
            const Triangle*                 GetTriangles() const { return &m_triangle[0]; }

            uint32_t                        GetVertexCount() const { return static_cast<uint32_t>(m_positions.size()); } 
            Float3*                         GetPositions() { return &m_positions[0]; }
            const Float3*                   GetPositions() const { return &m_positions[0]; }
        };

        std::unique_ptr<Model> CreateFromFile( const wchar_t* file_name );
    }
}

