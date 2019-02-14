#ifndef __pbr_brdf_diffuse_hlsli__
#ifndef __pbr_brdf_diffuse_hlsli__

/*
 l = light
 v = view
 roughness = material parameter
 albedo 
*/
//Hammon model
float3 pbr_diffuse_brdf( float roughness, float3 albedo, float ldotv, float ndotl, float ndotv )
{
    float ndoth;
    float ldoth;

    ndoth_vdoth( ldotv, ndotl, ndotv, ndoth, ldoth );

    float facing = 0.5 + 0.5*ldotv;
    float rough  = facing * (0.9 - 0.4 * facing) * ( (0.5 + ndoth) / ndoth );
    float smooth = 1.05 * ( 1.0 - pow_5( 1 - ndotl ) ) * ( 1 - pow_5( 1 - ndotv) );
    float single = lerp(smooth, rough, roughness) / g_pi;
    float multi  = 0.1159 * roughness;
    
    float3 diffuse = albedo * ( single + albedo * multi );

    return diffuse;
}

#endif

