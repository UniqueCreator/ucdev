#pragma once

#include <tuple>
#include <uc_dev/gx/error.h>
#include <uc_dev/math/math.h>


#include <uc_dev/gx/import/fbx/fbx_common.h>
#include <uc_dev/gx/import/fbx/fbx_transform.h>
#include <uc_dev/gx/import/fbx/fbx_transform_helper.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace fbx
            {
                struct get_uv_element
                {
                    virtual fbxsdk::FbxVector2 get_element(uint32_t index) const = 0;
                };

                struct get_uv_element_direct : public get_uv_element
                {
                    const fbxsdk::FbxGeometryElementUV* m_uv;

                public:

                    get_uv_element_direct(const FbxGeometryElementUV* uv) : m_uv(uv)
                    {

                    }

                    virtual fbxsdk::FbxVector2 get_element(uint32_t control_point_index) const override
                    {
                        return m_uv->GetDirectArray().GetAt(control_point_index);
                    }
                };

                struct get_uv_element_index_to_direct : public get_uv_element
                {
                    const fbxsdk::FbxGeometryElementUV* m_uv;

                public:

                    get_uv_element_index_to_direct(const FbxGeometryElementUV* uv) : m_uv(uv)
                    {

                    }

                    virtual fbxsdk::FbxVector2 get_element(uint32_t control_point_index) const override
                    {
                        auto id = m_uv->GetIndexArray().GetAt(control_point_index);
                        return m_uv->GetDirectArray().GetAt(id);
                    }
                };

                struct get_basic_point_index
                {
                    virtual uint32_t get_element(uint32_t triangle_index, uint32_t triangle_vertex) const = 0;
                };

                struct get_uv_control_point_index : public get_basic_point_index
                {
                    int* m_control_points;

                public:
                    get_uv_control_point_index(int* control_points) : m_control_points(control_points)
                    {

                    }

                    virtual uint32_t get_element(uint32_t triangle_index, uint32_t triangle_vertex) const override
                    {
                        assert(triangle_vertex == 0 || triangle_vertex == 1 || triangle_vertex == 2);
                        return m_control_points[triangle_index * 3 + triangle_vertex];
                    }
                };

                struct get_uv_texture_index : public get_basic_point_index
                {
                    const fbxsdk::FbxMesh* m_mesh;
                public:

                    get_uv_texture_index(const fbxsdk::FbxMesh* m) : m_mesh(m)
                    {

                    }

                    virtual uint32_t get_element(uint32_t triangle_index, uint32_t triangle_vertex) const override
                    {
                        auto m = const_cast<fbxsdk::FbxMesh*>(m_mesh);
                        return m->GetTextureUVIndex(triangle_index, triangle_vertex);
                    }
                };

                struct get_vector4_element
                {
                    virtual fbxsdk::FbxVector4 get_element(uint32_t index) const = 0;
                };

                struct get_normal_element_direct : public get_vector4_element
                {
                    const fbxsdk::FbxGeometryElementNormal* m_normal;

                public:

                    get_normal_element_direct(const FbxGeometryElementNormal* normal) : m_normal(normal)
                    {

                    }

                    virtual fbxsdk::FbxVector4 get_element(uint32_t control_point_index) const override
                    {
                        return m_normal->GetDirectArray().GetAt(control_point_index);
                    }
                };

                struct get_normal_element_index_to_direct : public get_vector4_element
                {
                    const fbxsdk::FbxGeometryElementNormal* m_normal;

                    public:

                    get_normal_element_index_to_direct(const FbxGeometryElementNormal* normal) : m_normal(normal)
                    {

                    }

                    virtual fbxsdk::FbxVector4 get_element(uint32_t control_point_index) const override
                    {
                        auto id = m_normal->GetIndexArray().GetAt(control_point_index);
                        return m_normal->GetDirectArray().GetAt(id);
                    }
                };

                
                template <typename fbx_geometry_element_t>
                struct get_typed_element_direct : public get_vector4_element
                {
                    const fbx_geometry_element_t* m_element;

                public:

                    get_typed_element_direct(const fbx_geometry_element_t* element) : m_element(element)
                    {

                    }

                    virtual fbxsdk::FbxVector4 get_element(uint32_t control_point_index) const override
                    {
                        return m_element->GetDirectArray().GetAt(control_point_index);
                    }
                };

                template <typename fbx_geometry_element_t>
                struct get_typed_element_index_to_direct : public get_vector4_element
                {
                    const fbx_geometry_element_t* m_element;

                public:

                    get_typed_element_index_to_direct(const fbx_geometry_element_t* element) : m_element(element)
                    {

                    }

                    virtual fbxsdk::FbxVector4 get_element(uint32_t control_point_index) const override
                    {
                        auto id = m_element->GetIndexArray().GetAt(control_point_index);
                        return m_element->GetDirectArray().GetAt(id);
                    }
                };

                struct get_control_point_index : public get_basic_point_index
                {
                    int* m_control_points;

                    public:
                    get_control_point_index(int* control_points) : m_control_points(control_points)
                    {

                    }

                    virtual uint32_t get_element(uint32_t triangle_index, uint32_t triangle_vertex) const override
                    {
                        return m_control_points[triangle_index * 3 + triangle_vertex];
                    }
                };

                struct get_point_index : public get_basic_point_index
                {
                    const fbxsdk::FbxMesh* m_mesh;
                    public:

                    get_point_index(const fbxsdk::FbxMesh* m) : m_mesh(m)
                    {

                    }

                    virtual uint32_t get_element(uint32_t triangle_index, uint32_t triangle_vertex) const override
                    {
                        return 3 * triangle_index + triangle_vertex;
                    }
                };

                inline geo::indexed_mesh::faces_t get_faces(const fbxsdk::FbxMesh* mesh, const std::array<int32_t, 3> & permutation)
                {
                    auto triangle_count = mesh->GetPolygonCount();
                    geo::indexed_mesh::faces_t faces;
                    faces.reserve(triangle_count);

                    geo::indexed_mesh::positions_t positions;

                    for (auto triangle = 0U; triangle < static_cast<uint32_t>(triangle_count); ++triangle)
                    {
                        //reorient triangles ccw, since they come cw from fbx
                        geo::indexed_mesh::face_t face = { triangle * 3 + permutation[0] , triangle * 3 + permutation[1], triangle * 3 + permutation[2] };
                        faces.push_back(face);
                    }

                    return faces;
                }

                inline geo::indexed_mesh::faces_t get_faces(const fbxsdk::FbxMesh* mesh)
                {
                    auto triangle_count = mesh->GetPolygonCount();
                    geo::indexed_mesh::faces_t faces;
                    faces.reserve(triangle_count);

                    geo::indexed_mesh::positions_t positions;

                    for (auto triangle = 0U; triangle < static_cast<uint32_t>(triangle_count); ++triangle)
                    {
                        //reorient triangles ccw, since they come cw from fbx
                        geo::indexed_mesh::face_t face = { triangle * 3 + 0 , triangle * 3 + 1, triangle * 3 + 2 };
                        faces.push_back(face);
                    }

                    return faces;
                }

                //returns all positions of an fbx sdk mesh
                inline geo::indexed_mesh::positions_t get_positions(const fbxsdk::FbxMesh* mesh)
                {
                    auto points = mesh->GetControlPoints();
                    auto indices = mesh->GetPolygonVertices();

                    auto triangle_count = mesh->GetPolygonCount();
                    geo::indexed_mesh::positions_t positions;

                    auto node_transform = world_transform(mesh->GetNode());

                    for (auto triangle = 0; triangle < triangle_count; ++triangle)
                    {
                        auto i0 = indices[triangle * 3];
                        auto i1 = indices[triangle * 3 + 1];
                        auto i2 = indices[triangle * 3 + 2];

                        //positions
                        {
                            const double* v0 = points[i0];
                            const double* v1 = points[i1];
                            const double* v2 = points[i2];

                            math::float4  vr0 = math::set(static_cast<float>(v0[0]), static_cast<float>(v0[1]), static_cast<float>(v0[2]), 1.0f);
                            math::float4  vr1 = math::set(static_cast<float>(v1[0]), static_cast<float>(v1[1]), static_cast<float>(v1[2]), 1.0f);
                            math::float4  vr2 = math::set(static_cast<float>(v2[0]), static_cast<float>(v2[1]), static_cast<float>(v2[2]), 1.0f);

                            math::float4  vq0 = math::mul(vr0, node_transform);
                            math::float4  vq1 = math::mul(vr1, node_transform);
                            math::float4  vq2 = math::mul(vr2, node_transform);

                            geo::indexed_mesh::position_t vp0;
                            geo::indexed_mesh::position_t vp1;
                            geo::indexed_mesh::position_t vp2;

                            math::store3u(&vp0.x, vq0);
                            math::store3u(&vp1.x, vq1);
                            math::store3u(&vp2.x, vq2);

                            positions.push_back(vp0);
                            positions.push_back(vp1);
                            positions.push_back(vp2);
                        }
                    }

                    return positions;
                }

                //returns all positions, which match triangle_indices;
                inline geo::indexed_mesh::positions_t get_positions(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    auto points = mesh->GetControlPoints();
                    auto indices = mesh->GetPolygonVertices();

                    auto triangle_count = triangle_indices.size();
                    geo::indexed_mesh::positions_t positions;

                    auto node_transform = world_transform(mesh->GetNode());

                    for (auto triangle = 0; triangle < triangle_count; ++triangle)
                    {
                        auto triangle_to_fetch = triangle_indices[triangle];

                        auto i0 = indices[triangle_to_fetch * 3];
                        auto i1 = indices[triangle_to_fetch * 3 + 1];
                        auto i2 = indices[triangle_to_fetch * 3 + 2];

                        //positions
                        {
                            const double* v0 = points[i0];
                            const double* v1 = points[i1];
                            const double* v2 = points[i2];

                            math::float4  vr0 = math::set(static_cast<float>(v0[0]), static_cast<float>(v0[1]), static_cast<float>(v0[2]), 1.0f);
                            math::float4  vr1 = math::set(static_cast<float>(v1[0]), static_cast<float>(v1[1]), static_cast<float>(v1[2]), 1.0f);
                            math::float4  vr2 = math::set(static_cast<float>(v2[0]), static_cast<float>(v2[1]), static_cast<float>(v2[2]), 1.0f);

                            math::float4  vq0 = math::mul(vr0, node_transform);
                            math::float4  vq1 = math::mul(vr1, node_transform);
                            math::float4  vq2 = math::mul(vr2, node_transform);

                            geo::indexed_mesh::position_t vp0;
                            geo::indexed_mesh::position_t vp1;
                            geo::indexed_mesh::position_t vp2;

                            math::store3u(&vp0.x, vq0);
                            math::store3u(&vp1.x, vq1);
                            math::store3u(&vp2.x, vq2);

                            positions.push_back(vp0);
                            positions.push_back(vp1);
                            positions.push_back(vp2);
                        }
                    }

                    return positions;
                }

                inline geo::indexed_mesh::uvs_t get_uvs(const fbxsdk::FbxMesh* mesh)
                {
                    auto indices = mesh->GetPolygonVertices();
                    auto triangle_count = mesh->GetPolygonCount();
                    auto uv = mesh->GetElementUV(0);

                    geo::indexed_mesh::uvs_t        uvs;
                    get_uv_element*                 get_uv = nullptr;
                    get_uv_element_direct           get_uv_0(uv);
                    get_uv_element_index_to_direct  get_uv_1(uv);

                    if (uv->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
                    {
                        get_uv = &get_uv_0;
                    }
                    else
                    {
                        assert(uv->GetReferenceMode() == fbxsdk::FbxGeometryElement::eIndexToDirect);
                        get_uv = &get_uv_1;
                    }

                    get_basic_point_index*      gpi = nullptr;
                    get_uv_texture_index        get_texture0(mesh);
                    get_uv_control_point_index  get_texture1(indices);

                    if (uv->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
                    {
                        gpi = &get_texture1;
                    }
                    else
                    {
                        assert(uv->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex);
                        gpi = &get_texture0;
                        get_uv = &get_uv_0; //todo; check this, produces different uvs? the file data indicates otherwise
                    }

                    for (auto triangle = 0; triangle < triangle_count; ++triangle)
                    {
                        //uv
                        {
                            auto uvi0 = gpi->get_element(triangle, 0);
                            auto uvi1 = gpi->get_element(triangle, 1);
                            auto uvi2 = gpi->get_element(triangle, 2);

                            double* uv0 = get_uv->get_element(uvi0);
                            double* uv1 = get_uv->get_element(uvi1);
                            double* uv2 = get_uv->get_element(uvi2);

                            geo::indexed_mesh::uv_t uvp0 = { static_cast<float>(uv0[0]), static_cast<float>(uv0[1]) };
                            geo::indexed_mesh::uv_t uvp1 = { static_cast<float>(uv1[0]), static_cast<float>(uv1[1]) };
                            geo::indexed_mesh::uv_t uvp2 = { static_cast<float>(uv2[0]), static_cast<float>(uv2[1]) };

                            //transform to directx
                            uvp0.y = 1.0f - uvp0.y;
                            uvp1.y = 1.0f - uvp1.y;
                            uvp2.y = 1.0f - uvp2.y;

                            uvs.push_back(uvp0);
                            uvs.push_back(uvp1);
                            uvs.push_back(uvp2);
                        }
                    }
                    return uvs;
                }

                //returns all uvs, which match triangle_indices
                inline geo::indexed_mesh::uvs_t get_uvs(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    auto indices = mesh->GetPolygonVertices();
                    auto triangle_count = triangle_indices.size();
                    auto uv = mesh->GetElementUV(0);

                    geo::indexed_mesh::uvs_t        uvs;
                    get_uv_element*                 get_uv = nullptr;
                    get_uv_element_direct           get_uv_0(uv);
                    get_uv_element_index_to_direct  get_uv_1(uv);

                    if (uv->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
                    {
                        get_uv = &get_uv_0;
                    }
                    else
                    {
                        assert(uv->GetReferenceMode() == fbxsdk::FbxGeometryElement::eIndexToDirect);
                        get_uv = &get_uv_1;
                    }

                    get_basic_point_index*      gpi = nullptr;
                    get_uv_texture_index        get_texture0(mesh);
                    get_uv_control_point_index  get_texture1(indices);

                    if (uv->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
                    {
                        gpi = &get_texture1;
                    }
                    else
                    {
                        assert(uv->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex);
                        gpi = &get_texture0;
                        get_uv = &get_uv_0; //todo; check this, produces different uvs? the file data indicates otherwise
                    }

                    for (auto triangle = 0; triangle < triangle_count; ++triangle)
                    {
                        auto triangle_to_fetch = triangle_indices[triangle];
                        //uv
                        {
                            auto uvi0 = gpi->get_element(triangle_to_fetch, 0);
                            auto uvi1 = gpi->get_element(triangle_to_fetch, 1);
                            auto uvi2 = gpi->get_element(triangle_to_fetch, 2);

                            double* uv0 = get_uv->get_element(uvi0);
                            double* uv1 = get_uv->get_element(uvi1);
                            double* uv2 = get_uv->get_element(uvi2);

                            geo::indexed_mesh::uv_t uvp0 = { static_cast<float>(uv0[0]), static_cast<float>(uv0[1]) };
                            geo::indexed_mesh::uv_t uvp1 = { static_cast<float>(uv1[0]), static_cast<float>(uv1[1]) };
                            geo::indexed_mesh::uv_t uvp2 = { static_cast<float>(uv2[0]), static_cast<float>(uv2[1]) };

                            //transform to directx
                            uvp0.y = 1.0f - uvp0.y;
                            uvp1.y = 1.0f - uvp1.y;
                            uvp2.y = 1.0f - uvp2.y;

                            uvs.push_back(uvp0);
                            uvs.push_back(uvp1);
                            uvs.push_back(uvp2);
                        }
                    }
                    return uvs;
                }

                template <typename triangle_indices_functor_t, typename triangle_count_functor_t, typename vector_element_type_t, typename return_vectors_type_t>
                void get_vectors_typed(const fbxsdk::FbxMesh* mesh, triangle_indices_functor_t triangle_indices, triangle_count_functor_t triangle_count_functor, const vector_element_type_t* vector_element, return_vectors_type_t& vectors)
                {
                    auto indices        = mesh->GetPolygonVertices();
                    auto triangle_count = triangle_count_functor();
                    auto element        = vector_element;

                    get_vector4_element*                                        gv = nullptr;
                    get_typed_element_direct<vector_element_type_t>             gv_0(element);
                    get_typed_element_index_to_direct<vector_element_type_t>    gv_1(element);

                    if (element->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
                    {
                        gv = &gv_0;
                    }
                    else
                    {
                        assert(element->GetReferenceMode() == fbxsdk::FbxGeometryElement::eIndexToDirect);
                        gv = &gv_1;
                    }

                    get_basic_point_index*                  gpi = nullptr;
                    get_point_index                         get_texture0(mesh);
                    get_control_point_index                 get_texture1(indices);

                    if (element->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
                    {
                        gpi = &get_texture1;
                    }
                    else
                    {
                        assert(element->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex);
                        gpi = &get_texture0;
                        //gv = &gv_0; //todo; check this, produces different normals? the file data indicates otherwise
                    }

                    auto normal_node_transform = math::transpose(math::inverse(world_transform(mesh->GetNode())));

                    for (auto triangle = 0; triangle < triangle_count; ++triangle)
                    {
                        auto triangle_to_fetch = triangle_indices(triangle);
                        //vector
                        {
                            auto normali0 = gpi->get_element(triangle_to_fetch, 0);
                            auto normali1 = gpi->get_element(triangle_to_fetch, 1);
                            auto normali2 = gpi->get_element(triangle_to_fetch, 2);

                            double* normal0 = gv->get_element(normali0);
                            double* normal1 = gv->get_element(normali1);
                            double* normal2 = gv->get_element(normali2);

                            using vector_type_t = typename return_vectors_type_t::value_type;

                            vector_type_t normalp0;
                            vector_type_t normalp1;
                            vector_type_t normalp2;

                            math::float4  vr0 = math::set(static_cast<float>(normal0[0]), static_cast<float>(normal0[1]), static_cast<float>(normal0[2]), 0.0f);
                            math::float4  vr1 = math::set(static_cast<float>(normal1[0]), static_cast<float>(normal1[1]), static_cast<float>(normal1[2]), 0.0f);
                            math::float4  vr2 = math::set(static_cast<float>(normal2[0]), static_cast<float>(normal2[1]), static_cast<float>(normal2[2]), 0.0f);

                            math::float4  vq0 = vr0;// math::normalize3(math::mul(vr0, normal_node_transform));
                            math::float4  vq1 = vr1;// math::normalize3(math::mul(vr1, normal_node_transform));
                            math::float4  vq2 = vr2;// math::normalize3(math::mul(vr2, normal_node_transform));

                            math::store3u(&normalp0, vq0);
                            math::store3u(&normalp1, vq1);
                            math::store3u(&normalp2, vq2);

                            vectors.push_back(normalp0);
                            vectors.push_back(normalp1);
                            vectors.push_back(normalp2);
                        }
                    }
                }

                template <typename triangle_indices_functor_t, typename triangle_count_functor_t, typename vector_element_type_t, typename return_vectors_type_t>
                void get_tangents_typed(const fbxsdk::FbxMesh* mesh, triangle_indices_functor_t triangle_indices, triangle_count_functor_t triangle_count_functor, const vector_element_type_t* vector_element, return_vectors_type_t& vectors)
                {
                    auto indices = mesh->GetPolygonVertices();
                    auto triangle_count = triangle_count_functor();
                    auto element = vector_element;

                    get_vector4_element*                                        gv = nullptr;
                    get_typed_element_direct<vector_element_type_t>             gv_0(element);
                    get_typed_element_index_to_direct<vector_element_type_t>    gv_1(element);

                    if (element->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
                    {
                        gv = &gv_0;
                    }
                    else
                    {
                        assert(element->GetReferenceMode() == fbxsdk::FbxGeometryElement::eIndexToDirect);
                        gv = &gv_1;
                    }

                    get_basic_point_index*                  gpi = nullptr;
                    get_point_index                         get_texture0(mesh);
                    get_control_point_index                 get_texture1(indices);

                    if (element->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
                    {
                        gpi = &get_texture1;
                    }
                    else
                    {
                        assert(element->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex);
                        gpi = &get_texture0;
                        //gv = &gv_0; //todo; check this, produces different normals? the file data indicates otherwise
                    }

                    auto normal_node_transform = math::transpose(math::inverse(world_transform(mesh->GetNode())));

                    for (auto triangle = 0; triangle < triangle_count; ++triangle)
                    {
                        auto triangle_to_fetch = triangle_indices(triangle);
                        //vector
                        {
                            auto normali0 = gpi->get_element(triangle_to_fetch, 0);
                            auto normali1 = gpi->get_element(triangle_to_fetch, 1);
                            auto normali2 = gpi->get_element(triangle_to_fetch, 2);

                            double* normal0 = gv->get_element(normali0);
                            double* normal1 = gv->get_element(normali1);
                            double* normal2 = gv->get_element(normali2);

                            using vector_type_t = typename return_vectors_type_t::value_type;

                            vector_type_t normalp0;
                            vector_type_t normalp1;
                            vector_type_t normalp2;

                            math::float4  vr0 = math::set(static_cast<float>(normal0[0]), static_cast<float>(normal0[1]), static_cast<float>(normal0[2]), static_cast<float>(normal0[3]));
                            math::float4  vr1 = math::set(static_cast<float>(normal1[0]), static_cast<float>(normal1[1]), static_cast<float>(normal1[2]), static_cast<float>(normal1[3]));
                            math::float4  vr2 = math::set(static_cast<float>(normal2[0]), static_cast<float>(normal2[1]), static_cast<float>(normal2[2]), static_cast<float>(normal2[3]));

                            math::float4  vq0 = vr0;// math::normalize3(math::mul(vr0, normal_node_transform));
                            math::float4  vq1 = vr1;// math::normalize3(math::mul(vr1, normal_node_transform));
                            math::float4  vq2 = vr2;// math::normalize3(math::mul(vr2, normal_node_transform));


                            //todo: store sideness in the last bit

                            math::store3u(&normalp0, vq0);
                            math::store3u(&normalp1, vq1);
                            math::store3u(&normalp2, vq2);

                            vectors.push_back(normalp0);
                            vectors.push_back(normalp1);
                            vectors.push_back(normalp2);
                        }
                    }
                }

                static inline geo::indexed_mesh::normals_t transform_normals(const uc::math::float4x4 m, const geo::indexed_mesh::normals_t &s)
                {
                    geo::indexed_mesh::normals_t r;
                    r.resize(s.size());
                    std::transform(s.cbegin(), s.cend(), r.begin(),
                        [&m]( const geo::indexed_mesh::normal& n)
                        {
                            math::float4  r0 = math::load3u_vector(&n);
                            math::float4  r1 = math::normalize3(math::mul(r0, m));

                            geo::indexed_mesh::normal r;
                            math::store3u_vector(&r, r1);
                            return r;
                        });

                    return r;
                }

                static inline geo::indexed_mesh::tangents_t transform_tangents(const uc::math::float4x4 m, const geo::indexed_mesh::tangents_t &s)
                {
                    geo::indexed_mesh::tangents_t r;
                    r.resize(s.size());
                    std::transform(s.cbegin(), s.cend(), r.begin(),
                        [&m](const geo::indexed_mesh::tangent& n)
                    {
                        //transform the tangent, but keep the sign

                        math::float4  r0    = math::load4u(&n);
                        math::float4  r_xyz = math::permute< math::permute_0x, math::permute_0y, math::permute_0z, math::permute_1w>(r0, math::zero());
                        
                        math::float4  r1    = math::normalize3(math::mul(r_xyz, m));
                        math::float4  r2    = math::permute< math::permute_0x, math::permute_0y, math::permute_0z, math::permute_1w>(r1, r0);

                        geo::indexed_mesh::tangent r;
                        math::store3u_vector(&r, r2);

                        return r;
                    });

                    return r;
                }


                inline geo::indexed_mesh::normals_t get_normals(const fbxsdk::FbxMesh* mesh)
                {
                    geo::indexed_mesh::normals_t  vectors;
                    auto f0 = [](auto triangle_index) {return triangle_index; };
                    auto f1 = [&mesh]() { return mesh->GetPolygonCount(); };

                    get_vectors_typed(mesh, f0, f1, mesh->GetElementNormal(0), vectors);

                    auto normal_node_transform = math::transpose(math::inverse(world_transform(mesh->GetNode())));
                    return transform_normals(normal_node_transform, vectors);
                }

                static inline bool has_normals(const fbxsdk::FbxMesh* m)
                {
                    return  m->GetElementNormal(0) != nullptr;
                }

                static inline bool has_tangents(const fbxsdk::FbxMesh* m)
                {
                    return  m->GetElementTangent(0) != nullptr;
                }

                static inline bool has_bitangents(const fbxsdk::FbxMesh* m)
                {
                    return  m->GetElementBinormal(0) != nullptr;
                }

                static inline geo::indexed_mesh::normals_t generate_normals(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    mesh;
                    triangle_indices;
                    geo::indexed_mesh::normals_t                normals;
                    //todo: not implemented
                    return normals;
                }

                static inline geo::indexed_mesh::normals_t generate_normals(const fbxsdk::FbxMesh* mesh)
                {
                    mesh;
                    geo::indexed_mesh::normals_t                normals;
                    //todo: not implemented
                    return normals;
                }

                static inline geo::indexed_mesh::tangents_t generate_tangents(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    mesh;
                    triangle_indices;
                    geo::indexed_mesh::tangents_t      tangents;
                    //todo: not implemented
                    return tangents;
                }

                static inline geo::indexed_mesh::tangents_t generate_tangents(const fbxsdk::FbxMesh* mesh)
                {
                    mesh;
                    geo::indexed_mesh::tangents_t      tangents;
                    //todo: not implemented
                    return tangents;
                }

                static inline geo::indexed_mesh::bitangents_t generate_bitangents(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    mesh;
                    triangle_indices;
                    geo::indexed_mesh::bitangents_t      bitangents;
                    //todo: not implemented
                    return bitangents;
                }

                static inline geo::indexed_mesh::bitangents_t generate_bitangents(const fbxsdk::FbxMesh* mesh)
                {
                    mesh;
                    geo::indexed_mesh::bitangents_t      bitangents;
                    //todo: not implemented
                    return bitangents;
                }

                //returns all normals, which match triangle_indices
                inline geo::indexed_mesh::normals_t get_normals(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    if (has_normals(mesh))
                    {
                        geo::indexed_mesh::normals_t    vectors;
                        auto f  = [&triangle_indices](auto triangle_index) {return triangle_indices[triangle_index]; };
                        auto f0 = [&triangle_indices]() { return triangle_indices.size(); };

                        get_vectors_typed(mesh, f, f0, mesh->GetElementNormal(0), vectors);
                        auto normal_node_transform = math::transpose(math::inverse(world_transform(mesh->GetNode())));
                        return transform_normals(normal_node_transform, vectors);
                    }
                    else
                    {
                        return generate_normals(mesh, triangle_indices);
                    }
                }

                inline geo::indexed_mesh::tangents_t get_tangents(const fbxsdk::FbxMesh* mesh)
                {
                    if (has_tangents(mesh))
                    {
                        geo::indexed_mesh::tangents_t   vectors;
                        auto f0 = [](auto triangle_index) {return triangle_index; };
                        auto f1 = [&mesh]() { return mesh->GetPolygonCount(); };

                        get_tangents_typed(mesh, f0, f1, mesh->GetElementTangent(0), vectors);

                        auto normal_node_transform = math::transpose(math::inverse(world_transform(mesh->GetNode())));
                        return transform_tangents(normal_node_transform, vectors);
                    }
                    else
                    {
                        return generate_tangents(mesh);
                    }
                }

                //returns all normals, which match triangle_indices
                inline geo::indexed_mesh::tangents_t get_tangents(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    if (has_tangents(mesh))
                    {
                        geo::indexed_mesh::tangents_t   vectors;
                        auto f0 = [&triangle_indices](auto triangle_index) {return triangle_indices[triangle_index]; };
                        auto f1 = [&triangle_indices]() { return triangle_indices.size(); };
                        
                        get_tangents_typed(mesh, f0, f1, mesh->GetElementTangent(0), vectors);

                        auto normal_node_transform = math::transpose(math::inverse(world_transform(mesh->GetNode())));
                        return transform_tangents(normal_node_transform, vectors);
                    }
                    else
                    {
                        return generate_tangents(mesh, triangle_indices);
                    }
                }

                inline geo::indexed_mesh::bitangents_t get_bitangents(const fbxsdk::FbxMesh* mesh)
                {
                    if (has_bitangents(mesh))
                    {
                        geo::indexed_mesh::bitangents_t vectors;
                        auto f0 = [](auto triangle_index) {return triangle_index; };
                        auto f1 = [&mesh]() { return mesh->GetPolygonCount(); };

                        get_vectors_typed(mesh, f0, f1, mesh->GetElementBinormal(0), vectors);

                        return vectors;
                    }
                    else
                    {
                        return generate_bitangents(mesh);
                    }
                }

                //returns all normals, which match triangle_indices
                inline geo::indexed_mesh::bitangents_t get_bitangents(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    if ( has_bitangents(mesh) )
                    {
                        geo::indexed_mesh::bitangents_t vectors;
                        auto f0 = [&triangle_indices](auto triangle_index) { return triangle_indices[triangle_index]; };
                        auto f1 = [&triangle_indices]() { return triangle_indices.size(); };
                        get_vectors_typed(mesh, f0, f1, mesh->GetElementBinormal(0), vectors);
                        return vectors;
                    }
                    else
                    {
                        return generate_bitangents(mesh, triangle_indices);
                    }
                }
            }
        }
    }
}

