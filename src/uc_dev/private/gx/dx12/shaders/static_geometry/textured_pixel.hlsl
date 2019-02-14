#include "../default_signature.hlsli"
#include "../default_samplers.hlsli"

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
};

float checker_board_pattern(float2 uv)
{
    float2 c = floor(uv) / 2;
    return frac(c.x + c.y) * 2;
}

float4 checker_board(float2 uv)
{
    float2 uv_scaled = uv * float2(32.0f, 32.0f);
    float checker = checker_board_pattern(uv_scaled);
    return float4(checker, checker, checker, 1.0f);
}

Texture2D<float4> t         : register(t1);



[RootSignature( MyRS1 ) ]
float4 main( interpolants r ) : SV_Target0
{
    //return checker_board(r.uv);
    //float2 te = float2(r.uv.x, 1- r.uv.y);
    float2 te = float2(r.uv.x, r.uv.y);
    return t.Sample(g_linear_clamp , te);

    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
