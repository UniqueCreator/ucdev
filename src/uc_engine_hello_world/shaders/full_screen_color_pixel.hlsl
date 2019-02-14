#include <uc_public/gpu/default_graphics_signature.hlsli>

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
};

[RootSignature( MyRS1 ) ]
float4 main( interpolants r ) : SV_Target0
{
    return float4(1.0f, 0.0f, 0.0f, 0.0f);
}





