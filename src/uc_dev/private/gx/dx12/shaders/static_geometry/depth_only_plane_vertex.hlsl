#include "../default_signature.hlsli"

#include "vector_space.hlsli"
#include "frame.hlsli"

struct interpolants
{
    float4 position       : SV_POSITION0;
};

struct input
{
    uint vid : SV_VertexID;
};

cbuffer per_draw_call : register(b1)
{
    euclidean_transform_3d m_world;
};

static const float3 g_triangle_0[6] =
{
    float3(-2, 0, -2),
    float3( 2, 0, -2),
    float3(-2, 0,  2),

    float3(2, 0,  2),
    float3(-2, 0,  2),
    float3(2, 0, -2),
};



[RootSignature( MyRS1 ) ]
interpolants main(input i)
{
    interpolants r;
    
    uint vid = i.vid > 5 ? 5 : i.vid;
    
    point_os position_os    = make_point_os(g_triangle_0[vid]);

    r.position              = project_p_os(position_os, m_world, m_view, m_perspective).m_value;
    return r;
}





