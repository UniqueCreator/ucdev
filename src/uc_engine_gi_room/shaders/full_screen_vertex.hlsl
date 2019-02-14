#include <uc_public/gpu/default_graphics_signature.hlsli>

struct interpolants
{
    float4 position     : SV_POSITION0;
    float2 uv           : texcoord0;
};


[RootSignature( MyRS1 ) ]
interpolants main( in uint VertID : SV_VertexID )
{
    interpolants r;
    // Texture coordinates range [0, 2], but only [0, 1] appears on screen.
    r.uv       = float2(uint2(VertID, VertID << 1) & 2);
    r.position = float4(lerp(float2(-1, 1), float2(1, -1), r.uv), 0, 1);

    return r;
}





