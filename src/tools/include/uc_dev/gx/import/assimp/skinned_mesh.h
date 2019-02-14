#pragma once


#include <cwctype>
#include <locale>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

#include <uc_dev/gx/import/geo/skinned_mesh.h>
#include <uc_dev/gx/import/assimp/multi_material_mesh.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {
                struct joint_influence
                {
                    float    m_weight;

                    joint_influence(float w) : m_weight(w)
                    {

                    }

                    operator float() const
                    {
                        return m_weight;
                    }
                };

                struct joint_index
                {
                    uint16_t    m_index;

                    joint_index(uint16_t w) : m_index(w)
                    {

                    }

                    operator uint16_t() const
                    {
                        return m_index;
                    }
                };

                using nodes_names_map           = std::unordered_map< std::string, const aiNode*>;
                using joint_indices_map         = std::unordered_map< std::string, uint16_t >;
                using joint_bind_poses_map      = std::unordered_map< std::string, aiMatrix4x4 >;
                using joint_local_transforms    = std::unordered_map< std::string, aiMatrix4x4 >;

                inline gx::import::geo::joint_transform build_transform(const aiMatrix4x4& m )
                {
                    gx::import::geo::joint_transform r;

                    auto m0 = m;
                    m0.Transpose(); // ? or inverse?

                    aiQuaternion q;
                    aiVector3D   t;
                    aiVector3D   s;

                    m.Decompose(s, q, t);

                    //todo: check this for normalized quaternions and scale
                    r.m_rotation    = math::set(q.x, q.y, q.z, q.w );
                    r.m_translation = math::set(t.x, t.y, t.z, 1.0f );

                    return r;
                }

                inline gx::import::geo::joint_transform_matrix build_transform_matrix(const aiMatrix4x4& m)
                {
                    gx::import::geo::joint_transform_matrix r;

                    r.m_transform.r[0] = math::load4u(&m.a1);
                    r.m_transform.r[1] = math::load4u(&m.b1);
                    r.m_transform.r[2] = math::load4u(&m.c1);
                    r.m_transform.r[3] = math::load4u(&m.d1);
                    r.m_transform      = math::transpose(r.m_transform);
                    return r;
                }

                inline joint_indices_map build_node_indices(const gx::import::geo::skeleton& s)
                {
                    joint_indices_map r;

                    for (uint16_t i = 0U; i < static_cast<uint16_t>( s.m_joints.size() ); ++i)
                    {
                        r.insert( std::make_pair(s.m_joints[i].m_name, i));
                    }

                    return r;
                }

                inline auto blend_weight( const aiMesh* mesh )
                {
                    using blend_weight_t = gx::import::geo::skinned_mesh::blend_weight_t;

                    std::vector< std::vector< joint_influence > > influences;

                    influences.resize( mesh->mNumVertices );

                    std::vector< blend_weight_t > weights;
                    weights.resize( mesh->mNumVertices );

                    for ( auto i = 0U; i < mesh->mNumBones; ++i )
                    {
                        const auto joint = mesh->mBones[i];

                        for ( auto w = 0U; w < joint->mNumWeights; ++w )
                        {
                            auto&& v = joint->mWeights[w];
                            influences[v.mVertexId].push_back( v.mWeight );
                        }
                    }

                    for ( auto i = 0U; i < mesh->mNumVertices; ++i )
                    {
                        float* w = reinterpret_cast<float*> (&weights[i]);
                        auto s = influences[i].size();
                        s = std::min<size_t>(s, 4);
                        assert( s < 5);

                        for (auto j = 0U; j < s; ++j)
                        {
                            w[j] = influences[i].at(j);
                        }
                    }

                    return weights;
                }

                inline auto blend_index(const gx::import::geo::skeleton& s, const aiMesh* mesh)
                {
                    using blend_index_t = gx::import::geo::skinned_mesh::blend_index_t;

                    std::vector< std::vector< joint_index > > influences;
                    influences.resize(mesh->mNumVertices);

                    std::vector< blend_index_t > indices;
                    indices.resize( mesh->mNumVertices );

                    {
                        auto map = build_node_indices(s);

                        for (auto i = 0U; i < mesh->mNumBones; ++i)
                        {
                            const auto joint = mesh->mBones[i];
                            const auto id = map.find(joint->mName.C_Str())->second;

                            for (auto w = 0U; w < joint->mNumWeights; ++w)
                            {
                                auto&& v = joint->mWeights[w];
                                influences[v.mVertexId].push_back(static_cast<uint32_t>(id));  //skeleton bone ids are unique, not per mesh
                            }
                        }
                    }

                    for (auto i = 0U; i < mesh->mNumVertices; ++i)
                    {
                        uint16_t* w = reinterpret_cast<uint16_t*> (&indices[i]);

                        auto influences_size = influences[i].size();
                        influences_size = std::min<size_t>(influences_size, 4); //some bones come with 5 weights
                        

                        for (auto j = 0U; j < influences_size; ++j)
                        {
                            w[j] = influences[i].at(j);
                        }
                    }

                    return indices;
                }

                inline auto blend_weights(const gsl::span<aiMesh*> meshes)
                {
                    using blend_weights_t          = gx::import::geo::skinned_mesh::blend_weights_t;
                    std::vector< blend_weights_t > vertices;
                    vertices.resize(meshes.size());

                    concurrency::parallel_for( 0U, static_cast<uint32_t>(meshes.size()), [&vertices, &meshes](uint32_t i)
                    {
                        vertices[i] = blend_weight(meshes[i]);
                    });

                    return vertices;
                }

                inline auto blend_indices(const gx::import::geo::skeleton& s, const gsl::span<aiMesh*> meshes)
                {
                    using blend_indices_t = gx::import::geo::skinned_mesh::blend_indices_t;
                    std::vector< blend_indices_t > vertices;
                    vertices.resize(meshes.size());

                    concurrency::parallel_for(0U, static_cast<uint32_t>(meshes.size()), [&s, &vertices, &meshes](uint32_t i)
                    {
                        vertices[i] = blend_index(s, meshes[i]);
                    });

                    return vertices;
                }

                inline void build_nodes( const aiNode* p, std::unordered_map< std::string, const aiNode*>& node_map )
                {
                    auto key = std::string(p->mName.C_Str());
                    node_map.insert_or_assign(key, p);

                    for (auto i = 0U; i < p->mNumChildren; ++i)
                    {
                        build_nodes(p->mChildren[i], node_map);
                    }
                }

                auto build_nodes_names( const aiScene* s)
                {
                    nodes_names_map names;
                    build_nodes(s->mRootNode, names);
                    return names;
                }

                inline auto build_joint_indices( const gsl::span<aiMesh*> meshes, const nodes_names_map& scene_nodes)
                {
                    joint_indices_map r;
                    uint16_t index = 0;

                    auto size = meshes.size();

                    for ( auto i = 0; i < size; ++i )
                    {
                        auto mesh   = meshes[i];
                        auto bc     = mesh->mNumBones;

                        for ( auto j = 0U; j < bc; ++j )
                        {
                            auto node = scene_nodes.find( std::string(mesh->mBones[j]->mName.C_Str()) );

                            while ( node != scene_nodes.end() )
                            {
                                auto&& name     = node->first;
                                auto&& ai_node  = node->second;

                                if (r.find(name) == r.end())
                                {
                                    r.insert(std::make_pair(name, index++));
                                }

                                auto parent     = ai_node->mParent;
                                
                                if (parent)
                                {
                                    node = scene_nodes.find(std::string(parent->mName.C_Str()));
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }

                    //sort by the lognest depth

                    return r;
                }

                inline auto build_joint_inverse_bind_poses( const gsl::span<aiMesh*> meshes )
                {
                    joint_bind_poses_map r;

                    auto size = meshes.size();

                    for (auto i = 0; i < size; ++i)
                    {
                        auto mesh = meshes[i];
                        auto bc = mesh->mNumBones;

                        for (auto j = 0U; j < bc; ++j)
                        {
                            auto joint  = mesh->mBones[j];
                            auto name   = std::string(joint->mName.C_Str());
                            auto node   = r.find(name);

                            if ( node == r.end())
                            {
                                r.insert(std::make_pair( name, joint->mOffsetMatrix) );
                            }
                            else
                            {
                                aiMatrix4x4 old = node->second;
                                //inverse bind poses must match
                                assert(old == joint->mOffsetMatrix);
                            }
                        }
                    }

                    return r;
                }

                inline auto build_joint_local_transforms(const nodes_names_map& s, const gsl::span<aiMesh*> meshes)
                {
                    joint_local_transforms r;
                    //size_t index = 0;

                    auto size = meshes.size();

                    for (auto i = 0; i < size; ++i )
                    {
                        auto mesh = meshes[i];
                        auto bc = mesh->mNumBones;

                        for (auto j = 0U; j < bc; ++j)
                        {
                            auto name = std::string( mesh->mBones[j]->mName.C_Str() );
                            auto node = r.find(name);

                            if (node == r.end())
                            {
                                auto n = s.find(name)->second;
                                r.insert(std::make_pair( name, n->mTransformation) );
                            }
                        }
                    }

                    return r;
                }

                inline auto build_parent_index( const aiNode* n, const joint_indices_map& joints)
                {
                    uint16_t parent_index = 0xFFFF;
                    if (n->mParent)
                    {
                        parent_index = joints.find(n->mParent->mName.C_Str())->second;
                    }

                    return parent_index;
                }

                template <typename map>
                inline auto build_matrix(const std::string& joint_name, const map& joints)
                {
                    aiMatrix4x4 r;

                    {
                        auto it = joints.find(joint_name);

                        if (it != joints.end())
                        {
                            r = it->second;
                        }
                    }

                    return r;
                }

                inline auto build_inverse_bind_pose_matrix(const std::string& joint_name, const joint_bind_poses_map& joints)
                {
                    return build_matrix<joint_bind_poses_map>(joint_name, joints);
                }
                
                inline auto build_local_transform_matrix(const std::string& joint_name, const joint_local_transforms& joints)
                {
                    return build_matrix<joint_local_transforms>(joint_name, joints);
                }


                inline auto skeletons( const aiScene* scene, const gsl::span<aiMesh*> meshes )
                {
                    auto nodes_map                  = build_nodes_names(scene);
                    auto joints_map                 = build_joint_indices(meshes, nodes_map);
                    auto joint_inverse_transforms   = build_joint_inverse_bind_poses(meshes);
                    
                    gx::import::geo::skeleton skeleton;
                    skeleton.m_joints.resize(joints_map.size() );

                    for ( auto&& joint : joints_map)
                    {
                        auto joint_node          = nodes_map.find(joint.first)->second;

                        auto joint_index        = joint.second;

                        auto&& b                = skeleton.m_joints[joint_index];

                        auto inverse_bind_pose  = build_inverse_bind_pose_matrix( joint.first, joint_inverse_transforms );

                        b.m_inverse_bind_pose   = build_transform( inverse_bind_pose );
                        b.m_inverse_bind_pose2  = build_transform_matrix( inverse_bind_pose );
                        b.m_parent_index        = build_parent_index( joint_node, joints_map );
                        b.m_name                = joint_node->mName.C_Str();
                    }

                    return skeleton;
                }

                inline auto skeleton_poses(const aiScene* scene, const gsl::span<aiMesh*> meshes)
                {
                    auto nodes_map                = build_nodes_names(scene);
                    auto joints_map               = build_joint_indices(meshes, nodes_map);
                    auto joint_inverse_transforms = build_joint_inverse_bind_poses(meshes);
                    auto joint_local_transforms   = build_joint_local_transforms(nodes_map, meshes);

                    gx::import::geo::skeleton_pose pose;
                    pose.m_skeleton.m_joints.resize(joints_map.size());

                    for (auto&& joint : joints_map)
                    {
                        auto joint_node = nodes_map.find(joint.first)->second;

                        auto joint_index = joint.second;

                        auto&& b = pose.m_skeleton.m_joints[joint_index];
                        b.m_inverse_bind_pose               = build_transform(build_inverse_bind_pose_matrix(joint.first, joint_inverse_transforms));
                        b.m_inverse_bind_pose2              = build_transform_matrix(build_inverse_bind_pose_matrix(joint.first, joint_inverse_transforms));
                        b.m_parent_index                    = static_cast<int16_t> (build_parent_index(joint_node, joints_map));
                        b.m_name                            = joint_node->mName.C_Str();
                    }

                    pose.m_joint_local_pose.resize( joints_map.size() );

                    for (auto&& joint : joints_map)
                    {
                        auto joint_index        = joint.second;
                        auto transform          = build_local_transform_matrix(joint.first, joint_local_transforms);
                        
                        gx::import::geo::local_joint_pose l;
                        
                        l.m_transform           = build_transform(transform);
                        l.m_transform_matrix    = build_transform_matrix(transform);

                        pose.m_joint_local_pose[joint_index] = l;
                    }

                    return pose;
                }

                inline std::unique_ptr< gx::import::geo::skinned_mesh > create_skinned_mesh(const std::string& file_name, uint32_t import_flags = aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FindInvalidData)
                {
                    auto scene = load_assimp_scene(file_name, import_flags);
                    validate_scene(scene.get(), validation_option::meshes);

                    auto meshes = gsl::make_span(scene->mMeshes, scene->mNumMeshes);
                    auto v = vertices(meshes);
                    auto uv = uvs(meshes);
                    auto face = faces(meshes);
                    auto normal = normals(meshes);
                    auto tangent = tangents(meshes);
                    auto mat = materials(scene.get(), meshes);
                    auto weights = blend_weights(meshes);
                    auto poses = skeleton_poses(scene.get(), meshes);
                    auto indices = blend_indices(poses.m_skeleton, meshes);


                    return std::make_unique<gx::import::geo::skinned_mesh>(std::move(v), std::move(normal), std::move(tangent), std::move(uv), std::move(face), std::move(mat), std::move(weights), std::move(indices), std::move(poses));
                }
              
            }
        }
    }

}







