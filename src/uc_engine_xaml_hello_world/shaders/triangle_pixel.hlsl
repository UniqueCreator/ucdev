
struct interpolants
{
    float4 position     : SV_POSITION0;
};

Texture2D<float4> t         : register(t1);
SamplerState g_linear       : register(s0)
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

float4 main( interpolants r ) : SV_Target0
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
