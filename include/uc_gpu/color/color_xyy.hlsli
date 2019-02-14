#ifndef __color_xyz_hlsli__
#ifndef __color_xyz_hlsli__

//reference functions, source wikipedia
float3 color_XYZ_to_xyY(float3 v_xyz)
{
    float x;
    float y;

    float X = v_xyz.x;
    float Y = v_xyz.y;
    float Z = v_xyz.z;
    
    x = X / (X + Y + Z);
    y = Y / (X + Y + Z);
    return float3(x,y,Y);
}

float3 color_xyY_to_XYZ(float2 v_xyY)
{
    float x = v_xyY.x;
    float y = v_xyY.y;
    float Y = v_xyY.z;

    float X = (Y / y) * x;
    float Z = (Y / y) * ( 1 - x - y);

    return float3(X,Y,Z);
}

#endif
