#pragma once

#include <tuple>
#include <uc_dev/gx/import/geo/skinned_mesh.h>
#include <uc_dev/gx/import/fbx/fbx_common.h>
#include <uc_dev/gx/import/fbx/fbx_common_multi_material_mesh.h>

#include <uc_dev/gx/import/fbx/fbx_helpers.h>
#include <uc_dev/gx/import/fbx/fbx_transform_helper.h>
#include <uc_dev/gx/import/fbx/fbx_common_transform.h>
#include <uc_dev/gx/import/fbx/fbx_common_transform_dcc.h>
#include <map>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace fbx
            {
                struct joint_influence
                {
                    std::vector<float>    m_weight;
                    std::vector<uint32_t> m_index;
                };

                //////////////////////
                inline geo::skinned_mesh::blend_weight_t get_blend_weight(const std::vector<float>& v)
                {
                    geo::skinned_mesh::blend_weight_t r = {};
                    float* as_float = &r.x;
                    auto j = 0U;

                    for (auto&& i : v)
                    {
                        if (j >= 4)
                        {
                            break;
                        }
                        as_float[j++] = i;
                    }

                    return r;
                }

                //////////////////////
                inline geo::skinned_mesh::blend_index_t get_blend_index(const std::vector<uint32_t>& v)
                {
                    geo::skinned_mesh::blend_index_t r = {};
                    uint16_t* as_float = &r.x;
                    auto j = 0U;

                    for (auto&& i : v)
                    {
                        if (j >= 4)
                        {
                            break;
                        }
                        as_float[j++] = static_cast<uint16_t>(i);
                    }

                    return r;
                }

                inline geo::joint_transform joint_transform(const fbxsdk::FbxAMatrix& m)
                {
                    geo::joint_transform r;

                    r.m_rotation    = to_float4(m.GetQ());
                    r.m_translation = to_float4(m.GetT());

                    return r;
                }
                inline geo::joint_transform_matrix joint_transform_matrix(const fbxsdk::FbxAMatrix& m)
                {
                    geo::joint_transform_matrix r;

                    //move to row major
                    r.m_transform = to_float4x4(m);
                    return r;
                }

                inline std::vector<fbxsdk::FbxNode*> get_skeleton_nodes(const fbxsdk::FbxNode* n)
                {
                    std::vector<fbxsdk::FbxNode*> r;

                    transform_node_recursive(n, [&r](const fbxsdk::FbxNode* n)
                    {
                        if (n->GetNodeAttribute() && n->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
                        {
                            r.push_back(const_cast<fbxsdk::FbxNode*>(n));
                        }
                    });

                    return r;
                }

                inline std::map<fbxsdk::FbxNode*, uint16_t> get_joint_indices(const fbxsdk::FbxNode* root)
                {
                    auto skeletal_nodes = get_skeleton_nodes(root);

                    std::map<fbxsdk::FbxNode*, uint16_t> joint2index;

                    {
                        for (auto&& i = 0U; i < skeletal_nodes.size(); ++i)
                        {
                            auto n = skeletal_nodes[i];
                            joint2index.insert(std::make_pair(n, static_cast<uint16_t>(i)));
                        }
                    }
                    return joint2index;
                }

                inline std::vector<fbxsdk::FbxCluster*> get_joints_skinned(const fbxsdk::FbxMesh* mesh)
                {
                    std::vector<fbxsdk::FbxCluster*> joints;

                    int skinCount = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);

                    for (int skinIndex = 0; skinIndex < skinCount; skinIndex++)
                    {
                        fbxsdk::FbxSkin* skin = (fbxsdk::FbxSkin *)mesh->GetDeformer(skinIndex, fbxsdk::FbxDeformer::eSkin);

                        int jointsCount = skin->GetClusterCount();

                        joints.resize(jointsCount);

                        for (int jointIndex = 0; jointIndex < jointsCount; jointIndex++)
                        {
                            fbxsdk::FbxCluster* joint = skin->GetCluster(jointIndex);
                            joints[jointIndex] = joint;
                        }
                    }

                    return joints;
                }

                inline std::map<fbxsdk::FbxNode*, fbxsdk::FbxCluster*> get_cluster_nodes(std::vector<fbxsdk::FbxCluster*> clusters)
                {
                    std::map<fbxsdk::FbxNode*, fbxsdk::FbxCluster*> r;

                    for (auto&& i : clusters)
                    {
                        r.insert(std::make_pair(i->GetLink(), i));
                    }

                    return r;
                }

                inline geo::skeleton_pose get_skeleton_pose(const fbxsdk::FbxMesh* mesh)
                {
                    std::vector<uint16_t>                parents;
                    std::map<fbxsdk::FbxNode*, uint16_t> joint2index;

                    geo::skeleton_pose skeleton;

                    auto evaluator              = mesh->GetScene()->GetAnimationEvaluator();
                    auto skeletal_nodes         = get_skeleton_nodes(mesh->GetScene()->GetRootNode());

                    std::vector<fbxsdk::FbxAMatrix> links;
                    {
                        skeleton.m_joint_local_pose.resize(skeletal_nodes.size());
                        for (auto&& i = 0U; i < skeletal_nodes.size(); ++i)
                        {
                            auto n = skeletal_nodes[i];
                            fbxsdk::FbxAMatrix t3 = evaluator->GetNodeGlobalTransform(n);
                            links.push_back(t3);

                            joint2index.insert(std::make_pair(n, static_cast<uint16_t>(i)));
                        }
                    }

                    parents.resize(skeletal_nodes.size());

                    //build up joint hierarchy, root node has parent 0xffff
                    for (auto i =0U; i <skeletal_nodes.size(); ++i)
                    {
                        const auto&& parent_index = joint2index.find(skeletal_nodes[i]->GetParent());
                        if (parent_index != joint2index.end())
                        {
                            parents[i] = parent_index->second;
                        }
                        else
                        {
                            parents[i] = 0xFFFF;
                        }
                    }

                    skeleton.m_joint_local_pose.resize(skeletal_nodes.size());

                    //transform global matrices to parent relative
                    for (auto i = 0U; i < skeletal_nodes.size(); ++i)
                    {
                        fbxsdk::FbxAMatrix parent;
                        fbxsdk::FbxAMatrix this_;
                        parent.SetIdentity();

                        if (parents[i] != 0xffff)
                        {
                            parent = links[parents[i]];
                        }

                        this_   = links[i];
                        auto t4 = parent.Inverse() * this_;
                        auto t5 = t4;
                        skeleton.m_joint_local_pose[i].m_transform          = joint_transform(t5);
                        skeleton.m_joint_local_pose[i].m_transform_matrix   = joint_transform_matrix(t5);
                    }

                    skeleton.m_skeleton.m_joints.resize(skeletal_nodes.size());

                    auto clusters       = get_joints_skinned(mesh);
                    auto skinned_joints = get_cluster_nodes(clusters);
                    auto geometry       = get_geometry(mesh->GetNode());

                    //fill up the inverse bind pose
                    for (auto i = 0U; i < skeletal_nodes.size(); ++i)
                    {
                        fbxsdk::FbxAMatrix parent;

                        fbxsdk::FbxAMatrix this_;
                        this_.SetIdentity();

                        skeleton.m_skeleton.m_joints[i].m_name = skeletal_nodes[i]->GetName();
                        skeleton.m_skeleton.m_joints[i].m_parent_index = parents[i];

                        auto skinned_joint = skinned_joints.find(skeletal_nodes[i]);

                        //if the joint participates in the skinning
                        if (skinned_joint != skinned_joints.end())
                        {
                            fbxsdk::FbxAMatrix init_bind_pose_joint;
                            skinned_joint->second->GetTransformLinkMatrix(init_bind_pose_joint);               // The transformation of the cluster(joint) at binding time from joint space to world space
                            fbxsdk::FbxAMatrix init_reference_global_position;
                            skinned_joint->second->GetTransformMatrix(init_reference_global_position);          // The transformation of the mesh at binding time

                            fbxsdk::FbxAMatrix mat_bind_pose_init = init_reference_global_position.Inverse() * init_bind_pose_joint * geometry;

                            auto t5 = mat_bind_pose_init;

                            skeleton.m_skeleton.m_joints[i].m_inverse_bind_pose     = joint_transform(t5.Inverse());
                            skeleton.m_skeleton.m_joints[i].m_inverse_bind_pose2    = joint_transform_matrix(t5.Inverse());
                        }
                        else
                        {
                            auto t5 = this_;
                            skeleton.m_skeleton.m_joints[i].m_inverse_bind_pose = joint_transform(t5);
                            skeleton.m_skeleton.m_joints[i].m_inverse_bind_pose2 = joint_transform_matrix(t5);
                        }
                    }

                    return skeleton;
                }

                //////////////////////
                inline geo::skinned_mesh::blend_weights_t get_blend_weights(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    std::vector<joint_influence> influences;
                    influences.resize(mesh->GetControlPointsCount());

                    auto joint_indices = get_joint_indices(mesh->GetScene()->GetRootNode());

                    int skinCount = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
                    for (int skinIndex = 0; skinIndex < skinCount; skinIndex++)
                    {
                        FbxSkin* skin = (FbxSkin *)mesh->GetDeformer(skinIndex, fbxsdk::FbxDeformer::eSkin);

                        int jointsCount = skin->GetClusterCount();
                        for (int jointIndex = 0; jointIndex < jointsCount; jointIndex++)
                        {
                            fbxsdk::FbxCluster* joint = skin->GetCluster(jointIndex);

                            int influencedCount = joint->GetControlPointIndicesCount();

                            int* influenceIndices = joint->GetControlPointIndices();
                            double* influenceWeights = joint->GetControlPointWeights();

                            for (int influenceIndex = 0; influenceIndex < influencedCount; influenceIndex++)
                            {
                                int controlPointIndex = influenceIndices[influenceIndex];
                                assert(controlPointIndex < (int)influences.size());//"Invalid skin control point index"
                                influences[controlPointIndex].m_index.push_back(joint_indices.find(joint->GetLink())->second);
                                influences[controlPointIndex].m_weight.push_back((float)influenceWeights[influenceIndex]);
                            }
                        }
                    }

                    auto indices = mesh->GetPolygonVertices();
                    geo::skinned_mesh::blend_weights_t blend_weights;
                    for (auto triangle = 0; triangle < triangle_indices.size(); ++triangle)
                    {
                        auto triange_to_fetch = triangle_indices[triangle];
                        auto i0 = indices[triange_to_fetch * 3];
                        auto i1 = indices[triange_to_fetch * 3 + 1];
                        auto i2 = indices[triange_to_fetch * 3 + 2];

                        auto w0 = influences[i0];
                        auto w1 = influences[i1];
                        auto w2 = influences[i2];

                        auto wp0 = get_blend_weight(w0.m_weight);
                        auto wp1 = get_blend_weight(w1.m_weight);
                        auto wp2 = get_blend_weight(w2.m_weight);

                        blend_weights.push_back(wp0);
                        blend_weights.push_back(wp1);
                        blend_weights.push_back(wp2);
                    }

                    return blend_weights;
                }
                //////////////////////
                inline geo::skinned_mesh::blend_indices_t get_blend_indices(const fbxsdk::FbxMesh* mesh, const std::vector<int32_t>& triangle_indices)
                {
                    std::vector<joint_influence> influences;
                    influences.resize(mesh->GetControlPointsCount());

                    auto joint_indices = get_joint_indices(mesh->GetScene()->GetRootNode());

                    int skinCount = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
                    for (int skinIndex = 0; skinIndex < skinCount; skinIndex++)
                    {
                        FbxSkin* skin = (FbxSkin *)mesh->GetDeformer(skinIndex, fbxsdk::FbxDeformer::eSkin);
                        int jointsCount = skin->GetClusterCount();
                        for (int jointIndex = 0; jointIndex < jointsCount; jointIndex++)
                        {
                            fbxsdk::FbxCluster* joint = skin->GetCluster(jointIndex);

                            int influencedCount = joint->GetControlPointIndicesCount();

                            int* influenceIndices = joint->GetControlPointIndices();
                            double* influenceWeights = joint->GetControlPointWeights();

                            for (int influenceIndex = 0; influenceIndex < influencedCount; influenceIndex++)
                            {
                                int controlPointIndex = influenceIndices[influenceIndex];
                                assert(controlPointIndex < (int)influences.size());//"Invalid skin control point index"
                                influences[controlPointIndex].m_index.push_back(joint_indices.find(joint->GetLink())->second);
                                influences[controlPointIndex].m_weight.push_back((float)influenceWeights[influenceIndex]);
                            }
                        }
                    }

                    auto indices = mesh->GetPolygonVertices();
                    geo::skinned_mesh::blend_indices_t blend_indices;
                    for (auto triangle = 0; triangle < triangle_indices.size(); ++triangle)
                    {
                        auto triange_to_fetch = triangle_indices[triangle];
                        auto i0 = indices[triange_to_fetch * 3];
                        auto i1 = indices[triange_to_fetch * 3 + 1];
                        auto i2 = indices[triange_to_fetch * 3 + 2];

                        auto w0 = influences[i0];
                        auto w1 = influences[i1];
                        auto w2 = influences[i2];

                        auto ip0 = get_blend_index(w0.m_index);
                        auto ip1 = get_blend_index(w1.m_index);
                        auto ip2 = get_blend_index(w2.m_index);

                        blend_indices.push_back(ip0);
                        blend_indices.push_back(ip1);
                        blend_indices.push_back(ip2);
                    }

                    return blend_indices;
                }

                inline bool is_skinned_mesh(const fbxsdk::FbxMesh* mesh)
                {
                    return mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin) > 0;
                }

                //returns which materials, which polygons affect
                inline std::vector< std::vector<int32_t> > get_material_indices(const fbxsdk::FbxMesh* mesh)
                {
                    auto material_count = mesh->GetElementMaterialCount();

                    if (material_count == 0)
                    {
                        ::uc::gx::throw_exception<uc::gx::fbx_exception>("mesh does not have assigned materials");
                    }

                    std::vector< std::vector<int32_t> > materials_indices;
                    //assign polygons to materials
                    for (auto i = 0; i < material_count; ++i)
                    {
                        auto&& element_material = mesh->GetElementMaterial(i);
                        auto&& material_indices = element_material->GetIndexArray();
                        auto count = material_indices.GetCount();

                        if (element_material->GetMappingMode() == fbxsdk::FbxLayerElement::eAllSame)
                        {
                            auto polygon_count = mesh->GetPolygonCount();
                            auto material_index_for_polygon = material_indices.GetAt(0);

                            if (material_index_for_polygon + 1 > materials_indices.size())
                            {
                                materials_indices.resize(material_index_for_polygon + 1);
                            }

                            materials_indices[material_index_for_polygon].reserve(polygon_count);
                            for (auto k = 0; k < polygon_count; ++k)
                            {
                                materials_indices[material_index_for_polygon].push_back(k);
                            }
                        }
                        else
                        {
                            for (auto j = 0; j < count; ++j)
                            {
                                auto material_index_for_polygon = material_indices.GetAt(j);
                                if (material_index_for_polygon + 1 > materials_indices.size())
                                {
                                    materials_indices.resize(material_index_for_polygon + 1);
                                }

                                materials_indices[material_index_for_polygon].push_back(j);
                            }
                        }
                    }

                    return materials_indices;
                }

                
                //////////////////////
                inline  std::vector<geo::skinned_mesh::positions_t> transform_dcc_positions(const std::vector<geo::skinned_mesh::positions_t>& positions, const fbx_context* ctx)
                {
                    std::vector<geo::skinned_mesh::positions_t> r;
                    ctx;

                    auto m0 = negate_z();
                    
                    r.reserve(positions.size());

                    for (auto&& p : positions)
                    {
                        std::vector<geo::skinned_mesh::position_t > pos;
                        pos.reserve(p.size());

                        for(auto&& p0: p)
                        {
                            auto v = transform_from_dcc(math::load3_point(&p0), ctx);
                            geo::skinned_mesh::position_t s;
                            math::store3u_point(&s, v);
                            pos.push_back(s);
                        }

                        r.push_back(pos);
                    }

                    return r;
                }

                //////////////////////
                inline  std::vector<geo::skinned_mesh::normals_t> transform_dcc_normals(const std::vector<geo::skinned_mesh::normals_t>& normals, const fbx_context* ctx)
                {
                    std::vector<geo::skinned_mesh::normals_t> r;
                    ctx;

                    auto m0 = negate_z();

                    r.reserve(normals.size());

                    for (auto&& p : normals)
                    {
                        std::vector<geo::skinned_mesh::normal_t > pos;
                        pos.reserve(p.size());

                        for (auto&& p0 : p)
                        {
                            auto v = transform_from_dcc(math::load3_point(&p0), ctx);
                            geo::skinned_mesh::normal_t s;
                            math::store3u_point(&s, v);
                            pos.push_back(s);
                        }

                        r.push_back(pos);
                    }

                    return r;
                }

                //////////////////////
                inline  std::vector<geo::skinned_mesh::tangents_t> transform_dcc_tangents(const std::vector<geo::skinned_mesh::tangents_t>& normals, const fbx_context* ctx)
                {
                    std::vector<geo::skinned_mesh::tangents_t> r;
                    ctx;

                    auto m0 = negate_z();

                    r.reserve(normals.size());

                    for (auto&& p : normals)
                    {
                        std::vector<geo::skinned_mesh::tangent_t > pos;
                        pos.reserve(p.size());

                        for (auto&& p0 : p)
                        {
                            auto t = math::load4(&p0);

                            //skip w on transform, since it contains sideness

                            math::float4  r0        = math::load4u(&p0);
                            math::float4  r_xyz     = math::permute< math::permute_0x, math::permute_0y, math::permute_0z, math::permute_1w>(r0, math::zero());

                            math::float4  r_xyz_t   = transform_from_dcc( r_xyz, ctx );
                            math::float4  v         = math::permute< math::permute_0x, math::permute_0y, math::permute_0z, math::permute_1w>(r_xyz_t, r0);
                            
                            geo::skinned_mesh::tangent_t s;

                            //todo: store sideness in the last bit
                            math::store3u(&s, v);
                            
                            pos.push_back(s);
                        }

                        r.push_back(pos);
                    }

                    return r;
                }

                inline geo::skinned_mesh::skeleton_pose_t transform_dcc_pose(const geo::skinned_mesh::skeleton_pose_t& p, const fbx_context* ctx)
                {
                    geo::skinned_mesh::skeleton_pose_t q = p;

                    for (auto && r : q.m_joint_local_pose)
                    {
                        auto m                              = r.m_transform_matrix.m_transform;
                        auto m0                             = transform_from_dcc(m, ctx);
                        auto test = r.m_transform.m_rotation;
                        r.m_transform_matrix.m_transform    = m0;
                        r.m_transform.m_rotation            = math::quaternion_normalize(math::matrix_2_quaternion_simd(math::rotation(m0)));
                        r.m_transform.m_translation         = math::translation(m0);

                    }

                    for (auto && r  : q.m_skeleton.m_joints)
                    {
                        auto m          = r.m_inverse_bind_pose2.m_transform;
                        auto m0         = transform_from_dcc(m, ctx);

                        r.m_inverse_bind_pose2.m_transform = m0;
                        r.m_inverse_bind_pose.m_rotation = math::quaternion_normalize(math::matrix_2_quaternion_simd(math::rotation(m0)));
                        r.m_inverse_bind_pose.m_translation = math::translation(m0);
                    }

                    return q;
                }
                

                //////////////////////
                inline std::shared_ptr<geo::skinned_mesh> create_skinned_mesh_internal(const fbxsdk::FbxMesh* mesh, const fbx_context* context)
                {
                    context;
                    const fbxsdk::FbxNode* mesh_node = mesh->GetNode();

                    //check
                    if (mesh == nullptr || !mesh->IsTriangleMesh())
                    {
                        ::uc::gx::throw_exception<uc::gx::fbx_exception>("file does not contain a triangle mesh");
                    }

                    assert(mesh_node);
                    assert(mesh->GetPolygonSize(0));

                    auto materials_indices = get_material_indices(mesh);
                    std::vector<geo::skinned_mesh::positions_t> positions;                  //positions used by every material
                    std::vector<geo::skinned_mesh::normals_t>   normals;                    //normals used by every material
                    std::vector<geo::skinned_mesh::tangents_t>  tangents;                   //tangents used by every material
                    std::vector<geo::skinned_mesh::uvs_t>       uvs;                        //uvs used by every material
                    std::vector<geo::skinned_mesh::blend_weights_t>   blend_weights;        //blend_weights used by every material
                    std::vector<geo::skinned_mesh::blend_indices_t>   blend_indices;        //blend_indices used by every material

                    

                    //get_positions
                    positions.resize(materials_indices.size());
                    normals.resize(materials_indices.size());
                    tangents.resize(materials_indices.size());
                    uvs.resize(materials_indices.size());
                    blend_weights.resize(materials_indices.size());
                    blend_indices.resize(materials_indices.size());

                    for (auto i = 0U; i < materials_indices.size(); ++i)
                    {
                        positions[i] = get_positions(mesh, materials_indices[i]);   //todo: positions may be shared
                        uvs[i] = get_uvs(mesh, materials_indices[i]);               
                        normals[i]  = get_normals(mesh, materials_indices[i]);       
                        tangents[i] = get_tangents(mesh, materials_indices[i]);
                        blend_weights[i] = get_blend_weights(mesh, materials_indices[i]);
                        blend_indices[i] = get_blend_indices(mesh, materials_indices[i]);
                    }

                    //reindex faces, these are indices in the separated positions and uvs
                    std::vector<geo::skinned_mesh::faces_t>  faces; //uvs used by every material
                    faces.resize(materials_indices.size());

                    auto p = triangle_permutaion(context);

                    for (auto i = 0; i < faces.size(); ++i)
                    {
                        //reorient triangles ccw, since they come cw from fbx, if needed
                        for (auto j = 0; j < materials_indices[i].size(); ++j)
                        {
                            auto triangle = j;
                            geo::skinned_mesh::face_t face;
                            face.v0 = triangle * 3 + p[0];
                            face.v1 = triangle * 3 + p[1];
                            face.v2 = triangle * 3 + p[2];

                            faces[i].push_back(face);
                        }
                    }

                    geo::skinned_mesh::skeleton_pose_t pose = get_skeleton_pose(mesh);

                    return std::make_shared<geo::skinned_mesh>(

                        transform_dcc_positions(positions, context),
                        transform_dcc_normals(normals, context),
                        transform_dcc_tangents(tangents, context),
                        std::move(uvs),
                        std::move(faces),
                        get_materials(mesh_node, static_cast<uint32_t>(materials_indices.size())),
                        std::move(blend_weights),
                        std::move(blend_indices),
                        transform_dcc_pose(pose, context)
                        );
                }

                struct point
                {
                    float m_x = 0;
                    float m_y = 0;
                    float m_z = 0;
                };

                struct vector
                {
                    float m_x = 0;
                    float m_y = 0;
                    float m_z = 0;
                };

                struct sphere
                {
                    point m_c;
                    float m_r = 0;
                };

                struct ray
                {
                    point   m_p;
                    vector  m_d;
                };

                float dot(const vector& a, const vector& b)
                {
                    return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
                }

                vector operator-(const vector& a, const vector& b)
                {
                    return { a.m_x - b.m_x,a.m_y - b.m_y,a.m_z - b.m_z };
                }

                vector operator*(const vector& a, float b)
                {
                    return { a.m_x * b, a.m_y *b, a.m_z * b };
                }

                point operator+(const point& a, const vector& b)
                {
                    return { a.m_x + b.m_x,a.m_y + b.m_y, a.m_z + b.m_z };
                }

                vector operator-(const point& a, const point& b)
                {
                    return { a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z };
                }

                float distance(const point& a, const point& b)
                {
                    vector v = b - a;
                    return sqrtf(dot(v, v));
                }

                vector normalize(const vector& v)
                {
                    float norm = sqrtf(dot(v, v));
                    return v * (1.0f / norm);
                }

                int32_t intersect_ray_sphere(const ray& r, const sphere& s, float& t, point& q)
                {
                    vector m = r.m_p - s.m_c;
                    float  b = dot(m, r.m_d);
                    float  c = dot(m, m) - s.m_r * s.m_r;

                    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
                    if (c > 0.0f && b > 0.0f) return 0;
                    float discr = b * b - c;

                    // A negative discriminant corresponds to ray missing sphere
                    if (discr < 0.0f) return 0;

                    // Ray now found to intersect sphere, compute biggest t value of intersection
                    t = -b + sqrt(discr);

                    q = r.m_p + r.m_d * t;
                    return 1;
                }

                point make_point(const geo::storage_position& p)
                {
                    return { p.x, p.y, p.z };
                }

                point largest_distance(const point& x, const std::vector<geo::storage_position>& positions)
                {
                    float max_distance = -FLT_MAX;

                    point y;
                    auto s = positions.size();

                    for (auto i = 0; i < s; ++i)
                    {
                        point p = make_point(positions[i]);
                        float d = distance(x, make_point(positions[i]));

                        if (d > max_distance)
                        {
                            y = p;
                            max_distance = d;
                        }
                    }

                    return y;
                }

                sphere make_enclosing_sphere(const point& a, const point& b)
                {
                    sphere s;

                    s.m_c = { 0.5f * (a.m_x + b.m_x), 0.5f * (a.m_x + b.m_x), 0.5f * (a.m_x + b.m_x) };
                    s.m_r = distance(a, b) * 0.5f;
                    return s;
                }

                sphere make_enclosing_sphere(const sphere& s, const point& b)
                {
                    ray r;

                    r.m_p = b;
                    r.m_d = normalize(s.m_c - b);

                    float t;
                    point q;

                    int32_t test = intersect_ray_sphere(r, s, t, q);
                    assert(test == 1); //we pick the ray in such a way, that we must have an intersection
                    return make_enclosing_sphere(q, b);
                }

                bool  point_inside_sphere(const sphere& a, const point&b)
                {
                    return distance(a.m_c, b) <= a.m_r;
                }

                sphere compute_enclosing_sphere(const std::vector<geo::storage_position>& positions)
                {
                    //naive Ritter's bounding sphere algorithm

                    sphere r = { {0,0,0}, 0 };

                    auto s = positions.size();

                    if (s == 0)
                    {
                        return r;
                    }

                    point x = make_point(positions[0]);

                    if (s == 1)
                    {
                        return { x, 0 };
                    }

                    point y = largest_distance(x, positions);
                    point z = largest_distance(y, positions);

                    sphere s0 = make_enclosing_sphere(y, z);

                    std::vector<bool> points(s, false);

                    int32_t points_to_check = static_cast<int32_t>(s);

                    while (points_to_check > 0)
                    {
                        bool all_inside = true;

                        for (auto i = 0U; i < positions.size(); ++i)
                        {
                            point p = make_point(positions[i]);
                            if (!points[i])
                            {
                                if ( !point_inside_sphere(s0, p) )
                                {
                                    s0 = make_enclosing_sphere(s0, p);
                                    points_to_check = points_to_check - 1;
                                    all_inside = false;
                                    points[i] = true;
                                    break;
                                }
                            }
                        }

                        if (all_inside)
                        {
                            break;
                        }
                    }

                    return s0;
                }


                //////////////////////
                inline std::shared_ptr<geo::skinned_mesh> create_skinned_mesh(const std::string& file_name)
                {
                    auto context    = load_fbx_file(file_name);
                    auto scene      = context->m_scene.get();

                    std::vector<fbxsdk::FbxMesh*> meshes;
                    meshes = get_meshes(scene->GetRootNode(), meshes);

                    for (auto& m : meshes)
                    {
                        m->RemoveBadPolygons();
                        m->ComputeBBox();

                        if ( !has_normals( m ) )
                        {
                            m->GenerateNormals();
                        }

                        if (!has_tangents(m))
                        {
                            m->GenerateTangentsDataForAllUVSets(true);
                        }

                    }

                    std::vector<  std::shared_ptr<geo::skinned_mesh> > multimeshes;
                    for (auto& m : meshes)
                    {
                        //skip meshes without skin and import only the first one
                        if (is_skinned_mesh(m) && multimeshes.empty())
                        {
                            multimeshes.push_back(create_skinned_mesh_internal(m, context.get()));
                            break;
                        }
                    }

                    //merge all multimaterial meshes into one
                    std::vector< geo::skinned_mesh::positions_t > pos;
                    std::vector< geo::skinned_mesh::normals_t >   normals;
                    std::vector< geo::skinned_mesh::tangents_t >  tangents;
                    std::vector< geo::skinned_mesh::uvs_t >       uv;
                    std::vector< geo::skinned_mesh::faces_t >     faces;
                    std::vector< geo::skinned_mesh::material >    mat;
                    std::vector< geo::skinned_mesh::blend_weights_t >    blend_weights;
                    std::vector< geo::skinned_mesh::blend_indices_t >    blend_indices;

                    geo::skeleton_pose pose;

                    for (auto&& m : multimeshes)
                    {
                        pos.insert(pos.end(), m->m_positions.begin(), m->m_positions.end());
                        normals.insert(normals.end(), m->m_normals.begin(), m->m_normals.end());
                        tangents.insert(tangents.end(), m->m_tangents.begin(), m->m_tangents.end());
                        uv.insert(uv.end(), m->m_uv.begin(), m->m_uv.end());
                        faces.insert(faces.end(), m->m_faces.begin(), m->m_faces.end());
                        mat.insert(mat.end(), m->m_materials.begin(), m->m_materials.end());
                        blend_weights.insert(blend_weights.end(), m->m_blend_weights.begin(), m->m_blend_weights.end());
                        blend_indices.insert(blend_indices.end(), m->m_blend_indices.begin(), m->m_blend_indices.end());
                        pose = m->m_skeleton_pose;
                    }

                    //for every bone, compute list of positions

                    {
                        std::vector< std::vector<geo::storage_position> >  position;
                        std::vector<sphere>                                bone_enclosing_spheres;

                        //joints 
                        auto joints_count = pose.m_joint_local_pose.size();

                        position.resize(joints_count);
                        bone_enclosing_spheres.resize(joints_count);

                        for (auto indices = 0U; indices < blend_indices.size(); ++indices)
                        {
                            const auto& indices_    = blend_indices[indices];
                            const auto& weights_    = blend_weights[indices];
                            const auto& positions   = pos[indices];

                            for (auto blend_index = 0U; blend_index < indices_.size(); ++blend_index)
                            {
                                const auto& blend_index_ = indices_[blend_index];
                                const auto& blend_weight_ = weights_[blend_index];

                                if (blend_weight_.x > 0.0f)
                                {
                                    position[blend_index_.x].push_back(positions[blend_index]);
                                }

                                if (blend_weight_.y > 0.0f)
                                {
                                    position[blend_index_.y].push_back(positions[blend_index]);
                                }

                                if (blend_weight_.z > 0.0f)
                                {
                                    position[blend_index_.z].push_back(positions[blend_index]);
                                }

                                if (blend_weight_.w > 0.0f)
                                {
                                    position[blend_index_.w].push_back(positions[blend_index]);
                                }
                            }
                        }

                        for (auto i = 0U; i < joints_count; ++i)
                        {
                            bone_enclosing_spheres[i] = compute_enclosing_sphere(position[i]);
                        }
                    }

                    return std::make_shared<geo::skinned_mesh>(
                        std::move(pos), 
                        std::move(normals),
                        std::move(tangents),
                        std::move(uv),
                        std::move(faces),
                        std::move(mat),
                        std::move(blend_weights),
                        std::move(blend_indices),
                        std::move(pose));
                }
            }
        }
    }
}

