#include "pch.h"

#include <uc_dev/gx/lip/model.h>


namespace uc
{
    namespace lip
    {
        LIP_BEGIN_DEFINE_RTTI(indices)
            LIP_RTTI_MEMBER(indices, m_data)
        LIP_END_DEFINE_RTTI(indices)

        LIP_BEGIN_DEFINE_RTTI(positions)
            LIP_RTTI_MEMBER(positions, m_data)
        LIP_END_DEFINE_RTTI(positions)

        LIP_BEGIN_DEFINE_RTTI(uvs)
            LIP_RTTI_MEMBER(uvs, m_data)
        LIP_END_DEFINE_RTTI(uvs)

        LIP_BEGIN_DEFINE_RTTI(normals)
            LIP_RTTI_MEMBER(normals, m_data)
        LIP_END_DEFINE_RTTI(normals)

        LIP_BEGIN_DEFINE_RTTI(tangents)
            LIP_RTTI_MEMBER(tangents, m_data)
        LIP_END_DEFINE_RTTI(tangents)

        LIP_BEGIN_DEFINE_RTTI(model)
            LIP_RTTI_MEMBER(model, m_indices)
            LIP_RTTI_MEMBER(model, m_positions)
        LIP_END_DEFINE_RTTI(model)

        LIP_BEGIN_DEFINE_RTTI(normal_model)
            LIP_RTTI_BASE_CLASS(model)
            LIP_RTTI_MEMBER(normal_model, m_normals)
        LIP_END_DEFINE_RTTI(normal_model)

        LIP_BEGIN_DEFINE_RTTI(parametrized_model)
            LIP_RTTI_BASE_CLASS(model)
            LIP_RTTI_MEMBER(parametrized_model, m_uv)
        LIP_END_DEFINE_RTTI(parametrized_model)

        LIP_BEGIN_DEFINE_RTTI(normal_parametrized_model)
            LIP_RTTI_BASE_CLASS(parametrized_model)
            LIP_RTTI_MEMBER(normal_parametrized_model, m_normals)
        LIP_END_DEFINE_RTTI(normal_parametrized_model)

        LIP_BEGIN_DEFINE_RTTI(derivatives_parametrized_model)
            LIP_RTTI_BASE_CLASS(normal_parametrized_model)
            LIP_RTTI_MEMBER(derivatives_parametrized_model, m_tangents)
        LIP_END_DEFINE_RTTI(derivatives_parametrized_model)

        LIP_BEGIN_DEFINE_RTTI(texture2d)
            LIP_RTTI_MEMBER(texture2d, m_data)
            LIP_RTTI_MEMBER(texture2d, m_width)
            LIP_RTTI_MEMBER(texture2d, m_height)
            LIP_RTTI_MEMBER(texture2d, m_storage_format)
            LIP_RTTI_MEMBER(texture2d, m_view_format)
        LIP_END_DEFINE_RTTI(texture2d)

        LIP_BEGIN_DEFINE_RTTI(texture2d_mip_level)
            LIP_RTTI_MEMBER(texture2d_mip_level, m_data)
            LIP_RTTI_MEMBER(texture2d_mip_level, m_width)
            LIP_RTTI_MEMBER(texture2d_mip_level, m_height)
            LIP_RTTI_MEMBER(texture2d_mip_level, m_mip_levels)
            LIP_RTTI_MEMBER(texture2d_mip_level, m_storage_format)
            LIP_RTTI_MEMBER(texture2d_mip_level, m_view_format)
        LIP_END_DEFINE_RTTI(texture2d_mip_level)

        LIP_BEGIN_DEFINE_RTTI(texture2d_mip_chain)
            LIP_RTTI_MEMBER(texture2d_mip_chain, m_levels)
        LIP_END_DEFINE_RTTI(texture2d_mip_chain)

        LIP_BEGIN_DEFINE_RTTI(textured_model)
            LIP_RTTI_BASE_CLASS(parametrized_model)
            LIP_RTTI_MEMBER(textured_model, m_texture)
        LIP_END_DEFINE_RTTI(textured_model)

        LIP_BEGIN_DEFINE_RTTI(normal_textured_model)
            LIP_RTTI_BASE_CLASS(normal_parametrized_model)
            LIP_RTTI_MEMBER(normal_textured_model, m_texture)
        LIP_END_DEFINE_RTTI(normal_textured_model)

        LIP_BEGIN_DEFINE_RTTI(derivatives_textured_model)
            LIP_RTTI_BASE_CLASS(derivatives_parametrized_model)
            LIP_RTTI_MEMBER(derivatives_textured_model, m_texture)
        LIP_END_DEFINE_RTTI(derivatives_textured_model)

        LIP_BEGIN_DEFINE_RTTI(primitive_range)
            LIP_RTTI_MEMBER(primitive_range, m_begin)
            LIP_RTTI_MEMBER(primitive_range, m_end)
        LIP_END_DEFINE_RTTI(primitive_range)

        LIP_BEGIN_DEFINE_RTTI(multi_textured_model)
            LIP_RTTI_BASE_CLASS(parametrized_model)
            LIP_RTTI_MEMBER(multi_textured_model, m_textures)
            LIP_RTTI_MEMBER(multi_textured_model, m_primitive_ranges)
        LIP_END_DEFINE_RTTI(multi_textured_model)

        LIP_BEGIN_DEFINE_RTTI(normal_multi_textured_model)
            LIP_RTTI_BASE_CLASS(normal_parametrized_model)
            LIP_RTTI_MEMBER(normal_multi_textured_model, m_textures)
            LIP_RTTI_MEMBER(normal_multi_textured_model, m_primitive_ranges)
        LIP_END_DEFINE_RTTI(normal_multi_textured_model)

        LIP_BEGIN_DEFINE_RTTI(derivatives_multi_textured_model)
            LIP_RTTI_BASE_CLASS(derivatives_parametrized_model)
            LIP_RTTI_MEMBER(derivatives_multi_textured_model, m_textures)
            LIP_RTTI_MEMBER(derivatives_multi_textured_model, m_primitive_ranges)
        LIP_END_DEFINE_RTTI(derivatives_multi_textured_model)

        LIP_BEGIN_DEFINE_RTTI(skinned_model)
            LIP_RTTI_BASE_CLASS(multi_textured_model)
            LIP_RTTI_MEMBER(skinned_model, m_blend_weights)
            LIP_RTTI_MEMBER(skinned_model, m_blend_indices)
        LIP_END_DEFINE_RTTI(skinned_model)

        LIP_BEGIN_DEFINE_RTTI(normal_skinned_model)
            LIP_RTTI_BASE_CLASS(normal_multi_textured_model)
            LIP_RTTI_MEMBER(normal_skinned_model, m_blend_weights)
            LIP_RTTI_MEMBER(normal_skinned_model, m_blend_indices)
        LIP_END_DEFINE_RTTI(normal_skinned_model)

        LIP_BEGIN_DEFINE_RTTI(derivatives_skinned_model)
            LIP_RTTI_BASE_CLASS(derivatives_multi_textured_model)
            LIP_RTTI_MEMBER(derivatives_skinned_model, m_blend_weights)
            LIP_RTTI_MEMBER(derivatives_skinned_model, m_blend_indices)
        LIP_END_DEFINE_RTTI(derivatives_skinned_model)
        
    }
}