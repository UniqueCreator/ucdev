#ifndef __pbr_brdf_hlsli__
#ifndef __pbr_brdf_hlsli__

#include "pbr_common.hlsli"


//identities
// | L + V | ^ 2 = 2 + 2L.V
// 0.5 + 0.5 L.V = 1 / 4 |L + V|^2
// N.H = ( N.L + N.V ) / | L + V |
// L.H = V.H = 1 / 2 |L + V|

void pbr_ndoth_vdoth( float ldotv, float ndotl, float ndotv, out float ndoth, out float ldoth )
{
    float lv     = 2 + 2 * ldotv;
    float rcp_lv = rqsrt( lv );

    ndoth = (ndotl + ndotv) * rcp_lv;
    ldoth = rcp_lv + rcp_lv * ldotv;
}

float pbr_ndoth( float ldotv, float ndotl, float ndotv)
{
    float lv     = 2 + 2 * ldotv;
    float rcp_lv = rqsrt( lv );

    float ndoth = (ndotl + ndotv) * rcp_lv;
    return ndoth;
}

float pbr_ldoth( float ldotv, float ndotl, float ndotv)
{
    float lv     = 2 + 2 * ldotv;
    float rcp_lv = rqsrt( lv );

    float ldoth  = rcp_lv + rcp_lv * ldotv;
    return ldoth;
}

float pbr_d_ggx( float alpha_g4, float ndoth )
{
    float a2    = alpha_g4;
    float d     = ( ndoth * a2 - ndoth ) * ndoth + 1;
    return a2 / (  d * d );                 
}

float pbr_v_smith_ggx_correlated( float alpha_g4, float ndotv, float ndotl )
{
    float a2            = alpha_g4;

    // Caution : the " NdotL *" and " NdotV *" are explicitely inversed , this is not a mistake .
    float smith_ggx_v   = ndotl * sqrt (( -ndotv * a2 + ndotv ) * ndotv + a2 );
    float smith_ggx_l   = ndotv * sqrt (( -ndotl * a2 + ndotl ) * ndotl + a2 );
    return 0.5f * rcp ( smith_ggx_v + smith_ggx_l );
}

float3 pbr_fresnel_schlick( float3 f0, float vdoth )
{
    float fc    = pbr_pow_5( 1 - vdoth );

    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    float f90   = saturate (50.0 * dot ( f0, 0.33) );
    return lerp( f0, f90, fc );
}


// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
/*
float v_smith_joint_approximate( float roughness, float ndotv, float ndotl )
{
    float a         = pow_2( roughness );
    float smith_v   = ndotl * ( ndotv * ( 1 - a ) + a );
    float simith_l  = ndotv * ( ndotl * ( 1 - a ) + a );

    // Note: will generate NaNs with roughness = 0.  min_roughness is used to prevent this
    return 0.5 * rcp( smith_v + smith_l );
}
*/




#endif

