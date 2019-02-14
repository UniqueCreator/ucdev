#ifndef __moment_shadow_maps_non_linear_hlsli__
#define __moment_shadow_maps_non_linear_hlsli__

#include "../default_signature.hlsli"

#include "moment_shadow_maps_utils.hlsli"
 

/*! Values below this threshold are clamped since this scarcely ever changes the
lower bounds significantly but saves space.
\sa WarpFourthMomentOffset()*/
static const float MinFourthMomentOffset = 1.2e-7f;
/*! Since we only consider moments resulting from depths between -1 and 1, the
fourth moment offset has a global upper bound.
\sa WarpFourthMomentOffset()*/
static const float MaxFourthMomentOffset = 0.25f;
/*! This value would map to minus infinity and is used for normalization.
\sa WarpFourthMomentOffset()*/
static const float FourthMomentOffsetSingularity = 1.0e-7f;



/*! This is a helper function for QuantizeMomentsNonLinear32Bit() and its variants.
With the outputs lerp(pow(OutDepth.x,j),pow(OutDepth.y,j),OutWeight) equals the
j-th biased moment for j in {0,1,2,3}. For the fourth moment
OutFourthMomentOffset has to be added.*/
void prepare_non_linear_moment_quantization(out float2 OutDepth, out float OutWeight, out float OutFourthMomentOffset, float4 UnbiasedMoments)
{
    // Apply very slight biasing right now
    float4 b = lerp(UnbiasedMoments, float4(0.0f, 0.375f, 0.0f, 0.375f), 3.0e-7f);

    // Apply a Vandermonde decomposition
    float3 Polynomial;
    float Variance = mad(-b.x, b.x, b.y);
    Polynomial.z = Variance;
    Polynomial.y = mad(b.x, b.y, -b.z);
    Polynomial.x = dot(-b.xy, Polynomial.yz);

    float InvVariance = 1.0f / Polynomial[2];
    float p = Polynomial[1] * InvVariance;
    float q = Polynomial[0] * InvVariance;
    float D = p*p*0.25f - q;
    float r = sqrt(D);

    OutDepth = float2(-0.5f*p - r, -0.5f*p + r);
    OutWeight = (b.x - OutDepth[0]) / (OutDepth[1] - OutDepth[0]);

    // Compute small parts of a Choleky factorization to get the offset of the fourth 
    // moment

    float L21D11 = mad(-b.x, b.y, b.z);
    float SquaredDepthVariance = mad(-b.y, b.y, b.w);
    OutFourthMomentOffset = mad(-L21D11, L21D11*InvVariance, SquaredDepthVariance);
}

/*! This function warps the fourth moment offset which is output by
PrepareNonLinearMomentQuantization() in a non-linear fashion that will make the
distribution far more uniform. Small values (below 1.2e-7) are clamped. Output
values are guaranteed to be in the range from zero to one.
\sa UnwarpFourthMomentOffset() */
float warp_fourth_moment_offset(float FourthMomentOffset)
{
    // Compute the coefficients for the linear transform mapping to [0,1] (these are 
    // compile-time constants)
    const float Factor = 1.0f / (log(log(MaxFourthMomentOffset / FourthMomentOffsetSingularity)) - log(log(MinFourthMomentOffset / FourthMomentOffsetSingularity)));
    const float Summand = -log(log(MinFourthMomentOffset / FourthMomentOffsetSingularity))*Factor;
    // Apply the warp
    return saturate(mad(log(log(max(MinFourthMomentOffset, FourthMomentOffset) / FourthMomentOffsetSingularity)), Factor, Summand));
}


/*! This is the inverse function of WarpFourthMomentOffset(). It fulfills
UnwarpFourthMomentOffset(WarpFourthMomentOffset(x))==x for x>=1.2e-7.
\sa WarpFourthMomentOffset() */
float unwarp_fourth_moment_offset(float WarpedFourthMomentOffset)
{
    // In the end, a linear transform has to map the value to the range from zero to 
    // one
    const float Summand = log(log(MinFourthMomentOffset / FourthMomentOffsetSingularity));
    const float Factor = log(log(MaxFourthMomentOffset / FourthMomentOffsetSingularity)) - Summand;
    return exp(exp(mad(WarpedFourthMomentOffset, Factor, Summand)))*FourthMomentOffsetSingularity;
}

/*! This function computes a lower bound to the cumulative distribution function on
the reals from four non-linearly quantized moments. The moments of order j=0 to
j=3 are given by lerp(pow(Support[0],j),pow(Support[1],j),Weight). The fourth
moment is given by the same formula except that FourthMomentOffset has to be
added. The lower bound is computed for the interval (-infinity,IntervalEnd].*/
float compute_moment4_non_linear_lower_bound(float IntervalEnd, float2 Support, float Weight, float FourthMomentOffset)
{
    // Avoid zero variance
    const float ClampingOffset = 5.0e-6f;
    Weight = clamp(Weight, ClampingOffset, 1.0f - ClampingOffset);
    Support.y = max(Support.x + ClampingOffset, Support.y);

    // Is the fragment fully lit?
    [branch] if (IntervalEnd <= Support.x)
    {
        return 0.0f;
    }
    // Normalize the interval end such that Support can be treated as 
    // float2(0.0f,1.0f). The fourth moment offset needs to be scaled accordingly.
    const float Scaling = rcp(Support[1] - Support[0]);
    const float NormalizedIntervalEnd = (IntervalEnd - Support[0])*Scaling;
    const float ScalingSquared = Scaling*Scaling;

    FourthMomentOffset *= ScalingSquared*ScalingSquared;
    // Prepare a few quantities that will be needed repeatedly
    const float InvWeight0 = rcp(1.0f - Weight);
    const float InvWeight1 = rcp(Weight);
    const float InvFourthMomentOffset = rcp(FourthMomentOffset);
    // Is the most complicated case present?
    float Root          = NormalizedIntervalEnd;
    float FlippedRoot   = 1.0f - Root;
    float2 OutputTransform = float2(-1.0f, 1.0f);

    [branch] if (NormalizedIntervalEnd<1.0f)
    {
        const float q = -FourthMomentOffset*InvWeight0 / NormalizedIntervalEnd;
        const float pHalf = mad(-0.5f*q, mad(NormalizedIntervalEnd, -InvWeight1, 1.0f) / FlippedRoot, -0.5f);
        Root = -pHalf - sqrt(mad(pHalf, pHalf, -q));
        FlippedRoot = 1.0f - Root;
        OutputTransform = float2(1.0f, 0.0f);
    }

    // Compute the weight
    const float RootSquared = Root*Root;
    const float FlippedRootSquared = FlippedRoot*FlippedRoot;
    const float RootWeight = 1.0f / dot( float3(InvWeight0, InvWeight1, InvFourthMomentOffset), float3(FlippedRootSquared, RootSquared, RootSquared*FlippedRootSquared));

    return mad(RootWeight, OutputTransform.x, OutputTransform.y);
}

#endif
