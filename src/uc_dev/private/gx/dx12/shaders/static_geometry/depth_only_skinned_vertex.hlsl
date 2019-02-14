#include "../default_signature.hlsli"

#include "vector_space.hlsli"
#include "frame.hlsli"
#include "transform_skinned.hlsli"

struct interpolants
{
    float4 position     : SV_POSITION0;
};

struct input
{
    float3 position : position;
    float4 weights  : blend_weights0;
    uint4 indices   : blend_indices0;
};

cbuffer per_draw_call : register(b1)
{
    euclidean_transform_3d m_world;
    float4x4 m_joints_palette[127];
};


[RootSignature( MyRS1 ) ]
interpolants main(input i)
{
    interpolants r;

    float4     position = float4(i.position, 1.0f);
    point_os   skinned_position = make_point_os(skin_position(position, i.weights, i.indices, m_joints_palette).xyz);
    point_ws   skinned_world = transform_p_os(skinned_position, m_world);

    r.position = project_p_os(skinned_position, m_world, m_view, m_perspective).m_value;

    return r;
}





