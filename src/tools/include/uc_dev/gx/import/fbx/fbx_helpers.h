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
                inline fbxsdk::FbxAMatrix get_geometry(const fbxsdk::FbxNode* node)
                {
                    const fbxsdk::FbxVector4 lT = node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
                    const fbxsdk::FbxVector4 lR = node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
                    const fbxsdk::FbxVector4 lS = node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

                    return fbxsdk::FbxAMatrix(lT, lR, lS);
                }

                // Get the matrix of the given pose
                fbxsdk::FbxAMatrix get_pose_matrix(const fbxsdk::FbxPose* pose, int32_t node)
                {
                    FbxAMatrix lPoseMatrix;
                    FbxMatrix lMatrix = pose->GetMatrix(node);

                    memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

                    return lPoseMatrix;
                }

                // Get the global position of the node for the current pose.
                // If the specified node is not part of the pose or no pose is specified, get its
                // global position at the current time.
                fbxsdk::FbxAMatrix get_global_position(const fbxsdk::FbxNode* node, const fbxsdk::FbxTime& pTime, const fbxsdk::FbxPose* pose = nullptr, const fbxsdk::FbxAMatrix* parent_global_position = nullptr)
                {
                    FbxAMatrix lGlobalPosition;
                    bool        lPositionFound = false;

                    if (pose)
                    {
                        int32_t node_index = pose->Find(node);

                        if (node_index > -1)
                        {
                            // The bind pose is always a global matrix.
                            // If we have a rest pose, we need to check if it is
                            // stored in global or local space.
                            if (pose->IsBindPose() || !pose->IsLocalMatrix(node_index))
                            {
                                lGlobalPosition = get_pose_matrix(pose, node_index);
                            }
                            else
                            {
                                // We have a local matrix, we need to convert it to
                                // a global space matrix.
                                FbxAMatrix lParentGlobalPosition;

                                if (parent_global_position)
                                {
                                    lParentGlobalPosition = *parent_global_position;
                                }
                                else
                                {
                                    if (node->GetParent())
                                    {
                                        lParentGlobalPosition = get_global_position(node->GetParent(), pTime, pose);
                                    }
                                }

                                FbxAMatrix lLocalPosition = get_pose_matrix(pose, node_index);
                                lGlobalPosition = lParentGlobalPosition * lLocalPosition;
                            }

                            lPositionFound = true;
                        }
                    }

                    if (!lPositionFound)
                    {
                        // There is no pose entry for that node, get the current global position instead.

                        // Ideally this would use parent global position and local position to compute the global position.
                        // Unfortunately the equation 
                        //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
                        // does not hold when inheritance type is other than "Parent" (RSrs).
                        // To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
                        lGlobalPosition = const_cast<fbxsdk::FbxNode*>(node)->EvaluateGlobalTransform(pTime);
                    }

                    return lGlobalPosition;
                }
            }
        }
    }
}


