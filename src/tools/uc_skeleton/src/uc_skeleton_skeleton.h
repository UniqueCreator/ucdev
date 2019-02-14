#pragma once

#include <cstdint>
#include <unordered_set>

#include <uc_dev/gx/lip/animation.h>
#include <uc_dev/gx/import/geo/skinned_mesh.h>

namespace uc
{
    namespace skeleton
    {
        struct joint_depth
        {
            uint32_t m_joint;
            uint32_t m_joint_parent;
            uint32_t m_depth;

            bool operator<(const joint_depth& o) const
            {
                return m_depth < o.m_depth;
            }

            bool operator==(const joint_depth& a) const
            {
                return m_joint == a.m_joint;
            }
        };

        inline lip::reloc_array < lip::joint_transform > bind_pose(const gx::import::geo::skeleton_pose& s)
        {
            lip::reloc_array < lip::joint_transform > r;

            r.reserve(s.m_skeleton.m_joints.size());

            for (auto&& v : s.m_skeleton.m_joints)
            {
                lip::joint_transform t;
                math::store4(&t.m_rotation, v.m_inverse_bind_pose.m_rotation);
                math::store4(&t.m_translation_scale, v.m_inverse_bind_pose.m_translation);
                r.push_back(std::move(t));
            }

            return r;
        }

        inline lip::reloc_array < lip::matrix4x4 > bind_pose2(const gx::import::geo::skeleton_pose& s)
        {
            lip::reloc_array < lip::matrix4x4 > r;

            r.reserve(s.m_skeleton.m_joints.size());

            for (auto&& v : s.m_skeleton.m_joints)
            {
                lip::matrix4x4 t;
                
                math::store4(&t.m_a0, v.m_inverse_bind_pose2.m_transform.r[0]);
                math::store4(&t.m_b0, v.m_inverse_bind_pose2.m_transform.r[1]);
                math::store4(&t.m_c0, v.m_inverse_bind_pose2.m_transform.r[2]);
                math::store4(&t.m_d0, v.m_inverse_bind_pose2.m_transform.r[3]);
                r.push_back(std::move(t));
            }

            return r;
        }

        inline auto joint_names( const gx::import::geo::skeleton_pose& s )
        {
            auto&& joints = s.m_skeleton.m_joints;

            lip::reloc_array < lip::joint_name > joint_names;
            lip::reloc_array < uint16_t   > joint_indices;

            joint_names.resize(joints.size());
            joint_indices.resize(joints.size());

            std::transform(joints.begin(), joints.end(), joint_names.begin(), [](const auto& b)
            {
                lip::joint_name n;
                n.m_hash = string_hash(b.m_name.c_str()).get_hash();
                return n;
            });

            uint16_t g = 0;
            std::generate(joint_indices.begin(), joint_indices.end(), [&g] { return g++; });

            std::sort(joint_indices.begin(), joint_indices.end(), [&joint_names](const auto a, const auto b)
            {
                return joint_names[a].m_hash < joint_names[b].m_hash;
            });

            std::sort(joint_names.begin(), joint_names.end(), [](const auto a, const auto b)
            {
                return a.m_hash < b.m_hash;
            });

            return std::make_tuple(joint_names, joint_indices);
        }

        inline lip::reloc_array < lip::joint_transform > local_transforms(const gx::import::geo::skeleton_pose& s)
        {
            lip::reloc_array < lip::joint_transform > r;

            r.reserve(s.m_joint_local_pose.size());

            for (auto&& v : s.m_joint_local_pose)
            {
                lip::joint_transform t;
                math::store4(&t.m_rotation, v.m_transform.m_rotation);
                math::store4(&t.m_translation_scale, v.m_transform.m_translation);
                r.push_back(std::move(t));
            }

            return r;
        }

        inline lip::reloc_array < lip::matrix4x4 > local_transforms2(const gx::import::geo::skeleton_pose& s)
        {
            lip::reloc_array < lip::matrix4x4 > r;

            r.reserve(s.m_joint_local_pose.size());

            for (auto&& v : s.m_joint_local_pose)
            {
                lip::matrix4x4 t;


                math::store4(&t.m_a0, v.m_transform_matrix.m_transform.r[0]);
                math::store4(&t.m_b0, v.m_transform_matrix.m_transform.r[1]);
                math::store4(&t.m_c0, v.m_transform_matrix.m_transform.r[2]);
                math::store4(&t.m_d0, v.m_transform_matrix.m_transform.r[3]);

                r.push_back(std::move(t));
            }

            return r;
        }

        inline auto joint_depths(const std::vector< gx::import::geo::joint > & d)
        {
            std::vector<uint32_t> depths;
            depths.resize(d.size());

            for (auto i = 0U; i < d.size(); ++i)
            {
                uint32_t depth = 0;
                for (auto j = d[i].m_parent_index; j != 0xFFFF; j = d[j].m_parent_index)
                {
                    depth++;
                    depths[i] = std::max(depths[i], depth);
                }
            }

            return depths;
        }

        inline auto joint_set(const gx::import::geo::skeleton_pose& s)
        {
            std::unordered_set<uint32_t> r;
            size_t size = s.m_skeleton.m_joints.size();
            for (auto i = 0U; i < size; ++i)
            {
                r.insert(i);
            }

            return r;
        }

        inline auto joint_links(const std::unordered_set<uint32_t>& links, const std::vector<uint32_t>& depths, const std::vector< gx::import::geo::joint > & joints)
        {
            std::list< joint_depth > r;

            for (auto&& b : links)
            {
                auto parent = joints[b].m_parent_index;

                if (links.find(parent) == links.end())
                {
                    joint_depth s;
                    s.m_joint = b;
                    s.m_joint_parent = parent;
                    s.m_depth = depths[b];
                    r.push_back(std::move(s));
                }
            }

            return r;
        }

        inline auto joint_links2(const std::vector< gx::import::geo::joint > & joints)
        {
            lip::reloc_array< uint16_t > r;

            r.reserve(joints.size());

            for (auto&& b : joints )
            {
                auto parent = b.m_parent_index;
                r.push_back(parent);
            }

            return r;
        }

        struct simd_linkage
        {
            lip::joint_linkage m_linkages[8];
        };

        //form parent,child relation ships sorted by depth and ready to be processed by simd
        inline auto joint_link(const gx::import::geo::skeleton_pose& s)
        {
            auto depths = joint_depths(s.m_skeleton.m_joints);
            auto joint_indices = joint_set(s);
            const auto max_split = 8; //avx

            lip::reloc_array< lip::joint_linkage >  linkages;
            lip::reloc_array< uint16_t >            linkage_map;


            linkage_map.resize(s.m_skeleton.m_joints.size());

            std::vector< simd_linkage >        simd_linkages;

            //split bones in long streaks formed from parents to children
            while (!joint_indices.empty())
            {
                auto links = joint_links(joint_indices, depths, s.m_skeleton.m_joints);

                simd_linkage simd;
                uint32_t     simd_counter = 0;

                while (!links.empty() && (simd_counter < max_split))
                {
                    auto joint = std::max_element(links.begin(), links.end());

                    simd.m_linkages[simd_counter].m_joint	= static_cast<std::uint16_t> (joint->m_joint);
                    simd.m_linkages[simd_counter].m_parent	= static_cast<std::uint16_t> (joint->m_joint_parent);

                    linkage_map[joint->m_joint] = static_cast<uint16_t>( simd_linkages.size() * 8 + simd_counter );

                    joint_indices.erase(joint->m_joint);

                    links.remove(*joint);
                    simd_counter++;
                }

                for (auto i = simd_counter; i < 8; ++i)
                {
                    simd.m_linkages[i].m_joint = simd.m_linkages[i - 1].m_joint;
                    simd.m_linkages[i].m_parent = simd.m_linkages[i - 1].m_parent;
                }

                simd_linkages.push_back(simd);
            }

            //copy the collected bones to output arrays

            linkages.resize(simd_linkages.size() * 8);
            std::memcpy(&linkages[0], &simd_linkages[0], simd_linkages.size() * 8 * sizeof(lip::joint_linkage));


            for (auto i = 0U; i < s.m_skeleton.m_joints.size(); ++i)
            {
                #if defined(_DEBUG)

                auto parent1 = s.m_skeleton.m_joints[i].m_parent_index;
                auto parent2 = linkages[linkage_map[i]].m_parent;
                auto joint1 = i;
                auto joint2 = linkages[linkage_map[i]].m_joint;

                assert(parent1 == parent2);
                assert(joint1 == joint2);

                #endif
            }

            return std::make_tuple(linkages, linkage_map);
        }

        inline std::unique_ptr< uc::lip::skeleton > skeleton(const gx::import::geo::skeleton_pose& pose, uint16_t locomotion_index = 0)
        {
            std::unique_ptr< uc::lip::skeleton > m = std::make_unique<uc::lip::skeleton>();

            m->m_joint_inverse_bind_pose    = bind_pose(pose);
            m->m_joint_inverse_bind_pose2   = bind_pose2(pose);
            m->m_joint_local_transforms     = local_transforms(pose);
            m->m_joint_local_transforms2    = local_transforms2(pose);
            m->m_joint_linkage2             = joint_links2(pose.m_skeleton.m_joints);
            m->m_locomotion_joint_index     = locomotion_index;

            {
                auto joint = joint_names(pose);
                m->m_joint_names = std::get<0>(joint);
                m->m_joint_name_indices = std::get<1>(joint);

            }

            {
                auto links = joint_link(pose);
                m->m_joint_linkage = std::get<0>(links);
                m->m_joint_linkage_indices = std::get<1>(links);
            }

            return m;
        }



    }
}

