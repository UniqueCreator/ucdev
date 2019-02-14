#include "default_graphics_signature.hlsl"

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
};

Texture2D<float4> t         : register(t1);
SamplerState g_linear       : register(s0)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};



[RootSignature( MyRS1 ) ]
float4 main( interpolants r ) : SV_Target0
{
    //float2 te = float2(r.uv.x, r.uv.y);
    //return t.Sample(g_linear, te);
    return float4(1.0f, 0.0f, 0.0f, 0.0f);
}





