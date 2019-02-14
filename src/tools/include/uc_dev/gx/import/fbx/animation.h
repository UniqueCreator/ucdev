#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>

#include <uc_dev/gx/import/anm/animation.h>
#include <uc_dev/gx/import/fbx/fbx_common.h>
#include <uc_dev/gx/import/fbx/fbx_transform_helper.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace fbx
            {

                inline fbxsdk::FbxAnimStack* get_anim_stack(const fbxsdk::FbxScene* s)
                {
                    // merge animation layer at first
                    return  s->GetMember<fbxsdk::FbxAnimStack>(0);
                }

                inline fbxsdk::FbxAnimLayer* get_anim_layer(const fbxsdk::FbxAnimStack* s, int32_t layer_index )
                {
                    return reinterpret_cast<fbxsdk::FbxAnimLayer*>(s->GetMember(layer_index));
                }

                inline std::vector<fbxsdk::FbxNode*> get_skeleton_nodes(const fbxsdk::FbxNode* n)
                {
                    std::vector<fbxsdk::FbxNode*> r;

                    transform_node_recursive( n, [&r](const fbxsdk::FbxNode* n)
                    {
                        if (n->GetNodeAttribute() && n->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
                        {
                            r.push_back(const_cast<fbxsdk::FbxNode*>(n));
                        }
                    });

                    return r;
                }
                
                struct fbx_joint_animation
                {
                    fbxsdk::FbxNode*                m_node;
                    std::vector<fbxsdk::FbxAMatrix> m_transforms;
                    std::vector<double>             m_transforms_time;
                };

                struct fbx_joint_animation_relative
                {
                    fbxsdk::FbxNode*                m_node;
                    std::vector<fbxsdk::FbxAMatrix> m_transforms;
                    std::vector<double>             m_transforms_time;
                    std::vector<bool>               m_conjugate_quaternion;
                };


                inline std::vector<uint32_t> build_up_hierarchy( const std::vector<fbxsdk::FbxNode*> nodes)
                {
                    std::vector<uint32_t> r;
                    std::map< fbxsdk::FbxNode*, uint16_t>    joint2index;

                    {
                        r.resize(nodes.size());
                        for (auto&& i = 0U; i < nodes.size(); ++i)
                        {
                            auto&& node = nodes[i];

                            joint2index.insert(std::make_pair(node, static_cast<uint16_t>(i)));
                        }
                    }

                    //build up joint hierarchy, root node has parent 0xffff
                    for (auto&& i = 0U; i< nodes.size(); ++i)
                    {
                        const auto&& parent         = nodes[i]->GetParent();
                        const auto&& parent_index   = joint2index.find(parent);
                        
                        if (parent_index != joint2index.end())
                        {
                            r[i] = parent_index->second;
                        }
                        else
                        {
                            r[i] = 0xFFFF;
                        }

                    }

                    return r;
                }

                inline anm::joint_rotation_key convert_to_joint_rotation_key(const fbxsdk::FbxQuaternion q, double time)
                {
                    anm::joint_rotation_key r;
                    r.m_time = time;

                    const double* d = q;
                    float temp[4];

                    temp[0] = static_cast<float>(*d);
                    temp[1] = static_cast<float>(*(d + 1));
                    temp[2] = static_cast<float>(*(d + 2));
                    temp[3] = static_cast<float>(*(d + 3));

                    r.m_transform = math::load4u(&temp[0]);
                    return r;
                }

                inline anm::joint_rotation_key convert_to_joint_rotation_key(math::afloat4 q, double time)
                {
                    anm::joint_rotation_key r;
                    r.m_time = time;
                    r.m_transform = q;
                    return r;
                }

                inline anm::joint_translation_key convert_to_joint_translation_key(const fbxsdk::FbxVector4 q, double time)
                {
                    anm::joint_translation_key r;
                    r.m_time = time;

                    const double* d = q;
                    float temp[4];

                    temp[0] = static_cast<float>(*d);
                    temp[1] = static_cast<float>(*(d + 1));
                    temp[2] = static_cast<float>(*(d + 2));
                    temp[3] = static_cast<float>(*(d + 3));

                    r.m_transform = math::load4u(&temp[0]);
                    return r;
                }

                inline anm::joint_translation_key convert_to_joint_translation_key(math::afloat4 q, double time)
                {
                    anm::joint_translation_key r;
                    r.m_time = time;
                    r.m_transform = q;
                    return r;
                }


                inline std::vector<fbxsdk::FbxNode*> get_meshes(const fbxsdk::FbxNode* n)
                {
                    std::vector<fbxsdk::FbxNode*> r;

                    transform_node_recursive(n, [&r](const fbxsdk::FbxNode* n)
                    {
                        if (n->GetNodeAttribute() && n->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
                        {
                            r.push_back(const_cast<fbxsdk::FbxNode*>(n));
                        }
                    });

                    return r;
                }

                inline bool is_skinned_mesh(const fbxsdk::FbxMesh* mesh)
                {
                    return mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin) > 0;
                }

                inline std::vector<anm::joint_animations> create_animations(const std::string& file_name)
                {
                    auto context    = load_fbx_file(file_name);
                    auto scene      = context->m_scene.get();
                    auto importer   = context->m_importer.get();

                    auto anim_stack_count = importer->GetAnimStackCount();

                    if (anim_stack_count == 0)
                    {
                        ::uc::gx::throw_exception<uc::gx::fbx_exception>("No Animations found");
                    }

                    fbxsdk::FbxTakeInfo* take_info = importer->GetTakeInfo(0);

                    anm::joint_animations m;
                    m.m_name = take_info->mName;
                    double frame_rate;

                    {
                        fbxsdk::FbxTime duration    = take_info->mLocalTimeSpan.GetDuration();
                        auto        mode            = duration.GetGlobalTimeMode();
                        frame_rate                  = duration.GetFrameRate(mode);
                        m.m_ticks_per_second        = frame_rate;
                        m.m_duration                = duration.GetSecondDouble() * frame_rate;
                    }

                    auto joints = get_skeleton_nodes(scene->GetRootNode());
                    auto meshes = get_meshes(scene->GetRootNode());
                    std::vector< fbx_joint_animation > joint_animations;

                    //sample the bones
                    {
                        auto evaluator = scene->GetAnimationEvaluator();
                        joint_animations.resize(joints.size());

                        const auto   duration   = take_info->mLocalTimeSpan.GetDuration();
                        const double delta_time = duration.GetSecondDouble() / m.m_ticks_per_second;
                        const double start_time = take_info->mLocalTimeSpan.GetStart().GetSecondDouble();
                        const double end_time   = take_info->mLocalTimeSpan.GetStop().GetSecondDouble();

                        for (auto i = 0U; i < joints.size(); ++i)
                        {
                            auto&& n = joints[i];

                            joint_animations[i].m_node = n;

                            double   current_time = start_time;
                            while (current_time <= end_time)
                            {
                                fbxsdk::FbxTime t;
                                t.SetSecondDouble(current_time);

                                auto rot = n->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
                                auto tr  = n->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
                                auto sc  = n->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

                                fbxsdk::FbxAMatrix geometric_transform;
                                geometric_transform.SetTRS(tr, rot, sc);

                                auto transform = evaluator->GetNodeGlobalTransform(n, t) * geometric_transform;

                                joint_animations[i].m_transforms.push_back(transform);
                                joint_animations[i].m_transforms_time.push_back(current_time * m.m_ticks_per_second);
                                current_time += delta_time;
                            }
                        }
                    }

                    //build up hierarchy
                    auto hierarchy = build_up_hierarchy(joints);
                    std::vector< fbx_joint_animation_relative > joint_animations_relative; //relative child to parent transforms, since the engine calculates them this way

                    joint_animations_relative.resize(joint_animations.size());
                    //convert to parent/child relation ship
                    for (auto i = 0U; i < joint_animations.size(); ++i)
                    {
                        auto&& a = joint_animations[i];
                        auto&& parent_index = hierarchy[i];
                        joint_animations_relative[i].m_node = a.m_node;

                        for (auto j = 0; j < a.m_transforms.size(); ++j)
                        {
                            fbxsdk::FbxAMatrix parent;
                            parent.SetIdentity();

                            if (parent_index != 0xFFFF)
                            {
                                parent = joint_animations[parent_index].m_transforms[j];
                            }

                            fbxsdk::FbxAMatrix this_ = a.m_transforms[j];
                            fbxsdk::FbxAMatrix relative = parent.Inverse() * this_;

                            joint_animations_relative[i].m_transforms.push_back(relative);
                            joint_animations_relative[i].m_transforms_time.push_back(a.m_transforms_time[j]);
                            joint_animations_relative[i].m_conjugate_quaternion.push_back(false);
                        }
                    }

                    //mark the rotations to move to the shortest path
                    for (auto i = 0U; i < joint_animations_relative.size(); ++i)
                    {
                        auto&& a = joint_animations_relative[i];
                        auto&& parent_index = hierarchy[i];

                        for (auto j = 0; j < a.m_transforms.size(); ++j)
                        {
                            fbxsdk::FbxAMatrix parent;
                            parent.SetIdentity();

                            if (parent_index != 0xFFFF)
                            {
                                parent = joint_animations[parent_index].m_transforms[j];
                            }

                            fbxsdk::FbxAMatrix this_ = a.m_transforms[j];

                            auto q0 = parent.GetQ();
                            auto r0 = this_.GetQ();

                            if ( q0.DotProduct(r0) < 0 )
                            { 
                                a.m_conjugate_quaternion[j] = true;
                            }
                        }
                    }

                    m.m_joint_animations.resize(joint_animations_relative.size());
                    for (auto i = 0U; i < joint_animations_relative.size(); ++i)
                    {
                        auto&& a = m.m_joint_animations[i];
                        a.m_joint_name = joint_animations_relative[i].m_node->GetName();

                        for ( auto j = 0; j < joint_animations_relative[i].m_transforms.size(); ++j )
                        { 

                            fbxsdk::FbxAMatrix transform    = joint_animations_relative[i].m_transforms[j];
                            double transform_time           = joint_animations_relative[i].m_transforms_time[j];
                            
                            auto trans                      = transform.GetT();
                            auto rot                        = transform.GetQ();

                            auto t                          = to_float4x4(transform);
                            
                            auto t0                         = transform_from_dcc(t, context.get());
                            auto rot0                       = math::rotation(t0);
                            auto trans0                     = math::translation(t0);
                            

                            if (joint_animations_relative[i].m_conjugate_quaternion[j])
                            {
                                //todo: check if this is working
                                //rot.Conjugate();
                                //rot = rot * -1.0;
                            }

                            //anm::joint_rotation_key     rot_key     = convert_to_joint_rotation_key(rot, transform_time);
                            //anm::joint_translation_key  trans_key   = convert_to_joint_translation_key(trans, transform_time);

                            anm::joint_rotation_key     rot_key     = convert_to_joint_rotation_key(math::quaternion_normalize(math::matrix_2_quaternion_simd(rot0)), transform_time);
                            anm::joint_translation_key  trans_key   = convert_to_joint_translation_key(trans0, transform_time);

                            a.m_rotation_keys.push_back(rot_key);
                            a.m_translation_keys.push_back(trans_key);
                        }
                    }

                    //export only the first animation for now
                    std::vector<anm::joint_animations> r;
                    r.push_back(std::move(m));
                    return r;
                }
            }
        }
    }
}







