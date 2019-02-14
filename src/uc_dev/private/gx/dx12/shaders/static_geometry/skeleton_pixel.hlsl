#include "../default_signature.hlsli"

struct interpolants
{
    float4 position     : SV_POSITION0;
};

[RootSignature( MyRS1 ) ]
float4 main( interpolants r ) : SV_Target0
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
