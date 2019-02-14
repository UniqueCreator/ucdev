#include "pch.h"

#include <uc_dev/gx/lip/animation.h>

namespace uc
{
        namespace lip
        {
            LIP_BEGIN_DEFINE_RTTI(joint_time)
            LIP_RTTI_MEMBER(joint_time, m_time)
            LIP_END_DEFINE_RTTI(joint_time)

            LIP_BEGIN_DEFINE_RTTI(joint_rotation)
            LIP_RTTI_MEMBER(joint_rotation, m_transform)
            LIP_END_DEFINE_RTTI(joint_rotation)

            LIP_BEGIN_DEFINE_RTTI(joint_translation)
            LIP_RTTI_MEMBER(joint_translation, m_transform)
            LIP_END_DEFINE_RTTI(joint_translation)

            LIP_BEGIN_DEFINE_RTTI(joint_translation_scale)
            LIP_RTTI_MEMBER(joint_translation_scale, m_translation)
            LIP_RTTI_MEMBER(joint_translation_scale, m_scale)
            LIP_END_DEFINE_RTTI(joint_translation_scale)

            LIP_BEGIN_DEFINE_RTTI(joint_transform)
            LIP_RTTI_MEMBER(joint_transform, m_rotation)
            LIP_RTTI_MEMBER(joint_transform, m_translation_scale)
            LIP_END_DEFINE_RTTI(joint_transform)

            LIP_BEGIN_DEFINE_RTTI(joint_name)
            LIP_RTTI_MEMBER(joint_name, m_hash)
            LIP_END_DEFINE_RTTI(joint_name)

            LIP_BEGIN_DEFINE_RTTI(joint_linkage)
            LIP_RTTI_MEMBER(joint_linkage, m_joint)
            LIP_RTTI_MEMBER(joint_linkage, m_parent)
            LIP_END_DEFINE_RTTI(joint_linkage)

            LIP_BEGIN_DEFINE_RTTI(skeleton)
            LIP_RTTI_MEMBER(skeleton, m_joint_inverse_bind_pose)
            LIP_RTTI_MEMBER(skeleton, m_joint_inverse_bind_pose2)
            LIP_RTTI_MEMBER(skeleton, m_joint_local_transforms)
            LIP_RTTI_MEMBER(skeleton, m_joint_names)
            LIP_RTTI_MEMBER(skeleton, m_joint_name_indices)
            LIP_RTTI_MEMBER(skeleton, m_joint_linkage)
            LIP_RTTI_MEMBER(skeleton, m_joint_linkage_indices)
            LIP_RTTI_MEMBER(skeleton, m_locomotion_joint_index)
            LIP_RTTI_MEMBER(skeleton, m_joint_local_transforms2)
            LIP_RTTI_MEMBER(skeleton, m_joint_linkage2)
            LIP_END_DEFINE_RTTI(skeleton)

            LIP_BEGIN_DEFINE_RTTI(joint_animation)
            LIP_RTTI_MEMBER(joint_animation, m_rotation_keys)
            LIP_RTTI_MEMBER(joint_animation, m_rotation_times)
            LIP_RTTI_MEMBER(joint_animation, m_translation_keys)
            LIP_RTTI_MEMBER(joint_animation, m_translation_times)
            LIP_RTTI_MEMBER(joint_animation, m_joint_name)
            LIP_END_DEFINE_RTTI(joint_animation)

            LIP_BEGIN_DEFINE_RTTI(joint_animations)
            LIP_RTTI_MEMBER(joint_animations, m_joint_animations)
            LIP_RTTI_MEMBER(joint_animations, m_duration)
            LIP_RTTI_MEMBER(joint_animations, m_ticks_per_second)
            LIP_END_DEFINE_RTTI(joint_animations)
        }
}
