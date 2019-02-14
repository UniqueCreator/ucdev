#ifndef __moment_shadow_maps_64_hlsli__
#define __moment_shadow_maps_64_hlsli__

#include "../default_signature.hlsli"

#include "moment_shadow_maps_utils.hlsli"
 


/*! Given a vector of moments from signed depths with single precision and no 
   biasing this function turns it into a compact representation using only four 
   floats which may be packed into 64 bits using four 16-bit fixed point values 
   from zero to one. This is accomplished by a non-linear quantization transform.*/
float4 quantize_moments_non_linear_64_bit(float4 UnbiasedMoments)
{
	float2 Depth;
	float Weight,FourthMomentOffset;
    prepare_non_linear_moment_quantization(Depth,Weight,FourthMomentOffset,UnbiasedMoments);
	return float4(mad(Depth,0.5f,0.5f),Weight, warp_fourth_moment_offset(FourthMomentOffset));
}

/*! Takes the output of QuantizeMomentsNonLinear32Bit() and reconstructs the output
of PrepareNonLinearMomentQuantization() except for quantization errors.*/
void unpack_moments_non_linear_64_bit(out float2 OutDepth, out float OutWeight, out float OutFourthMomentOffset, float4 PackedDistribution)
{
    OutDepth=mad(PackedDistribution.xy,2.0f,-1.0f);
    OutWeight=PackedDistribution.z;
    OutFourthMomentOffset= unwarp_fourth_moment_offset(PackedDistribution.w);
}

void apply_resolve_non_linear_64_bit(inout RWTexture2D<unorm float4> shadow_moments, Texture2DMS<float, 4> shadows_buffer, uint3 dtid : SV_DispatchThreadID)
{
    const uint2 location = dtid.xy;

    float z0 = 1.0f - shadows_buffer.Load(location, 0);
    float z1 = 1.0f - shadows_buffer.Load(location, 1);
    float z2 = 1.0f - shadows_buffer.Load(location, 2);
    float z3 = 1.0f - shadows_buffer.Load(location, 3);

    float moment_0 = (z0 + z1 + z2 + z3) / 4;
    float moment_1 = (p2(z0) + p2(z1) + p2(z2) + p2(z3)) / 4;
    float moment_2 = (p3(z0) + p3(z1) + p3(z2) + p3(z3)) / 4;
    float moment_3 = (p4(z0) + p4(z1) + p4(z2) + p4(z3)) / 4;

    float4 unbiased_moments = float4(moment_0, moment_1, moment_2, moment_3);

    shadow_moments[location] = quantize_moments_non_linear_64_bit(unbiased_moments);
}

#endif
