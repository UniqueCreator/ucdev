#ifndef __pbr_common_hlsli__
#define __pbr_common_hlsli__

const static float g_pi = 3.1415926535897932f;

float pbr_pow_2(float x)
{
    return x * x;
}

float pbr_pow_4(float x)
{
    return pow_2(x) * pow_2(x);
}

float pbr_pow_5(float x)
{
    return pow_2(x) * pow_2(x) * x;
}

#endif

