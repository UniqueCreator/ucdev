#pragma once

#include <uc_dev/gx/import/geo/multi_material_mesh.h>
#include <uc_dev/math/math.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace geo
            {
                struct joint_transform
                {
                    math::float4    m_rotation; //quaternion
                    math::float4    m_translation;
                };

                struct joint_transform_matrix
                {
                    math::float4x4 m_transform;
                };

                struct joint
                {
                    joint_transform         m_inverse_bind_pose;
                    joint_transform_matrix  m_inverse_bind_pose2;
                    std::string             m_name;
                    uint16_t                m_parent_index;
                };

                struct skeleton
                {
                    std::vector< joint >    m_joints;
                };

                struct local_joint_pose
                {
                    joint_transform        m_transform;
                    joint_transform_matrix m_transform_matrix;
                };

                struct global_joint_pose
                {
                    joint_transform m_transform;
                };

                struct skeleton_pose
                {
                    skeleton                        m_skeleton;
                    std::vector<local_joint_pose>   m_joint_local_pose; //aka rest pose
                };

                struct skinned_mesh : multi_material_mesh
                {
                    using base              = multi_material_mesh;
                    using position_t        = base::position_t;
                    using normal_t          = base::normal_t;
                    using uv_t              = base::uv_t;
                    using blend_weight_t    = storage_blend_weight;
                    using blend_index_t     = storage_blend_index;
                    using facet_            = base::face_t;

                    using positions_t       = base::positions_t;
                    using uvs_t             = base::uvs_t;
                    using normals_t         = base::normals_t;
                    using tangents_t        = base::tangents_t;
                    using faces_t           = base::faces_t;
                    

                    using skeleton_pose_t = skeleton_pose;

                    using blend_weights_t = std::vector<blend_weight_t>;
                    using blend_indices_t = std::vector<blend_index_t>;

                    public:

                    skinned_mesh
                    (
                        std::vector<positions_t>&& positions,
                        std::vector<normals_t>&& normals,
                        std::vector<tangents_t>&& tangents,
                        std::vector<uvs_t>&& uv,
                        std::vector<faces_t>&& faces,
                        std::vector<material>&& materials,
                        std::vector<blend_weights_t>&& weights,
                        std::vector<blend_indices_t>&& indices,
                        skeleton_pose_t &&             pose
                    ) : base(std::move(positions), std::move(normals), std::move(tangents), std::move(uv), std::move(faces), std::move(materials))
                        , m_blend_weights(std::move(weights))
                        , m_blend_indices(std::move(indices))
                        , m_skeleton_pose(pose)
                    {

                    }

                    std::vector<blend_weights_t> m_blend_weights;
                    std::vector<blend_indices_t> m_blend_indices;
                    skeleton_pose_t              m_skeleton_pose;
                };

                struct skinned_mesh_view : material_view<skinned_mesh>
                {
                    using base              = material_view<skinned_mesh>;

                    using positions_t       = base::positions_t;
                    using uvs_t             = base::uvs_t;
                    using normals_t         = base::normals_t;
                    using faces_t           = base::faces_t;
                    using material_t        = base::material;
                    using blend_weights_t   = skinned_mesh::blend_weights_t;
                    using blend_indices_t   = skinned_mesh::blend_indices_t;
                    using material_index    = base::material_index;

                    using position_t        = typename base::position_t;
                    using uv_t              = typename base::uv_t;
                    using normal_t          = base::normal_t;
                    using face_t            = typename base::face_t;
                    using blend_weight_t    = typename skinned_mesh::blend_weight_t;
                    using blend_index_t     = typename skinned_mesh::blend_index_t;

                    using skeleton_pose_t   = skinned_mesh::skeleton_pose_t;

                    const blend_weights_t& blend_weight(material_index idx) const
                    {
                        return m_mesh->m_blend_weights[index(idx)];
                    }

                    const blend_indices_t& blend_index(material_index idx) const
                    {
                        return m_mesh->m_blend_indices[index(idx)];
                    }
                };

                inline skinned_mesh_view skinned_mesh_material_view(const skinned_mesh* m, std::vector<uint32_t>&& indices)
                {
                    skinned_mesh_view v;
                    v.m_mesh = m;
                    v.m_indices = std::move(indices);
                    return v;
                }
            }
        }
    }
}







