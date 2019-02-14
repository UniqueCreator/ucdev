#pragma once

#include <uc_dev/lip/lip.h>
#include "base.h"
#include "math.h"

namespace uc
{
    namespace lip
    {
        struct joint_time
        {
            double m_time;
            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_time)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_time >)

        struct joint_rotation
        {
            quaternion m_transform;
            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_rotation)

        struct joint_translation
        {
            vector4 m_transform;
            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_translation)

        struct joint_translation_scale
        {
            vector3 m_translation;
            float   m_scale;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_translation_scale)

        struct alignas(32) joint_transform
        {
            joint_rotation          m_rotation;
            joint_translation_scale m_translation_scale;

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_transform)

        struct joint_name
        {
            uint32_t m_hash;  

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_name)

        struct joint_linkage
        {
            uint16_t m_joint;
            uint16_t m_parent;  //parent child relationship

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_linkage)

        struct joint_bounding_volume
        {
            point3 m_center;
            float  m_radius;
        };

        struct skeleton
        {
            reloc_array < joint_transform >    m_joint_inverse_bind_pose;   //bind pose is when the pose of the skeleton when you bind the vertices
            reloc_array < matrix4x4 >          m_joint_inverse_bind_pose2;

            reloc_array < joint_transform >    m_joint_local_transforms;    //this is the rest or local pose, this is what is changed from the animations

            reloc_array < joint_name >         m_joint_names;               //joint names are sorted, for faster search by name
            reloc_array < uint16_t >           m_joint_name_indices;        //and point to indices 

            reloc_array < joint_linkage >      m_joint_linkage;             //( joint, parent ) sorted by depth
            reloc_array < uint16_t >           m_joint_linkage_indices;     //( points to the joint linkage struct for given joint

            reloc_array < matrix4x4 >          m_joint_local_transforms2;   //this is the rest or local pose, this is what is changed from the animations
            reloc_array < uint16_t>            m_joint_linkage2;

            reloc_array<joint_bounding_volume> m_joint_bounding_volumes;    //bounding volume of the vertices affected by this bone in bind pose space
            
            uint16_t                           m_locomotion_joint_index;  
            skeleton()
            {

            }

            skeleton(const lip::load_context& c) : 
                    m_joint_inverse_bind_pose(c)
                ,   m_joint_inverse_bind_pose2(c)
                ,   m_joint_local_transforms(c)
                ,   m_joint_names(c)
                ,   m_joint_name_indices(c)
                ,   m_joint_linkage(c)
                ,   m_joint_linkage_indices(c)
                ,   m_joint_local_transforms2(c)
                ,   m_joint_linkage2(c)
            {

            }

            size_t joint_count() const
            {
                return m_joint_inverse_bind_pose.size();
            }

            joint_linkage linkage(uint16_t joint_index) const
            {
                return m_joint_linkage[m_joint_linkage_indices[joint_index]];
            }

            joint_name   name( uint16_t joint_index ) const
            {
                return m_joint_names[m_joint_name_indices[joint_index]];
            }

            uint16_t     joint_index(joint_name n) const
            {
                auto s = m_joint_names.size();

                for (auto i = 0U; i < s; ++i)
                {
                    if ( m_joint_names[i].m_hash == n.m_hash )
                    {
                        return m_joint_name_indices[i];
                    }
                }

                assert(false);
                return 0xffff;
            }

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::skeleton)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_transform >)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_linkage >)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_name >)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < matrix4x4 >)

        struct joint_animation
        {
            //todo: subject to change, with optimizations
            reloc_array < joint_rotation >      m_rotation_keys;
            reloc_array < joint_time >          m_rotation_times;
            reloc_array < joint_translation >   m_translation_keys;
            reloc_array < joint_time >          m_translation_times;
            joint_name                          m_joint_name;

            joint_animation()
            {

            }

            joint_animation(const lip::load_context& c) : 
                    m_rotation_keys(c)
                ,   m_rotation_times(c)
                ,   m_translation_keys(c)
                ,   m_translation_times(c)
            {

            }

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_animation)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_rotation > )
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_translation > )

        struct joint_animations
        {
            reloc_array< joint_animation >  m_joint_animations;
            double                          m_duration;
            double                          m_ticks_per_second;

            joint_animations()
            {

            }

            joint_animations(const lip::load_context& c ) : m_joint_animations(c)
            {

            }

            LIP_DECLARE_RTTI()
        };

        LIP_DECLARE_TYPE_ID(uc::lip::joint_animations)
        LIP_DECLARE_TYPE_ID(uc::lip::reloc_array < joint_animation > )
    }
}
