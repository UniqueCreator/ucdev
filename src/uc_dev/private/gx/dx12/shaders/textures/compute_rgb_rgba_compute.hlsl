ByteAddressBuffer    texture_rgb     : register(t0);
RWTexture2D<uint4>   texture_rgba    : register(u1);

cbuffer CB : register(b0)
{
    uint texture_width;
    uint texture_height;
    uint texture_pitch;
}

[numthreads( 64, 1, 1 )]
void main( uint3 dtid : SV_DispatchThreadID )
{
    uint row = dtid.x;

    if ( row < texture_height )
    {
        for (uint i = 0; i < texture_width; i += 3)
        {
            uint a      = row * texture_pitch + i;
            uint3 r     = texture_rgb.Load3(a);
            uint2 pos   = uint2(i / 3, row);
            texture_rgba[pos] = uint4(255, 0, 255, 1);// uint4(r, 1.0f);
        }
    }
}
