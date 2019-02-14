#pragma once

#include <cstdint>

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <uc_dev/math/math.h>

namespace uc
{
    namespace gx
    {
        namespace import 
        {
            namespace geo
            {
                using pointer = uint32_t;

                static const uint32_t null_pointer = 0xFFFFFFFF;

                struct storage_position
                {
                    float x = 0.0f;
                    float y = 0.0f;
                    float z = 0.0f;
                };

                inline bool operator==(const storage_position& a, const storage_position& b)
                {
                    return a.x == b.x && a.y == b.y && a.z == b.z;
                }

                inline bool operator!=(const storage_position& a, const storage_position& b)
                {
                    return !(a.x == b.x && a.y == b.y && a.z == b.z);
                }

                struct storage_normal
                {
                    float x = 0.0f;
                    float y = 0.0f;
                    float z = 0.0f;
                };

                inline bool operator==(const storage_normal& a, const storage_normal& b)
                {
                    return a.x == b.x && a.y == b.y && a.z == b.z;
                }

                inline bool operator!=(const storage_normal& a, const storage_normal& b)
                {
                    return !(a.x == b.x && a.y == b.y && a.z == b.z);
                }

                struct storage_tangent
                {
                    float x = 0.0f;
                    float y = 0.0f;
                    float z = 0.0f;
                };

                inline bool operator==(const storage_tangent& a, const storage_tangent& b)
                {
                    return a.x == b.x && a.y == b.y && a.z == b.z;
                }

                inline bool operator!=(const storage_normal& a, const storage_tangent& b)
                {
                    return !(a.x == b.x && a.y == b.y && a.z == b.z);
                }

                struct storage_bitangent
                {
                    float x = 0.0f;
                    float y = 0.0f;
                    float z = 0.0f;
                };

                inline bool operator==(const storage_bitangent& a, const storage_bitangent& b)
                {
                    return a.x == b.x && a.y == b.y && a.z == b.z;
                }

                inline bool operator!=(const storage_bitangent& a, const storage_bitangent& b)
                {
                    return !(a.x == b.x && a.y == b.y && a.z == b.z);
                }

                struct storage_uv
                {
                    float x = 0.0f;
                    float y = 0.0f;
                };

                inline bool operator==(const storage_uv& a, const storage_uv& b)
                {
                    return a.x == b.x && a.y == b.y;
                }

                inline bool operator!=(const storage_uv& a, const storage_uv& b)
                {
                    return !(a.x == b.x && a.y == b.y);
                }

                struct storage_blend_weight
                {
                    float x = 0.0f;
                    float y = 0.0f;
                    float z = 0.0f;
                    float w = 0.0f;
                };

                struct storage_blend_index
                {
                    uint16_t x = 0;
                    uint16_t y = 0;
                    uint16_t z = 0;
                    uint16_t w = 0;
                };

                struct face
                {
                    pointer v0 = null_pointer;
                    pointer v1 = null_pointer;
                    pointer v2 = null_pointer;
                };

                inline bool operator==(const face& a, const face& b)
                {
                    return (a.v0 == b.v0 && a.v1 == b.v1 && a.v2 == b.v2);
                }

                struct edge
                {
                    pointer v0 = null_pointer;
                    pointer v1 = null_pointer;
                };

                struct winged_edge
                {
                    pointer v0 = null_pointer; //start position
                    pointer v1 = null_pointer; //end   position

                    pointer f0 = null_pointer; // left   face
                    pointer f1 = null_pointer; // right  face

                    pointer l_p = null_pointer; // left  predecessor
                    pointer r_p = null_pointer; // right predecessor

                    pointer l_s = null_pointer; // left  successor
                    pointer r_s = null_pointer; // right successor
                };

                struct half_edge
                {
                    pointer o = null_pointer;  //opposite half edge
                    pointer n = null_pointer;  //next half edge

                    pointer v = null_pointer;  //position
                    pointer f = null_pointer;  //face
                };
            }
        }
    }
}

namespace std
{
    // hash functor for bool
    template<> struct hash<uc::gx::import::geo::edge>
    {
        using this_type = uc::gx::import::geo::edge;

        size_t operator()(const this_type val) const
        {
            uint64_t op = ( (uint64_t ) val.v0 << 32ULL ) | val.v1;
            hash<uint64_t> v;
            return v(op);
        }
    };
}

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace geo
            {
                struct indexed_mesh
                {
                    public:

                    using position      = storage_position;
                    using normal        = storage_normal;
                    using tangent       = storage_tangent;
                    using bitangent     = storage_bitangent;
                    using uv            = storage_uv;
                    using face          = gx::import::geo::face;

                    using face_t        = face;

                    using position_t    = position;
                    using uv_t          = uv;
                    using normal_t      = normal;
                    using positions_t   = std::vector<position>;
                    using faces_t       = std::vector<face>;
                    using uvs_t         = std::vector<uv>;
                    using normals_t     = std::vector<normal>;
                    using tangents_t    = std::vector<tangent>;
                    using bitangents_t  = std::vector<bitangent>;

                    indexed_mesh(
                        const std::vector< position >&   positions,
                        const std::vector< face >&     faces
                    ) : m_positions(positions)
                        , m_faces(faces)
                    {
                        initialize();
                    }

                    indexed_mesh(
                        std::vector< position > &&     positions,
                        std::vector< face >   &&     faces
                    ) :
                        m_positions(std::move(positions))
                        , m_faces(std::move(faces))
                    {
                        initialize();
                    }

                    indexed_mesh(
                        std::vector< position > &&     positions,
                        std::vector< uv > &&         uv,
                        std::vector< face >   &&     faces
                    ) :
                        m_positions(std::move(positions))
                        , m_uv(std::move(uv))
                        , m_faces(std::move(faces))
                    {
                        initialize();
                    }

                    indexed_mesh(
                        std::vector< position > &&      positions,
                        std::vector< normal >  &&       normals,
                        std::vector< tangent >  &&      tangents,
                        std::vector< uv > &&            uv,
                        std::vector< face >   &&        faces
                    ) :
                        m_positions(std::move(positions))
                        , m_normals(std::move(normals))
                        , m_tangents(std::move(tangents))
                        , m_uv(std::move(uv))
                        , m_faces(std::move(faces))
                    {
                        initialize();
                    }

                    position* get_position(pointer p)
                    {
                        return &m_positions[static_cast<uint32_t> (p)];
                    }

                    const position* get_position(pointer p) const
                    {
                        return &m_positions[static_cast<uint32_t> (p)];
                    }

                    face*   get_face(pointer p)
                    {
                        return &m_faces[static_cast<uint32_t> (p)];
                    }

                    const face*   get_face(pointer p) const
                    {
                        return &m_faces[static_cast<uint32_t> (p)];
                    }

                    half_edge*   get_edge(pointer p)
                    {
                        return &m_edges_storage[static_cast<uint32_t> (p)];
                    }

                    const half_edge*   get_edge(pointer p) const
                    {
                        return &m_edges_storage[static_cast<uint32_t> (p)];
                    }

                    bool is_half_edge(pointer p) const
                    {
                        return static_cast<uint32_t>(p) < m_edges_storage.size();
                    }

                    std::vector< position >              m_positions;
                    std::vector< normal >                m_normals;
                    std::vector< tangent >               m_tangents;
                    std::vector< uv >                    m_uv;
                    std::vector< face >                  m_faces;
                    std::vector< half_edge >             m_edges_storage;
                    std::unordered_map < edge, pointer > m_edges;

                    void initialize()
                    {
                        build_normals();
                    }

                    std::vector< normal > build_face_normals() const
                    {
                        std::vector< normal > face_normals(m_faces.size());

                        for (uint32_t i = 0; i < face_normals.size(); ++i)
                        {
                            math::float4 v0     = math::load3u_point(&m_positions[m_faces[i].v0]);
                            math::float4 v1     = math::load3u_point(&m_positions[m_faces[i].v1]);
                            math::float4 v2     = math::load3u_point(&m_positions[m_faces[i].v2]);

                            math::float4 n      = math::cross3(math::sub(v0, v1), math::sub(v1, v2));
                            math::float4 normal = math::normalize3(n);
                            math::store3u_vector(&face_normals[i], normal);
                        }

                        return face_normals;
                    }

                    void calculate_pivot()
                    {
                        std::vector< indexed_mesh::position > positions(m_positions.size());

                        math::float4 sum = math::zero();

                        std::for_each(m_positions.begin(), m_positions.end(), [&](const indexed_mesh::position& v)
                        {
                            math::float4 v0 = math::load3u_point(&v);
                            sum = math::add(sum, v0);
                        });

                        auto position_size = static_cast<float>(m_positions.size());
                        sum = math::div(sum, math::set(position_size, position_size, position_size, position_size));
                        sum = math::mul(sum, math::set(1.0f, 1.0f, 1.0f, 0.0f));

                        std::transform(m_positions.begin(), m_positions.end(), positions.begin(), [&](const indexed_mesh::position& v)
                        {
                            math::float4 v0 = math::load3u_point(&v);

                            v0 = math::sub(v0, sum);

                            indexed_mesh::position r;

                            math::store3u_point(&r, v0);

                            return r;
                        });

                        m_positions = std::move(positions);
                    }

                    void clean_degenerate_faces()
                    {
                        std::vector< face > faces(m_faces.size());

                        auto last = std::copy_if(m_faces.begin(), m_faces.end(), faces.begin(), [=](const face& f)
                        {
                            return (f.v0 != f.v1 && f.v0 != f.v2 && f.v1 != f.v2);
                        });

                        faces.resize(std::distance(faces.begin(), last));
                        m_faces = std::move(faces);
                    }

                    void build_normals()
                    {
                        std::vector< normal > normals(m_positions.size());

                        auto face_size = m_faces.size();

                        for (uint32_t i = 0; i < face_size; ++i)
                        {
                            auto i0 = m_faces[i].v0;
                            auto i1 = m_faces[i].v1;
                            auto i2 = m_faces[i].v2;

                            math::float4 v0 = math::load3u_point(&m_positions[i0]);
                            math::float4 v1 = math::load3u_point(&m_positions[i1]);
                            math::float4 v2 = math::load3u_point(&m_positions[i2]);

                            math::float4 n = math::cross3(math::sub(v0, v1), math::sub(v1, v2));

                            math::float4 n0 = math::load3u_vector(&normals[i0]);
                            math::float4 n1 = math::load3u_vector(&normals[i1]);
                            math::float4 n2 = math::load3u_vector(&normals[i2]);

                            n0 = math::add(n0, n);
                            n1 = math::add(n1, n);
                            n2 = math::add(n2, n);

                            math::store3u_point(&normals[i0], n0);
                            math::store3u_point(&normals[i1], n1);
                            math::store3u_point(&normals[i2], n2);

                        }

                        std::transform(normals.begin(), normals.end(), normals.begin(), [=](normal& n0)
                        {
                            math::float4 n  = math::load3u_vector(&n0);
                            math::float4 n1 = math::normalize3(n);

                            normal result;

                            math::store3u_vector(&result, n1);
                            return result;
                        });

                        m_normals = std::move(normals);
                    }

                    void clear_positions_not_referenced_by_faces()
                    {
                        return;
                    }

                    void clean_duplicate_faces()
                    {
                        struct equal_faces
                        {
                            struct hash_function
                            {
                                uint64_t operator() (const face f) const
                                {
                                    return (((uint64_t)f.v0) << 42UL) | (((uint64_t)f.v1) << 21UL) | f.v2;
                                }
                            };

                            static inline face permutation(face f)
                            {
                                face r;
                                r.v0 = f.v2;
                                r.v1 = f.v0;
                                r.v2 = f.v1;
                                return r;
                            }

                            bool operator()(face f0, face f1) const
                            {
                                face p0 = f1;
                                face p1 = permutation(f1);
                                face p2 = permutation(p1);

                                return (f0 == p0 || f0 == p1 || f0 == p2);
                            }
                        };

                        std::unordered_set< face, equal_faces::hash_function, equal_faces > unique_faces;

                        for ( auto&& f : m_faces )
                        {
                            face f0 = f;

                            if (unique_faces.find(f0) == unique_faces.end())
                            {
                                unique_faces.insert(f0);
                            }
                        }

                        std::vector< indexed_mesh::face> faces;
                        faces.resize(unique_faces.size());

                        std::copy(unique_faces.begin(), unique_faces.end(), faces.begin());

                        m_faces = std::move(faces);
                    }
                };

                inline size_t size(const std::vector< indexed_mesh::position >& v)
                {
                    return v.size() * sizeof(indexed_mesh::position);
                }

                inline size_t size(const std::vector< indexed_mesh::face >& v)
                {
                    return v.size() * sizeof(indexed_mesh::face);
                }
            }
        }
    }
}







