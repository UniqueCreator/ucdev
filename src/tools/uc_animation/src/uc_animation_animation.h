#pragma once

#include <cstdint>
#include <tuple>
#include <unordered_set>

#include <uc_dev/gx/lip/animation.h>
#include <uc_dev/gx/import/anm/animation.h>
#include <uc_dev/fnd/string_hash.h>


namespace uc
{
    namespace animation
    {
        inline auto time(double time)
        {
            lip::joint_time t = { time };
            return t;
        }

        inline auto rotation(const gx::import::anm::joint_rotation_key& k)
        {
            lip::joint_rotation r;
            math::store4(&r.m_transform, k.m_transform);
            return r;
        }

        inline auto translation(const gx::import::anm::joint_translation_key& k)
        {
            lip::joint_translation r;
            math::store4(&r.m_transform, k.m_transform);
            return r;
        }

        inline auto rotation_split( const gx::import::anm::joint_rotation_key& r, lip::joint_rotation& rot, lip::joint_time& time )
        {
            rot = rotation(r);
            time.m_time = r.m_time;
        }

        inline auto translation_split(const gx::import::anm::joint_translation_key& r, lip::joint_translation& t, lip::joint_time& time)
        {
            t  = translation(r);
            time.m_time = r.m_time;
        }

        inline auto rotation_split(const gx::import::anm::joint_animation& a )
        {
            lip::reloc_array<lip::joint_rotation> r;
            lip::reloc_array<lip::joint_time>     t;

            r.reserve(a.m_rotation_keys.size());
            t.reserve(a.m_rotation_keys.size());

            for (auto&& i : a.m_rotation_keys)
            {
                lip::joint_rotation rot = rotation(i);
                lip::joint_time     time;
                time.m_time = i.m_time;
                

                r.push_back(rot);
                t.push_back(time);
            }

            return std::make_tuple(std::move(r), std::move(t));
        }

        inline auto translation_split(const gx::import::anm::joint_animation& a)
        {
            lip::reloc_array<lip::joint_translation> r;
            lip::reloc_array<lip::joint_time>     t;

            r.reserve(a.m_translation_keys.size());
            t.reserve(a.m_translation_keys.size());

            for (auto&& i : a.m_translation_keys)
            {
                lip::joint_translation trans = translation(i);
                lip::joint_time        time;

                time.m_time = i.m_time;


                r.push_back(trans);
                t.push_back(time);
            }

            return std::make_tuple(std::move(r), std::move(t));
        }

        //quantization, analysis, curve fitting go here, for now, just copy
        inline std::unique_ptr< lip::joint_animations > animation(const gx::import::anm::joint_animations& a)
        {
            std::unique_ptr< uc::lip::joint_animations > m = std::make_unique<uc::lip::joint_animations>();

            m->m_ticks_per_second		= a.m_ticks_per_second;
            m->m_duration				= a.m_duration;

            for ( auto&& r : a.m_joint_animations )
            {
                lip::joint_animation ja;

                ja.m_joint_name.m_hash = make_string_hash(r.m_joint_name).get_hash();

                {
                    auto s				= rotation_split(r);

                    ja.m_rotation_keys  = std::move(std::get<0>(s));
                    ja.m_rotation_times = std::move(std::get<1>(s));
                }

                {
                    auto s = translation_split(r);
                    ja.m_translation_keys = std::move(std::get<0>(s));
                    ja.m_translation_times = std::move(std::get<1>(s));
                }

                m->m_joint_animations.push_back(std::move(ja));
            }

            return m;
        }

    }
}

