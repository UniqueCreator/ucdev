#include "pch.h"

#include <uc_dev/gx/anm/transforms.h>


namespace uc {
    namespace gx {
        namespace anm {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            std::vector< joint_transform >  local_to_world_joints(const joint_transform* local_joints, const joint_linkage* linkages, const uint16_t*, uint32_t joint_linkage_count, uint32_t joint_count)
            {
                std::vector< joint_transform >  r;

                r.resize(joint_count);

                math::float4 root_joint_rotation = math::set(0, 0, 0, 1);
                math::float4 root_joint_translation = math::set(0, 0, 0, 0);

                auto simd_joints_count = (joint_linkage_count + 7U) / 8U;

                for (auto i = 0U; i < simd_joints_count; ++i)
                {
                    for (auto j = 0U; j < 8; ++j)
                    {
                        auto linkage_index = i * 8 + j;
                        auto linkage = linkages[linkage_index];

                        math::float4    parent_joint_rotation = linkage.m_parent == -1 ? root_joint_rotation : math::load4(&local_joints[linkage.m_parent].m_rotation);
                        math::float4    parent_joint_translation = linkage.m_parent == -1 ? root_joint_translation : math::load3(&local_joints[linkage.m_parent].m_translation_scale.m_translation);

                        math::float4    child_joint_rotation = math::load4(&local_joints[linkage.m_joint].m_rotation);
                        math::float4    child_joint_translation = math::load3(&local_joints[linkage.m_joint].m_translation_scale.m_translation);

                        math::float4    final_rotation = math::quaternion_normalize(math::quaternion_mul(parent_joint_rotation, child_joint_rotation));
                        math::float4    final_translation = math::add(parent_joint_translation, math::rotate_vector3(child_joint_translation, parent_joint_rotation));

                        math::store4(&r[linkage.m_joint].m_rotation, final_rotation);
                        math::store4(&r[linkage.m_joint].m_translation_scale, final_translation);
                    }
                }

                return r;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            std::vector< joint_transform >  local_to_world_joints(const lip::skeleton* s)
            {
                auto t = local_to_world_joints(&s->m_joint_local_transforms[0], &s->m_joint_linkage[0], &s->m_joint_linkage_indices[0], static_cast<uint32_t>(s->m_joint_linkage.size()), static_cast<uint32_t>(s->joint_count()));


                joint_transform root;

                root.m_rotation.m_transform.m_x = 0;
                root.m_rotation.m_transform.m_y = 0;
                root.m_rotation.m_transform.m_z = 0;
                root.m_rotation.m_transform.m_w = 1;

                root.m_translation_scale.m_translation.m_x = 0;
                root.m_translation_scale.m_translation.m_y = 0;
                root.m_translation_scale.m_translation.m_z = 0;

                root.m_translation_scale.m_scale = 1.0f;

                std::vector<joint_transform> r;
                r.resize(s->joint_count() * 2);

                for (auto i = 0U; i < s->joint_count(); ++i)
                {
                    auto& p = r[2 * i];
                    auto& j = r[2 * i + 1];

                    auto linkage_index = s->m_joint_linkage_indices[i];
                    auto linkage = s->m_joint_linkage[linkage_index];

                    auto joint_transform_parent = linkage.m_parent == -1 ? root : t[linkage.m_parent];
                    auto joint_transform_child = t[linkage.m_joint];

                    j = joint_transform_parent;
                    p = joint_transform_child;

                }
                return r;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            math::float4x4 global_transform(const lip::skeleton* s, uint32_t index, math::afloat4x4 locomotion_transform)
            {
                auto local_transform = math::load44(&s->m_joint_local_transforms2[index].m_a0);
                auto parent_index = s->m_joint_linkage2[index];

                while (parent_index != 0xFFFF)
                {
                    auto parent_transform = math::load44(&s->m_joint_local_transforms2[parent_index].m_a0);
                    local_transform = math::mul(local_transform, parent_transform);
                    parent_index = s->m_joint_linkage2[parent_index];
                }

                if (parent_index == 0xFFFF)
                {
                    local_transform = math::mul(local_transform, locomotion_transform);
                }

                return local_transform;// math::transpose(local_transform);
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            math::float4x4 global_transform(const lip::skeleton* s, const std::vector<math::float4x4>& local_transforms, uint32_t index, math::afloat4x4 locomotion_transform)
            {
                auto local_transform = local_transforms[index];
                auto parent_index = s->m_joint_linkage2[index];

                while (parent_index != 0xFFFF)
                {
                    auto parent_transform = local_transforms[parent_index];
                    local_transform = math::mul(local_transform, parent_transform);
                    parent_index = s->m_joint_linkage2[parent_index];
                }

                if (parent_index == 0xFFFF)
                {
                    local_transform = math::mul(local_transform, locomotion_transform);
                }

                return local_transform;// math::transpose(local_transform);
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //concatenates all trasnforms from the root to the children
            std::vector< lip::matrix4x4 >  local_to_world_joints2(const lip::skeleton* s, math::afloat4x4 locomotion_transform)
            {
                std::vector< lip::matrix4x4 > r;

                auto joint_size = s->m_joint_local_transforms2.size();
                r.resize(joint_size);

                for (auto i = 0U; i < joint_size; ++i)
                {
                    auto m = global_transform(s, i, locomotion_transform);
                    math::store44(&r[i].m_a0, m);
                }

                return r;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //concatenates all trasnforms from the root to the children
            std::vector< math::float4x4 >  local_to_world_joints2(const lip::skeleton* s, const std::vector<math::float4x4>& local_transforms, math::afloat4x4 locomotion_transform)
            {
                std::vector< math::float4x4> r;

                auto joint_size = local_transforms.size();
                r.resize(joint_size);

                for (auto i = 0U; i < joint_size; ++i)
                {
                    r[i] = global_transform(s, local_transforms, i, locomotion_transform);
                }

                return r;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            std::vector< gx::position_3d > skeleton_positions(const lip::skeleton* skeleton, const std::vector< math::float4x4>& local_transforms, math::afloat4x4 locomotion_transform)
            {
                using position = gx::position_3d;

                auto joints = gx::anm::local_to_world_joints2(skeleton, local_transforms, locomotion_transform);
                std::vector<position> v;
                v.reserve(joints.size());

                //form lines from parent to child
                //swap y and z, since the coordinate system is not the same (for now)
                for (auto i = 0U; i < joints.size(); ++i)
                {
                    auto parent_index = skeleton->m_joint_linkage2[i];

                    if (parent_index != 0xFFFF)
                    {
                        auto pm = math::load4u(&joints[parent_index].r[3]);
                        auto cm = math::load4u(&joints[i].r[3]);

                        //swap z and y, todo: do this offline
                        //pm = math::swizzle<math::x, math::z, math::y, math::w>(pm);
                        //cm = math::swizzle<math::x, math::z, math::y, math::w>(cm);

                        gx::position_3d parent;
                        gx::position_3d child;

                        math::store3u(reinterpret_cast<float*>(&parent), pm);
                        math::store3u(reinterpret_cast<float*>(&child), cm);

                        v.push_back(parent);
                        v.push_back(child);
                    }
                }

                return v;
            }
        }
    }
}