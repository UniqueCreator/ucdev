#include "pch.h"

#include "amd_model.h"

#include <array>
#include <ppl.h>

#include <uc_public/assets/assets.h>
#include "amd_file_stream.h"


namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        namespace
        {
            struct OffsetTable
            {
                UInt32  m_material_chunk;
                UInt32  m_vertex_chunk;
                UInt32  m_triangle_chunk;
                UInt32  m_mesh_chunk;
                UInt32  m_bone_chunk;
            };

            static void recursion_mesh_traverse(Model* model, std::vector<Transform*>& hie, Transform *parent, uint32_t* traverse_index)
            {
                hie[*traverse_index] = parent;
                
                *traverse_index = *traverse_index + 1;

                auto s = parent->m_children.size();

                for (auto i = 0U; i < s; i++)
                {
                    recursion_mesh_traverse(model, hie, &model->m_mesh[ parent->m_children[i] ], traverse_index);
                }
            }

            static void build_mesh_hierarchy( Model* m)
            {
                uint32_t i;

                if (!m->m_mesh.empty())
                {
                    m->m_mesh_hie.resize(m->m_mesh.size());

                    for (i = 0; i < m->m_mesh.size(); ++i)
                    {
                        if (m->m_mesh[i].m_parent == -1)
                        {
                            break;
                        }
                    }

                    uint32_t traverse_index = 0;
                    recursion_mesh_traverse(m, m->m_mesh_hie, &m->m_mesh[i], &traverse_index);
                }
            }

            static void recursion_bone_traverse(Model* model, std::vector<Transform*>& hie, Transform *parent, uint32_t* traverse_index)
            {
                hie[*traverse_index] = parent;

                *traverse_index = *traverse_index + 1;

                auto s = parent->m_children.size();

                for (auto i = 0U; i < s; i++)
                {
                    recursion_mesh_traverse(model, hie, &model->m_bones[parent->m_children[i]], traverse_index);
                }
            }

            static void build_bone_hierarchy(Model* m)
            {
                uint32_t i;

                if (!m->m_bones.empty())
                {
                    m->m_bone_hie.resize(m->m_bones.size());

                    for (i = 0; i < m->m_bones.size(); ++i)
                    {
                        if (m->m_mesh[i].m_parent == -1)
                        {
                            break;
                        }
                    }

                    uint32_t traverse_index = 0;
                    recursion_bone_traverse(m, m->m_bone_hie, &m->m_bones[i], &traverse_index);
                }
            }
        }

        std::unique_ptr<Model> CreateFromFile( const wchar_t* file_name )
        {
            SimpleReadFileStream s(file_name);

            std::unique_ptr<Model> r = std::make_unique<Model>();

            wcscpy_s(&r->m_texture_path[0], 1, _T(""));

            uint32_t version = 0;
            OffsetTable offsetTable;

            char Header[9] = "AtiModel";
            char ReadMark[9];

            s.ReadBuffer(ReadMark, 8);
            ReadMark[8] = '\0';
            
            if ( _stricmp(ReadMark, Header) )
            {
                throw StreamReadException();
            }

            Read( s, r->m_animation_type );
            Read( s, version );
            Read( s, offsetTable );

            {
                s.Seek(offsetTable.m_material_chunk);

                uint32_t num_materials;
                Read(s, num_materials);

                r->m_material.resize(num_materials);

                for (auto i = 0U; i < num_materials; ++i)
                {
                    //skip...

                    s.SeekRelative(64);
                    s.SeekRelative(sizeof(Float4));
                    s.SeekRelative(sizeof(Float4));
                    s.SeekRelative(sizeof(Float));

                    {
                        char buf[64];
                        s.ReadBuffer(&buf[0], 64);
                        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, buf, -1, &r->m_material[i].m_diffuse_map_file[0], static_cast<int>(r->m_material[i].m_diffuse_map_file.size()));
                    }

                    //skip
                    s.SeekRelative(64);

                    {
                        char buf[64];
                        s.ReadBuffer(&buf[0], 64);
                        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, buf, -1, &r->m_material[i].m_bump_map_file[0], static_cast<int>(r->m_material[i].m_bump_map_file.size()));

                    }

                    //skip
                    s.SeekRelative(64);
                    s.SeekRelative(64);
                }
            }

            {
                s.Seek(offsetTable.m_vertex_chunk);
                uint32_t num_vertices;

                Read(s, num_vertices);

                r->m_num_vertices = num_vertices;

                r->m_positions.resize(num_vertices);
                r->m_normal.resize(num_vertices);
                r->m_tangent.resize(num_vertices);
                r->m_bitangent.resize(num_vertices);
                r->m_texture_coord.resize(num_vertices);
                r->m_color.resize(num_vertices);

                s.ReadBuffer(&r->m_positions[0], num_vertices * sizeof(Float3) );
                s.ReadBuffer(&r->m_normal[0], num_vertices * sizeof(Float3));
                s.ReadBuffer(&r->m_tangent[0], num_vertices * sizeof(Float3));
                s.ReadBuffer(&r->m_bitangent[0], num_vertices * sizeof(Float3));
                s.ReadBuffer(&r->m_texture_coord[0], num_vertices * sizeof(Float2));
                s.ReadBuffer(&r->m_color[0], num_vertices * sizeof(Float4));

                r->m_skinning_data.resize(num_vertices);

                for (auto i = 0U; i<num_vertices; i++)
                {
                    Read(s, r->m_skinning_data[i].m_num_bones);
                    if (r->m_skinning_data[i].m_num_bones)
                    {
                        r->m_skinning_data[i].m_blend_data = std::make_unique<BlendData>();

                        auto bones = r->m_skinning_data[i].m_num_bones;

                        for (auto b = 0U; b < bones; b++)
                        {
                            Read(s, r->m_skinning_data[i].m_blend_data->m_bone);
                            Read(s, r->m_skinning_data[i].m_blend_data->m_offset);
                            Read(s, r->m_skinning_data[i].m_blend_data->m_weight);
                        }
                    }
                }
            }

            {
                s.Seek(offsetTable.m_triangle_chunk);
                uint32_t num_triangles;

                Read(s, num_triangles);

                r->m_triangle.resize(num_triangles);

                for (auto i = 0U; i < num_triangles; ++i)
                {
                    Read(s, r->m_triangle[i].m_v[0]);
                    Read(s, r->m_triangle[i].m_v[1]);
                    Read(s, r->m_triangle[i].m_v[2]);
                    Read(s, r->m_triangle[i].m_normal);
                }
            }

            {
                s.Seek(offsetTable.m_mesh_chunk);
                uint32_t num_meshes;
                Read(s, num_meshes);


                r->m_mesh.resize(num_meshes);

                for (auto i = 0U; i < num_meshes; ++i)
                {
                    r->m_mesh[i].m_id = i;

                    auto& m = r->m_mesh[i];
                    
                    s.ReadBuffer(&m.m_name[0], 64);

                    Read(s, m.m_material_index);
                    Read(s, m.m_base_vertex);
                    Read(s, m.m_num_vertices);
                    Read(s, m.m_base_triangle);
                    Read(s, m.m_num_triangles);

                    {
                        uint32_t num_bones;
                        Read(s, num_bones);
                        
                        if (num_bones > 0)
                        {
                            m.m_bones.resize(num_bones);
                            s.ReadBuffer(&m.m_bones[0], num_bones * sizeof(UInt32));
                        }
                    }


                    {
                        Read(s, m.m_parent);
                        uint32_t num_children;

                        Read(s, num_children);

                        if (num_children > 0)
                        {
                            m.m_children.resize(num_children);
                            s.ReadBuffer(&m.m_children[0], num_children * sizeof(UInt32));
                        }
                    }


                    {
                        uint32_t num_primitives;

                        Read(s, num_primitives);

                        if (num_primitives)
                        {
                            m.m_primitives.resize(num_primitives);

                            for (auto i = 0U; i < num_primitives; i++)
                            {
                                auto& p = m.m_primitives[i];

                                Read(s, p.m_type);

                                uint32_t num_indices;

                                Read(s, num_indices);
                                p.m_indices.resize(num_indices);
                                s.ReadBuffer(&p.m_indices[0], num_indices * sizeof(uint32_t));
                            }
                        }
                    }

                    {
                        uint32_t num_position_keys;

                        Read(s, num_position_keys);

                        if (num_position_keys)
                        {
                            m.m_animation.m_position_keys.m_keys.resize(num_position_keys);

                            for (auto i = 0U; i < num_position_keys; ++i)
                            {
                                auto& key = m.m_animation.m_position_keys.m_keys[i];
                                Read(s, key.m_start_time);
                                Read(s, key.m_end_time);
                                Read(s, key.m_value);
                            }
                        }
                    }


                    {
                        uint32_t num_rotation_keys;

                        Read(s, num_rotation_keys);

                        if (num_rotation_keys)
                        {
                            m.m_animation.m_rotation_keys.m_keys.resize(num_rotation_keys);

                            for (auto i = 0U; i < num_rotation_keys; ++i)
                            {
                                auto& key = m.m_animation.m_rotation_keys.m_keys[i];
                                Read(s, key.m_start_time);
                                Read(s, key.m_end_time);
                                Read(s, key.m_value);
                            }
                        }
                    }

                    {
                        uint32_t num_scale_keys;

                        Read(s, num_scale_keys);

                        if (num_scale_keys)
                        {
                            m.m_animation.m_scale_keys.m_keys.resize(num_scale_keys);

                            for (auto i = 0U; i < num_scale_keys; ++i)
                            {
                                auto& key = m.m_animation.m_scale_keys.m_keys[i];
                                Read(s, key.m_start_time);
                                Read(s, key.m_end_time);
                                Read(s, key.m_value);
                            }
                        }
                    }
                }
            }


        {
            s.Seek(offsetTable.m_bone_chunk);
            uint32_t num_bones;

            Read(s, num_bones);

            if (num_bones)
            {
                r->m_bones.resize(num_bones);

                for (auto i = 0U; i < num_bones; ++i)
                {
                    auto&& bone = r->m_bones[i];
                    bone.m_id = i;

                    //skip
                    s.SeekRelative(64);

                    Read(s, bone.m_parent);
                    {

                        uint32_t children;
                        Read(s, children);

                        if (children > 0)
                        {
                            bone.m_children.resize(children);

                            s.ReadBuffer(&bone.m_children[0], children * sizeof(uint32_t));
                        }
                    }

                    {
                        uint32_t num_position_keys;

                        Read(s, num_position_keys);

                        if (num_position_keys)
                        {
                            bone.m_animation.m_position_keys.m_keys.resize(num_position_keys);

                            for (auto i = 0U; i < num_position_keys; ++i)
                            {
                                auto& key = bone.m_animation.m_position_keys.m_keys[i];
                                Read(s, key.m_start_time);
                                Read(s, key.m_end_time);
                                Read(s, key.m_value);
                            }
                        }
                    }


                    {
                        uint32_t num_rotation_keys;

                        Read(s, num_rotation_keys);

                        if (num_rotation_keys)
                        {
                            bone.m_animation.m_rotation_keys.m_keys.resize(num_rotation_keys);

                            for (auto i = 0U; i < num_rotation_keys; ++i)
                            {
                                auto& key = bone.m_animation.m_rotation_keys.m_keys[i];
                                Read(s, key.m_start_time);
                                Read(s, key.m_end_time);
                                Read(s, key.m_value);
                            }
                        }
                    }

                    {
                        uint32_t num_scale_keys;

                        Read(s, num_scale_keys);

                        if (num_scale_keys)
                        {
                            bone.m_animation.m_scale_keys.m_keys.resize(num_scale_keys);

                            for (auto i = 0U; i < num_scale_keys; ++i)
                            {
                                auto& key = bone.m_animation.m_scale_keys.m_keys[i];
                                Read(s, key.m_start_time);
                                Read(s, key.m_end_time);
                                Read(s, key.m_value);
                            }
                        }
                    }
                }
            }
        }

            build_mesh_hierarchy(r.get());
            build_bone_hierarchy(r.get());

            return r;
        }
    }
}


