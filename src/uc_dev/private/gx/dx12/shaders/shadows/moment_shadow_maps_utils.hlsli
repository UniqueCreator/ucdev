#ifndef __moment_shadow_maps_utils_hlsli__
#define __moment_shadow_maps_utils_hlsli__

float p2(float x)
{
    return x * x;
}

float p3(float x)
{
    return p2(x) * x;
}

float p4(float x)
{
    return p3(x) * x;
}

#endif
