#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>

#include <uc_dev/gx/import/anm/animation.h>
#include <uc_dev/gx/import/assimp/assimp_details.h>

namespace uc
{
    namespace gx
    {
        namespace import
        {
            namespace assimp
            {
                inline std::string animation_name(const aiAnimation* a)
                {
                    std::string r;
                    if (a->mName.length == 0)
                    {
                        r = "no_name";
                    }
                    else
                    {
                        r = a->mName.C_Str();
                    }

                    std::transform(r.begin(), r.end(), r.begin(), [](auto v) { return static_cast<char>(::tolower(v)); });
                    std::transform(r.begin(), r.end(), r.begin(), [](const auto& v)
                    {
                        std::locale loc;
                        if (!std::isalnum(v, loc))
                        {
                            return '_';
                        }
                        else
                        {
                            return v;
                        }
                    });

                    return r;
                }

                inline std::vector<anm::joint_rotation_key> rotation_keys(const aiNodeAnim* a)
                {
                    using namespace anm;
                    std::vector<joint_rotation_key> r;

                    for (auto rotation = 0U; rotation < a->mNumRotationKeys; ++rotation)
                    {
                        auto rot = a->mRotationKeys[rotation];

                        joint_rotation_key rotation_key;

                        rotation_key.m_time = rot.mTime;

                        //first component in assimp is the w of the quaternion, so swap it
                        rotation_key.m_transform = math::swizzle<math::y, math::z, math::w, math::x>(math::load4(&rot.mValue));
                        r.push_back(std::move(rotation_key));
                    }

                    return r;
                }

                inline std::vector<anm::joint_translation_key> translation_keys(const aiNodeAnim* a)
                {
                    using namespace anm;
                    std::vector<joint_translation_key> r;

                    for (auto translation = 0U; translation < a->mNumPositionKeys; ++translation)
                    {
                        auto pos = a->mPositionKeys[translation];

                        joint_translation_key key;

                        key.m_time = pos.mTime;
                        key.m_transform = math::load3u(&pos.mValue);
                        r.push_back(key);
                    }

                    return r;
                }

                inline std::vector<anm::joint_animations> create_animations_from_assimp(const std::string& file_name, uint32_t import_flags = 0)
                {
                    using namespace anm;
                    auto scene = load_assimp_scene(file_name, import_flags);
                    validate_scene(scene.get(), validation_option::animation );
                    std::vector<joint_animations> r;

                    for (auto i = 0U; i < scene->mNumAnimations; ++i)
                    {
                        auto animation = scene->mAnimations[i];

                        joint_animations a;

                        a.m_name = animation_name(animation);
                        a.m_duration = animation->mDuration;
                        a.m_ticks_per_second = animation->mTicksPerSecond;

                        if (a.m_ticks_per_second == 0.0)
                        {
                            a.m_ticks_per_second = 30.0;
                        }

                        //todo: raise exception
                        assert(a.m_duration != 0.0);

                        for (auto j = 0U; j < animation->mNumChannels; ++j)
                        {
                            auto node_anim = animation->mChannels[j];

                            joint_animation joint;

                            joint.m_joint_name = node_anim->mNodeName.C_Str();
                            joint.m_rotation_keys = rotation_keys(node_anim);
                            joint.m_translation_keys = translation_keys(node_anim);

                            a.m_joint_animations.push_back(std::move(joint));
                        }

                        r.push_back(std::move(a));
                    }

                    std::string noname = "no_name";
                    for (auto i = 0U; i < r.size(); ++i)
                    {
                        if (r[i].m_name == noname)
                        {
                            std::stringstream str;
                            str << "no_name_" << std::setfill('0') << std::setw(4) << i;
                            r[i].m_name = str.str();
                        }
                    }

                    return r;
                }
            }
        }
    }
}







