#pragma once

#include <uc_dev/gx/import/geo/indexed_mesh.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace geo
            {
                struct multi_material_mesh
                {
                    using position_t    = indexed_mesh::position;
                    using normal_t      = indexed_mesh::normal;
                    using tangent_t     = indexed_mesh::tangent;
                    using uv_t          = indexed_mesh::uv;
                    using face_t        = indexed_mesh::face;

                    using positions_t   = indexed_mesh::positions_t;
                    using uvs_t         = indexed_mesh::uvs_t;
                    using normals_t     = indexed_mesh::normals_t;
                    using tangents_t    = indexed_mesh::tangents_t;
                    using faces_t       = indexed_mesh::faces_t;

                    enum material_type : uint16_t
                    {
                        diffuse = 0,
                        diffuse_alpha_test = 1
                    };

                    struct material
                    {
                        std::string     m_name;
                        material_type   m_type;
                    };

                public:

                    multi_material_mesh
                    (
                        std::vector<positions_t>&& positions,
                        std::vector<normals_t>&& normals,
                        std::vector<tangents_t>&& tangents,
                        std::vector<uvs_t>&& uv,
                        std::vector<faces_t>&& faces,
                        std::vector<material>&& materials
                    ) : m_positions(std::move(positions))
                        , m_normals(std::move(normals))
                        , m_tangents(std::move(tangents))
                        , m_uv(std::move(uv))
                        , m_faces(std::move(faces))
                        , m_materials(std::move(materials))
                    {

                    }

                    multi_material_mesh
                    (
                        positions_t&& positions,
                        normals_t&& normals,
                        tangents_t&& tangents,
                        uvs_t&& uv,
                        faces_t&& faces,
                        std::vector<material>&& materials
                    ) : m_materials(std::move(materials))
                    {
                        m_positions.emplace_back(std::move(positions));
                        m_normals.emplace_back(std::move(normals));
                        m_tangents.emplace_back(std::move(tangents));
                        m_uv.emplace_back(std::move(uv));
                        m_faces.emplace_back(std::move(faces));
                    }

                    std::vector<positions_t> m_positions;
                    std::vector<normals_t>   m_normals;
                    std::vector<tangents_t>  m_tangents;
                    std::vector<uvs_t>       m_uv;
                    std::vector<faces_t>     m_faces;
                    std::vector<material>    m_materials;
                };

                template <typename mesh> struct mesh_view
                {
                    const mesh*         m_mesh;
                    using position_t    = typename mesh::position_t;
                    using uv_t          = typename mesh::uv_t;
                    using normal_t      = typename mesh::normal_t;
                    using tangent_t     = typename mesh::tangent_t;
                    using face_t        = typename mesh::face_t;
                };

                template <typename mesh> struct material_view : public mesh_view<mesh>
                {
                    using base          = mesh_view<mesh>;
                    using positions_t   = multi_material_mesh::positions_t;
                    using uvs_t         = multi_material_mesh::uvs_t;
                    using normals_t     = multi_material_mesh::normals_t;
                    using tangents_t    = multi_material_mesh::tangents_t;
                    using faces_t       = multi_material_mesh::faces_t;
                    using material      = multi_material_mesh::material;

                    using position_t    = typename base::position_t;
                    using normal_t      = typename base::normal_t;
                    using tangent_t     = typename base::tangent_t;
                    using uv_t          = typename base::uv_t;
                    using face_t        = typename base::face_t;

                    using material_index = uint32_t;

                    uint32_t index(material_index  i) const
                    {
                        return m_indices[i];
                    }

                    const positions_t& position(material_index idx) const
                    {
                        return m_mesh->m_positions[index(idx)];
                    }

                    const normals_t& normal(material_index idx) const
                    {
                        return m_mesh->m_normals[index(idx)];
                    }

                    const tangents_t& tangent(material_index idx) const
                    {
                        return m_mesh->m_tangents[index(idx)];
                    }

                    const uvs_t& uv(material_index idx) const
                    {
                        return m_mesh->m_uv[index(idx)];
                    }

                    const faces_t& face(material_index idx) const
                    {
                        return m_mesh->m_faces[index(idx)];
                    }

                    size_t vertex_count() const
                    {
                        //todo: cache this property
                        size_t r = 0;

                        auto   s = material_count();

                        for (auto i = 0U; i < s; ++i)
                        {
                            r += position(i).size();
                        }

                        return r;
                    }

                    size_t face_count() const
                    {
                        //todo: cache this property
                        size_t r = 0;

                        auto   s = material_count();

                        for (auto i = 0U; i < s; ++i)
                        {
                            r += face(i).size();
                        }

                        return r;
                    }

                    size_t index_count() const
                    {
                        return face_count() * 3; //triangle list
                    }

                    size_t material_count() const
                    {
                        return m_indices.size();
                    }

                    std::vector<material_index> m_indices;
                };

                struct multi_material_mesh_view : public material_view < multi_material_mesh >
                {
                    using base = material_view < multi_material_mesh>;

                    using positions_t   = base::positions_t;
                    using uvs           = base::uvs_t;
                    using normals       = base::normals_t;
                    using tangents      = base::tangents_t;
                    using faces         = base::faces_t;
                    using material      = base::material;
                    using position_t    = base::position_t;
                };

                inline multi_material_mesh_view identity(multi_material_mesh* m)
                {
                    multi_material_mesh_view v;
                    v.m_mesh = m;
                    v.m_indices.reserve(m->m_materials.size());
                    for (auto i = 0U; i < m->m_materials.size(); ++i)
                    {
                        v.m_indices.push_back(i);
                    }

                    return v;
                }

                template<typename permutator> inline multi_material_mesh_view permute(const multi_material_mesh* m, permutator p)
                {
                    multi_material_mesh_view v;
                    v.m_mesh = m;
                    v.m_indices.reserve(m->m_materials.size());
                    for (auto i = 0U; i < m->m_materials.size(); ++i)
                    {
                        v.m_indices.push_back(i);
                    }

                    std::sort(v.m_indices.begin(), v.m_indices.end(), p);
                    return v;
                }

                inline multi_material_mesh_view multi_mesh_material_view(const multi_material_mesh* m, std::vector<uint32_t>&& indices)
                {
                    multi_material_mesh_view v;
                    v.m_mesh = m;
                    v.m_indices = std::move(indices);

                    return v;
                }

                inline multi_material_mesh_view sort_by_material_type(multi_material_mesh* m)
                {
                    using material_index = multi_material_mesh_view::material_index;

                    return permute(m, [m](material_index a, material_index b)
                    {
                        return m->m_materials[a].m_type < m->m_materials[b].m_type;
                    });
                }
            }
        }
    }
}







