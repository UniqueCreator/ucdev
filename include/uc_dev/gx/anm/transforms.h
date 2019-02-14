#pragma once

#include <uc_dev/gx/lip/animation.h>
#include <uc_dev/gx/structs.h>
#include <uc_dev/math/quaternion.h>
#include <uc_dev/math/graphics.h>

namespace uc
{
    namespace gx
    {
        namespace anm
        {
            using joint_transform = lip::joint_transform;
            using skeleton        = lip::skeleton;
            using joint_linkage   = lip::joint_linkage;

            std::vector< joint_transform >  local_to_world_joints(const joint_transform* local_joints, const joint_linkage* linkages, const uint16_t*, uint32_t joint_linkage_count, uint32_t joint_count);
            std::vector< joint_transform >  local_to_world_joints(const lip::skeleton* s);
            math::float4x4 global_transform(const lip::skeleton* s, uint32_t index, math::afloat4x4 locomotion_transform = math::identity_matrix());
            math::float4x4 global_transform(const lip::skeleton* s, const std::vector<math::float4x4>& local_transforms, uint32_t index, math::afloat4x4 locomotion_transform = math::identity_matrix());

            //concatenates all transforms from the root to the children
            std::vector< lip::matrix4x4 >  local_to_world_joints2(const lip::skeleton* s, math::afloat4x4 locomotion_transform = math::identity_matrix());

            //concatenates all transforms from the root to the children
            std::vector< math::float4x4 >  local_to_world_joints2(const lip::skeleton* s, const std::vector<math::float4x4>& local_transforms, math::afloat4x4 locomotion_transform = math::identity_matrix());
            std::vector< gx::position_3d > skeleton_positions(const lip::skeleton* skeleton, const std::vector< math::float4x4>& local_transforms, math::afloat4x4 locomotion_transform = math::identity_matrix());
        }
    }
}
