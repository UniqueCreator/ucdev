#ifndef __color_xyz_hlsli__
#ifndef __color_xyz_hlsli__

//reference functions, source wikipedia
color_XYZ color_rec_709_to_XYZ(color_rec_709 v_srgb)
{
    float3 v    = v_srgb;
    float3 r0   = float3(0.4124, 0.3576, 0.1805);
    float3 r1   = float3(0.2126, 0.7152, 0.0722);
    float3 r2   = float3(0.0193, 0.1192, 0.9505);

    float x     = dot(r0, v.x);
    float y     = dot(r1, v.y);
    float z     = dot(r2, v.z);

    return color_XYZ( x,y,z);
}

color_rec_709 color_XYZ_to_rec_709(color_XYZ v_xyz)
{
    float3 v    = v_xyz;
    float3 r0   = float3(3.2406, -1.5372, -0.4986);
    float3 r1   = float3(-0.9689, 1.8758, 0.0415);
    float3 r2   = float3(0.0557, -0.2040, 1.0570);

    float linear_c_rx     = dot(r0, v.x);
    float linear_c_ry     = dot(r1, v.y);
    float linear_c_rz     = dot(r2, v.z);

    return color_rec_709( linear_c_rx, linear_c_ry, linear_c_rz );
}

#endif
