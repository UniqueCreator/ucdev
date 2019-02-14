#ifndef __shade_hlsli__
#define __shade_hlsli__

#include "../color/color.hlsli"
#include "../lighting/pbr_diffuse.hlsli"
#include "../lighting/pbr_specular.hlsli"

color_rec_709 shade_direct_light( float3 n, float3 l, float3 v, float roughness, float metallic, float3 albedo, color_rec_709 light_color )
{
    const float ldotv = saturate(dot(l,v));
    const float ndotl = saturate(dot(n,l));
    const float ndotv = saturate(dot(n,v));

    const float  dielectric         = 0.04;
    const float  ambient_occlusion  = 1.0;
    const float3 diffuse_albedo     = lerp(albedo, float3(0, 0, 0), metallic) * ambient_occlusion;
    const float3 f0                 = lerp(dielectric, albedo, metallic)  * ambientOcclusion;
    
    float3 diffuse      = pbr_diffuse_brdf(roughness, albedo, ldotv, ndotl, ndotv);
    float3 specular     = pbr_diffuse_brdf(roughness, f0,     ldotv, ndotl, ndotv);

    return ( diffuse + specular ) * ndotl * light_color;
}





#endif
