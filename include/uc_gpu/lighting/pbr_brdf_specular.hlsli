#ifndef __pbr_brdf_specular_hlsli__
#define __pbr_brdf_specular_hlsli__

#include "pbr_brdf.hlsli"

/*
 l = light
 v = view
 roughness = material parameter
 f0 = fresnel ( 0.04 for dielectrics, color for metals )
*/

float3 pbr_specular_brdf( float roughness, float3 f0, float ldotv, float ndotl, float ndotv )
{
    float ndoth;
    float ldoth;

    ndoth_vdoth( ldotv, ndotl, ndotv, ndoth, ldoth );

    float alpha_4   = pow4(roughness);

    float d         = d_ggx( alpha_4, ndoth );
    float v         = v_smith_ggx_correlated( alpha_g4, ndotv, ndotl );
    float3 f        = fresnel_schlick(f0, ldoth);

    return d * f * v / g_pi;
}



#endif

