#include "../default_signature.hlsli"

#include "vector_space.hlsli"
#include "frame.hlsli"

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
};

struct input
{
    float3 position : position;
    float2 uv       : texcoord0;
};

cbuffer per_draw_call : register(b1)
{
    euclidean_transform_3d m_world;
};

[RootSignature( MyRS1 ) ]
interpolants main(input i)
{
    interpolants r;

    point_os position_os    = make_point_os(i.position);
    
    r.uv                    = i.uv;
    r.position              = project_p_os(position_os, m_world, m_view, m_perspective).m_value;

    return r;
}





