#include <uc_public/gpu/default_graphics_signature.hlsli>

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
};

struct input
{
    float3 position : position;
    float2 uv       : texcoord0;
    float4 weights  : blend_weights0;
    uint4 indices   : blend_indices0;
};

cbuffer per_frame   : register(b0)
{
    float4x4 m_view;
    float4x4 m_perspective;
};

cbuffer per_draw_call : register(b1)
{
    float4x4 m_world;
    float4x4 m_joints_palette[127];
};

float4 project_vertex(float4 v_os, float4x4 world, float4x4 view, float4x4 perspective)
{
    //three muls for greater accuracy
    float4 result = mul( mul( mul( v_os, world ), view), perspective);

    return result;
}

//todo: replace with quaternions
float4 skin_position( float4 position, float4x4 joint_transform, float weight )
{
    return mul( position, joint_transform )  * weight;
}

float4 skin_position(float4 position, float4 weights, float4 indices, float4x4 joints[127] )
{
    float4 skinned_position = position;

    //weights = float4(0.25, 0.25, 0.0, 0.0);
    //indices = uint4(81, 0, 25, 12);

    skinned_position =  skin_position(position, joints[indices.x], weights.x);
    skinned_position += skin_position(position, joints[indices.y], weights.y);
    skinned_position += skin_position(position, joints[indices.z], weights.z);
    skinned_position += skin_position(position, joints[indices.w], weights.w);

    skinned_position /= (weights.x + weights.y + weights.z + weights.w);

    return skinned_position;
}

[RootSignature( MyRS1 ) ]
interpolants main(input i)
{
    interpolants r;

    float4     position         = float4(i.position, 1.0f);
    float4     skinned_position = skin_position(position, i.weights, i.indices, m_joints_palette);
    
    r.uv       = i.uv;
    r.position = project_vertex( float4(skinned_position.xyz, 1 ), m_world, m_view, m_perspective );

    return r;
}





