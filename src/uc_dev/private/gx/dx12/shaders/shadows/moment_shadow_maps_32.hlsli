#ifndef __moment_shadow_maps_32_hlsli__
#define __moment_shadow_maps_32_hlsli__

#include "../default_signature.hlsli"

#include "moment_shadow_maps_utils.hlsli"
#include "moment_shadow_maps_non_linear.hlsli"
 

/*! Given a vector of moments from signed depths with single precision and no
biasing this function turns it into a compact representation using only 32 bits.
This is accomplished by a non-linear quantization transform.*/
uint quantize_moments_non_linear_32_bit(float4 unbiased_moments)
{
    float2 Depth;
    float Weight;
    float FourthMomentOffset;

    prepare_non_linear_moment_quantization(Depth, Weight, FourthMomentOffset, unbiased_moments);

    // The weight often maps to a shadow intensity directly, so quantizing the 
    // square root makes the banding perceptionally less problematic
    float WeightPrepared = sqrt(saturate(Weight));
    float FourthMomentOffsetPrepared = warp_fourth_moment_offset(FourthMomentOffset);
    uint WeightQuantized = min(uint(WeightPrepared*pow(2.0f, 7.0f)), pow(2, 7) - 1);
    uint FourthMomentOffsetQuantized = min(uint(FourthMomentOffsetPrepared*pow(2.0f, 6.0f)), pow(2, 6) - 1);

    // Quantize depth to 10 bits per value using only values from 0 to 1022 to 
    // enable exploitation of Depth[0]<Depth[1]
    uint2 DepthQuantized = uint2(mad(clamp(Depth, -1.0f, 0.999f), 511.5f, 511.5f));
    // Save one bit by exploiting Depth[0]<Depth[1]
    DepthQuantized = (DepthQuantized[0] >= 512) ? (uint2(1023, 1022) - DepthQuantized) : DepthQuantized;
    // Pack everything into a single integer and write the result
    return (DepthQuantized[0] << 23 | DepthQuantized[1] << 13 | WeightQuantized << 6 | FourthMomentOffsetQuantized);
}


/*! Takes the output of QuantizeMomentsNonLinear32Bit() and reconstructs the output
of PrepareNonLinearMomentQuantization() except for quantization errors.*/
void unpack_moments_non_linear_32_bit(out float2 OutDepth, out float OutWeight, out float OutFourthMomentOffset, uint PackedDistribution)
{
    uint2 DepthQuantized    = (PackedDistribution&uint2(0xFF800000, 0x007FE000)) >> uint2(23, 13);
    DepthQuantized          = (DepthQuantized[0]>DepthQuantized[1]) ? (uint2(1023, 1022) - DepthQuantized) : DepthQuantized;
    OutDepth                = mad(float2(DepthQuantized), 1.0f / 511.5f, -1.0f + 2.0f / 1023.0f);
    OutWeight               = mad(float(PackedDistribution & 0x00001FC0), pow(0.5f, 6.0f + 7.0f), pow(0.5f, 7.0f + 1.0f));
    OutWeight               *= OutWeight;
    OutFourthMomentOffset   = unwarp_fourth_moment_offset(mad(float(PackedDistribution & 0x0000003F), pow(0.5f, 6.0f), pow(0.5f, 6.0f)));
}

void apply_resolve_non_linear_32_bit(inout RWTexture2D<uint> shadow_moments, Texture2DMS<float, 4> shadows_buffer, uint3 dtid : SV_DispatchThreadID)
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

    shadow_moments[location] = quantize_moments_non_linear_32_bit(unbiased_moments);
}

#endif
