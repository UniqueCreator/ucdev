#ifndef __moment_shadow_maps_compute_hlsli__
#define __moment_shadow_maps_compute_hlsli__

/*! Given a sampled value from a four-moment shadow map and a computed shadow map
depth for a point at the same location this function outputs 1.0, if the fragment
is in shadow 0.0f, if the fragment is lit and an intermediate value for partial
shadow. The returned value is an optimal lower bound.*/
float compute4_moment_shadow_intensity(float4 _4Moments, float FragmentDepth, float DepthBias = 0.0f, float MomentBias = 0.00003f)
{
    float OutShadowIntensity;
    // Bias input data to avoid artifacts
    float4 b;
    
    b.x = lerp(_4Moments.x, 0.5f, MomentBias);
    b.y = lerp(_4Moments.y, 0.5f, MomentBias);
    b.z = lerp(_4Moments.z, 0.5f, MomentBias);
    b.w = lerp(_4Moments.w, 0.5f, MomentBias);
    
//    float4 b = lerp(_4Moments, float4(0.5f, 0.5f, 0.5f, 0.5f), MomentBias);

    // Use short-hands for the many formulae to come
    //float4 b = Biased4Moments;
    float3 z;
    z[0] = FragmentDepth - DepthBias;

    // Compute a Cholesky factorization of the Hankel matrix B storing only non-
    // trivial entries or related products
    float L21D11 = mad(-b[0], b[1], b[2]);
    float D11 = mad(-b[0], b[0], b[1]);
    float InvD11 = rcp(D11);
    float L21 = L21D11*InvD11;
    float SquaredDepthVariance = mad(-b[1], b[1], b[3]);
    float D22 = mad(-L21D11, L21, SquaredDepthVariance);

    // Obtain a scaled inverse image of bz=(1,z[0],z[0]*z[0])^T
    float3 c = float3(1.0f, z[0], z[0] * z[0]);
    // Forward substitution to solve L*c1=bz
    c[1] -= b.x;
    c[2] -= b.y + L21*c[1];
    // Scaling to solve D*c2=c1
    c[1] *= InvD11;
    c[2] /= D22;
    // Backward substitution to solve L^T*c3=c2
    c[1] -= L21*c[2];
    c[0] -= dot(c.yz, b.xy);
    // Solve the quadratic equation c[0]+c[1]*z+c[2]*z^2 to obtain solutions z[1] 
    // and z[2]
    float InvC2 = rcp(c[2]);
    float p = c[1] * InvC2;
    float q = c[0] * InvC2;
    float D = ((p*p) / 4.0f) - q;
    float r = sqrt(D);
    z[1] = -(p / 2.0f) - r;
    z[2] = -(p / 2.0f) + r;

    // Use a solution made of four deltas if the solution with three deltas is 
    // invalid
    [branch] if (z[1]<-1.0f || z[2]>1.0f) {
        float zFree = ((b[0] - b[2])*z[0] + b[3] - b[1]) / (z[0] + b[2] - b[0] - b[1] * z[0]);
        float w1Factor = (z[0]>zFree) ? 1.0f : 0.0f;
        // Construct a polynomial taking value zero at z[0] and 1, value 1 at -1 and 
        // value w1Factor at zFree. Start with a linear part and then multiply by 
        // linear factors to get the roots.
        float2 Normalizers;
        Normalizers.x = w1Factor / ((zFree - z[0])*mad(zFree, zFree, -1.0f));
        Normalizers.y = 0.5f / ((zFree + 1.0f)*(z[0] + 1.0f));
        float4 Polynomial;
        Polynomial[0] = mad(zFree, Normalizers.y, Normalizers.x);
        Polynomial[1] = Normalizers.x - Normalizers.y;
        // Multiply the polynomial by (z-z[0])
        Polynomial[2] = Polynomial[1];
        Polynomial[1] = mad(Polynomial[1], -z[0], Polynomial[0]);
        Polynomial[0] *= -z[0];
        // Multiply the polynomial by (z-1)
        Polynomial[3] = Polynomial[2];
        Polynomial.yz = Polynomial.xy - Polynomial.yz;
        Polynomial[0] *= -1.0f;
        // The shadow intensity is the dot product of the coefficients of this 
        // polynomial and the power moments for the respective powers
        OutShadowIntensity = dot(Polynomial, float4(1.0f, b.xyz));
    }
    // Use the solution with three deltas
    else {
        float4 Switch =
            (z[2]<z[0]) ? float4(z[1], z[0], 1.0f, 1.0f) : (
            (z[1]<z[0]) ? float4(z[0], z[1], 0.0f, 1.0f) :
                float4(0.0f, 0.0f, 0.0f, 0.0f));
        float Quotient = (Switch[0] * z[2] - b[0] * (Switch[0] + z[2]) + b[1]) / ((z[2] - Switch[1])*(z[0] - z[1]));
        OutShadowIntensity = Switch[2] + Switch[3] * Quotient;
    }
    OutShadowIntensity = saturate(OutShadowIntensity);

    return OutShadowIntensity;
}

/*! This function takes a sample from the four-moment optimized shadow map in the
given sampler using the given texture coordinate converts to moments and outputs
it. The shadow map dimensions and reciproque dimensions are needed to correct
off-by-one errors.*/
float4 compute_deoptimized_moments(float4 optimized_moments)
{
    float4 OptimizedSample = optimized_moments;
    OptimizedSample[0] -= 0.035955884801f;
    float4 Out4Moments = mul(OptimizedSample, float4x4(
        0.2227744146f, 0.1549679261f, 0.1451988946f, 0.163127443f,
        0.0771972861f, 0.1394629426f, 0.2120202157f, 0.2591432266f,
        0.7926986636f, 0.7963415838f, 0.7258694464f, 0.6539092497f,
        0.0319417555f, -0.1722823173f, -0.2758014811f, -0.3376131734f));

    return Out4Moments;
}



#endif
