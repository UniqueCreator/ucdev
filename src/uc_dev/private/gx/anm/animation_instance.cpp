#include "pch.h"

#include <uc_dev/gx/anm/animation_instance.h>
#include <uc_dev/gx/anm/skeleton_instance.h>
#include <uc_dev/gx/lip/animation.h>

#include <ppl.h>

namespace uc {
    namespace gx {
        namespace anm {
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            math::float4 animate_translation(const lip::joint_animation* a, double time, double animation_duration)
            {
                math::float4 t = math::zero();

                size_t keys = a->m_translation_keys.size();

                if (keys == 0)
                {
                    return t;
                }

                uint32_t this_frame = 0;

                //todo: linear search
                while (this_frame < keys - 1)
                {
                    if (time < a->m_translation_times[this_frame + 1].m_time)
                    {
                        break;
                    }
                    ++this_frame;
                }

                uint32_t next_frame = (this_frame + 1) % keys;
                math::float4 this_key = math::load4(&a->m_translation_keys[this_frame]);
                math::float4 next_key = math::load4(&a->m_translation_keys[next_frame]);

                //todo: remove double calculations
                auto this_time = a->m_translation_times[this_frame].m_time;
                auto next_time = a->m_translation_times[next_frame].m_time;
                auto  diff_time = next_time - this_time;

                if (diff_time < 0.0)
                {
                    diff_time += animation_duration;
                }

                if (diff_time > 0.0)
                {
                    float factor = static_cast<float> ((time - this_time) / diff_time);

                    math::float4 f = math::set(factor, factor, factor, 0.0f);

                    t = math::add(this_key, math::mul(f, math::sub(next_key, this_key)));
                }
                else
                {
                    t = this_key;
                }

                return t;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            math::float4 animate_rotation(const lip::joint_animation* a, double time, double animation_duration)
            {
                math::float4 r = math::identity_r3();

                size_t keys = a->m_rotation_keys.size();

                uint32_t this_frame = 0;

                //todo: linear search
                while (this_frame < keys - 1)
                {
                    if (time < a->m_rotation_times[this_frame + 1].m_time)
                    {
                        break;
                    }
                    ++this_frame;
                }

                uint32_t next_frame = (this_frame + 1) % keys;
                math::float4 this_key = math::load4(&a->m_rotation_keys[this_frame]);
                math::float4 next_key = math::load4(&a->m_rotation_keys[next_frame]);

                //todo: remove double calculations
                auto this_time = a->m_rotation_times[this_frame].m_time;
                auto next_time = a->m_rotation_times[next_frame].m_time;
                auto  diff_time = next_time - this_time;

                if (diff_time < 0.0)
                {
                    diff_time += animation_duration;
                }

                if (diff_time > 0.0)
                {
                    float factor = static_cast<float> ((time - this_time) / diff_time);
                    r = math::slerp(this_key, next_key, math::splat(factor));
                }
                else
                {
                    r = this_key;
                }

                return r;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            animation_instance::animation_instance(const lip::joint_animations* a, const lip::skeleton* s, double start_time) : m_animations(a)
            {
                m_start_time = start_time;
                m_time = start_time * a->m_ticks_per_second;
                m_skeleton_map = make_skeleton_animation_map(s, a);
            }

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            void animation_instance::reset()
            {
                m_time = m_start_time * m_animations->m_ticks_per_second;
            }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            void animation_instance::accumulate(skeleton_instance* result, double delta_time)
            {
                std::vector< math::float4x4 >& res = result->local_transforms();

                auto a = m_animations;
                auto time = delta_time;

                // every following time computation happens in ticks
                time *= a->m_ticks_per_second;

                m_time += time;

                // map into anim's duration
                m_time = fmod(m_time, a->m_duration);

                time = m_time;

                auto s = a->m_joint_animations.size();

                //todo: do this with avx
                concurrency::parallel_for(static_cast<size_t>(0U), s, [ this, &res, time, a ](const auto i)
                {
                    //todo: do this with avx
                    auto t = animate_translation(&a->m_joint_animations[i], time, a->m_duration);
                    auto r = animate_rotation(&a->m_joint_animations[i], time, a->m_duration);

                    auto m = math::quaternion_2_matrix(r);
                    m.r[3] = math::permute<math::permute_0x, math::permute_0y, math::permute_0z, math::permute_1w>(t, math::one());

                    assert(m_skeleton_map.m_data[i] != 0xffff);
                    res[m_skeleton_map.m_data[i]] = m;

                });
            }
        }
    }
}

           
