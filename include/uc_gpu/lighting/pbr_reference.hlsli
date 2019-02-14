#ifndef __pbr_reference_hlsli__
#define __pbr_reference_hlsli__

#include "pbr_common.hlsli"
#include "pbr_brdf.hlsli"


//Moving frostbite engine to pbr 
//Listing A.1 shows a simple kernel for integrating a light probe with a specular GGX microfacet model in order to quickly see the expected results.

//paper Building an Orthonormal Basis, Revisited
float copysignf( float x, float y )
{
    return abs(x) * sgn ( y );
}

void branchless_onb( in float3 n, out float3 b1, out float3 b2)
{
    const float sign = copysignf(1.0f, n.z);
    const float a = -1.0f / (sign + n.z);
    const float b = n.x * n.y * a;
    b1 = float3(1.0f + sign * n.x * n.x * a, sign * b, -sign * n.x);
    b2 = float3(b, sign + n.y * n.y * a, -n.y);
}

void get_tangent_basis(in float3 n, out float3 tangent_x, out float3 tangent_y )
{
    float3 up_vector    = abs(n.z) < 0.999 ? float3(0,0,1) : float3(1,0,0);
    tangent_x           = normalize( cross( up_vector, n ) );
    tangent_y           = cross( n, tangent_x );
    branchless_onb( n, tangent_x, tangent_y);
}

static float2 get_sample ( uint32_t sample, uint32_t sample_count )
{
    //todo:
    return 0.0f;
}

static const uint32_t g_sample_count = 1024;

float3 pbr_reference_compute_specular_ibl_reference( in float3 n, in float3 v, in float roughness, in float3 f0 )
{
    //build local referential
    //float3 up_vector = abs(n.z) < 0.999 ? float3(0, 0, 1) : float3 ( 1, 0, 0 );
    //float3 tangent_x = normalize( cross ( up_vector, n ) );
    //float3 tangent_y = cross ( n, tangent_x );

    float3 tangent_x;
    float3 tangent_y;
    get_tangent_basis( n, tangent_x, tangent_y );

    float3 accumulated_light = 0;
    for ( uint32_t i = 0; i < g_sample_count; ++i )
    {
        float2 u = get_sample( i, g_sample_count );

        //ggx ndf sampling
        float cos_theta_h = sqrt( 1-u.x ) / ( 1 + (roughness * roughness - 1) * u.x );
        float sin_theta_h = sqrt( 1- min( 1, cos_theta_h, cos_theta_h ) );
        float phi_h       = u.y * g_pi / 2;

        //convert sample from half angle to incident angle
        float3 h;
        h = float3( sin_theta_h * cos(phi_h) , sin_theta_h * sin(phi_h), cos_theta_h);
        h = normalize ( tangent_x * h.y + tangent_t * h.x + n * h.z );
        
        float3 l = dot( 2 * dot(v,h) * h - v );


        float ldoth = saturate(dot(h,l));
        float ndoth = saturate(dot(n,h));
        float ndotv = saturate(dot(n,v));
        float ndotl = saturate(dot(n,l));

        //importance sampling weight for each sample
        //weight = fr.(n.l)
        //fr = d(h) * f(h) * g(v,l) / ( 4 * (n.l) * (n.o)
        float d         = d_ggx( ndoth, roughness );
        float pdfh      = d * g_pi * ndoth; //bring back pi
        float pdf       = pdfh / (4 * ldoth);

        float f         = f_schlick(f0, ldoth );
        float g         = v_smith_ggx(roughness, ndotv, ndotl);
        float weight    = f * g * d;
        
   
        //todo:        
    }
}


#endif
