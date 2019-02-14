#include "../default_signature.hlsli"

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
    float4 color        : color0;
};

struct input
{
    float2 position : position;
    float2 uv       : texcoord0;
    float4 color    : color0;
};

cbuffer per_frame   : register(b0)
{
    float4x4 m_view;
    float4x4 m_perspective;
};

cbuffer per_draw_call : register(b1)
{
    float4x4 m_world;
};

float4 project_vertex(float4 v_os, float4x4 world, float4x4 view, float4x4 perspective)
{
    //three muls for greater accuracy
    float4 result = mul( mul( mul( v_os, world ), view), perspective);

    return result;
}

[RootSignature( MyRS1 ) ]
interpolants main(input i)
{
    interpolants r;

    r.uv        = i.uv;
    r.color     = i.color;
    r.position  = project_vertex(float4(i.position, 0.0f, 1.0f), m_world, m_view, m_perspective);

    return r;
}





