#pragma once

#include <tuple>
#include <uc_dev/gx/error.h>
#include <uc_dev/math/math.h>

#include <fbxsdk.h>
namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace fbx
            {
                class exception : std::exception
                {
                    using base = std::exception;

                public:
                    exception(const char* message) : base(message)
                    {

                    }


                };

                template <typename t>
                struct fbxsdk_object_deleter
                {
                    inline void operator()(t* m) const
                    {
                        m->Destroy();
                    }
                };

                using fbxmanager_deleter = fbxsdk_object_deleter < FbxManager >;
                using fbxscene_deleter = fbxsdk_object_deleter < FbxScene >;
                using fbximporter_deleter = fbxsdk_object_deleter < FbxImporter >;

                /// Do this setup for each node(FbxNode).
                // We set up what we want to bake via ConvertPivotAnimationRecursive.
                // When the destination is set to 0, baking will occur.
                // When the destination value is set to the source’s value, the source values will be retained and not baked.
                inline void bake_node_transforms(fbxsdk::FbxNode* node)
                {
                    fbxsdk::FbxVector4 lZero(0, 0, 0);

                    // Activate pivot converting
                    node->SetPivotState(fbxsdk::FbxNode::eSourcePivot, fbxsdk::FbxNode::ePivotActive);
                    node->SetPivotState(fbxsdk::FbxNode::eDestinationPivot, fbxsdk::FbxNode::ePivotActive);

                    // We want to set all these to 0 and bake them into the transforms.
                    node->SetPostRotation(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetPreRotation(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetRotationOffset(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetScalingOffset(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetRotationPivot(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetScalingPivot(fbxsdk::FbxNode::eDestinationPivot, lZero);

                    node->SetRotationOrder(fbxsdk::FbxNode::eDestinationPivot, fbxsdk::FbxEuler::eOrderXYZ);

                    // Similarly, this is the case where geometric transforms are supported by the system.
                    // If geometric transforms are not supported, set them to zero instead of
                    // the source’s geometric transforms.
                    // Geometric transform = local transform, not inherited by children.
                    node->SetGeometricTranslation(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetGeometricRotation(fbxsdk::FbxNode::eDestinationPivot, lZero);
                    node->SetGeometricScaling(fbxsdk::FbxNode::eDestinationPivot, lZero);

                    node->SetQuaternionInterpolation(fbxsdk::FbxNode::eDestinationPivot, fbxsdk::eQuatInterpSlerp);
                }

                template <typename f> inline void transform_node_recursive(const fbxsdk::FbxNode* root, f op)
                {
                    if (root)
                    {
                        op(root);
                        auto children = root->GetChildCount();

                        for (auto i = 0; i < children; ++i)
                        {
                            auto child = root->GetChild(i);
                            transform_node_recursive(child, op);
                        }
                    }
                }

                inline math::float4x4 world_transform(fbxsdk::FbxNode* node)
                {
                    using namespace fbxsdk;

                    auto translation = node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
                    auto rotation = node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
                    auto scale = node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

                    fbxsdk::FbxAMatrix geometryTransform;
                    geometryTransform.SetTRS(translation, rotation, scale);

                    fbxsdk::FbxAMatrix globalTransform = node->EvaluateGlobalTransform();

                    fbxsdk::FbxAMatrix finalTransform;
                    finalTransform = globalTransform * geometryTransform;

                    float m[16];
                    double* d = finalTransform;

                    for (auto i = 0; i < 16; ++i)
                    {
                        m[i] = static_cast<float>(*d);
                        d++;

                        if (std::fabsf(m[i]) < 0.0000001f)
                        {
                            m[i] = 0.0f;
                        }
                    }

                    return math::transpose(math::load44u(&m[0]));
                }

                inline std::tuple<fbxsdk::FbxMesh*, fbxsdk::FbxNode*> get_mesh(fbxsdk::FbxNode* node)
                {
                    if (node)
                    {
                        auto&& att = node->GetNodeAttribute();

                        if (att != nullptr)
                        {
                            auto type = att->GetAttributeType();
                            switch (type)
                            {
                            case fbxsdk::FbxNodeAttribute::eMesh:
                            {
                                auto&& m = node->GetMesh();
                                if (m && m->IsTriangleMesh())
                                {
                                    return std::make_tuple(m, node);
                                }
                            }
                            }
                        }

                        auto children = node->GetChildCount();
                        for (int i = 0; i < children; i++)
                        {
                            if (auto&& child = node->GetChild(i))
                            {
                                auto r = get_mesh(child);
                                if (std::get<0>(r))
                                {
                                    return r;
                                }
                            }
                        }
                    }

                    return std::make_tuple(nullptr, nullptr);
                }

                inline std::vector<fbxsdk::FbxMesh*> get_meshes(fbxsdk::FbxNode* node, std::vector<fbxsdk::FbxMesh*>& meshes)
                {
                    if (node)
                    {
                        auto&& att = node->GetNodeAttribute();

                        if (att != nullptr)
                        {
                            auto type = att->GetAttributeType();
                            switch (type)
                            {
                            case fbxsdk::FbxNodeAttribute::eMesh:
                            {
                                auto&& m = node->GetMesh();
                                if (m && m->IsTriangleMesh())
                                {
                                    meshes.push_back(m);
                                }
                            }
                            }
                        }

                        auto children = node->GetChildCount();
                        for (int i = 0; i < children; i++)
                        {
                            if (auto&& child = node->GetChild(i))
                            {
                                get_meshes(child, meshes);
                            }
                        }
                    }

                    return meshes;
                }

                struct fbx_context
                {
                    std::unique_ptr<fbxsdk::FbxManager, fbxmanager_deleter>     m_manager;
                    std::unique_ptr<fbxsdk::FbxScene, fbxscene_deleter>         m_scene;
                    std::unique_ptr<fbxsdk::FbxImporter, fbximporter_deleter>   m_importer;
                    bool                                                        m_coordinate_system_swap_y_z;
                    bool                                                        m_invert_handness;
                };

                inline std::unique_ptr<fbx_context> load_fbx_file(const std::string& file_name)
                {
                    std::unique_ptr<fbxsdk::FbxManager, fbxmanager_deleter>     manager(fbxsdk::FbxManager::Create(), fbxmanager_deleter());
                    std::unique_ptr<fbxsdk::FbxScene, fbxscene_deleter>         scene(fbxsdk::FbxScene::Create(manager.get(), ""), fbxscene_deleter());
                    std::unique_ptr<fbxsdk::FbxImporter, fbximporter_deleter>   importer(fbxsdk::FbxImporter::Create(manager.get(), ""), fbximporter_deleter());

                    auto f = file_name;

                    auto import_status = importer->Initialize(f.c_str(), -1, manager->GetIOSettings());

                    if (import_status == false)
                    {
                        auto status = importer->GetStatus();
                        auto error = status.GetErrorString();
                        ::uc::gx::throw_exception<uc::gx::fbx_exception>(error);
                    }

                    import_status = importer->Import(scene.get());
                    if (import_status == false)
                    {
                        auto status = importer->GetStatus();
                        auto error = status.GetErrorString();
                        ::uc::gx::throw_exception<uc::gx::fbx_exception>(error);
                    }

                    FbxGeometryConverter geometryConverter(manager.get());
                    geometryConverter.Triangulate(scene.get(), true);

                    fbxsdk::FbxAxisSystem scene_axis_system = scene->GetGlobalSettings().GetAxisSystem();
                    fbxsdk::FbxAxisSystem our_axis_system   = fbxsdk::FbxAxisSystem(fbxsdk::FbxAxisSystem::EPreDefinedAxisSystem::eMayaZUp);

                    if (scene_axis_system != our_axis_system)
                    {
                       our_axis_system.ConvertScene(scene.get());
                    }

                    fbxsdk::FbxSystemUnit units = scene->GetGlobalSettings().GetSystemUnit();
                    fbxsdk::FbxSystemUnit meters = fbxsdk::FbxSystemUnit::m;

                    if (units != fbxsdk::FbxSystemUnit::m)
                    {
                        //FbxSystemUnit::m.ConvertScene(scene.get());
                    }

                    auto r = std::make_unique<fbx_context>();
                    r->m_manager = std::move(manager);
                    r->m_scene = std::move(scene);
                    r->m_importer = std::move(importer);
                    {
                        int32_t sign;
                        r->m_coordinate_system_swap_y_z = scene_axis_system.GetUpVector(sign) == fbxsdk::FbxAxisSystem::eZAxis;
                        r->m_invert_handness            = true; //todo: from maya, always transform
                    }
                    return r;
                }
            }
        }
    }
}

