#include "../default_signature.hlsli"
#include "../shadows.hlsli"
#include "moment_shadow_maps_32.hlsli"

/*! @{
These constant are meant to make the code in the compute shader for the resolve
and blur (RAB) more meaningful. In most cases, further modifications to the code
are necessary if any of these constants is changed.*/
static const uint RABOutputBlockHeight      = 24;
static const uint RABOutputBlockWidth       = 32;
static const uint RABGuardBandSize          = 8;
static const uint RABInputBlockWidth        = RABOutputBlockWidth + RABGuardBandSize;
static const uint RABInputBlockHeight       = RABOutputBlockHeight + RABGuardBandSize;
static const uint RABSpacerStride           = 4;
static const uint RABHalfInputBlockWidth    = RABInputBlockWidth / 2;
static const uint nRABMoment                = 4;
static const uint nRABThread                = 256;
static const uint nRABHalfThread            = nRABThread / 2;
static const uint nRABInputPerThread        = (RABInputBlockWidth*RABInputBlockHeight) / nRABThread;
static const uint nRABOutputPerThread       = (RABOutputBlockWidth*RABOutputBlockHeight) / nRABThread;
static const uint RABMomentPitch            = RABInputBlockWidth*RABInputBlockHeight + RABInputBlockHeight / RABSpacerStride - 1;
//! @}

/*! This array stores all moments for the resolve and blur. Initially these are the
source moments that come out of the resolve, then they are blurred in two passes.
To query the moment for power j+1 at coordinates x,y, you have to access index
j*RABMomentPitch+(x%RABHalfInputBlockWidth)*2+y*RABInputBlockWidth+y/RABSpacerStride+x/RABHalfInputBlockWidth.
This slightly intricate indexing scheme is needed to avoid bank conflicts.
Note how the left and the right half of the block are interleaved and that
every RABSpacerStride-th row ends with one unused entry for spacing. The
number of entries per moment, including unused spacers, is denoted by this
variable.*/
groupshared float pRABMoment[RABMomentPitch*nRABMoment];

/*! Filter weights for a Gaussian filter with standard deviation 2.4 and a width of
9 samples (centered at sample 4).*/
static const float pRABGaussianWeight[9] = { 0.0440444341987f,0.0808695919535f,0.124819121228f,0.161949138463f,0.176635428315f,0.161949138463f,0.124819121228f,0.0808695919535f,0.0440444341987f };


/*! This function is a helper for all variants of ApplyResolveAndGaussian1_32Bit()
including the 64-bit variants. It takes the moments in shared memory and applies
a 9x9 Gaussian blur with standard deviation 2.4. The result for the left half is
written back to the left top, the result for the right half is written to the
right top.
\param GroupThreadID The 3D index of the invoking thread within its group.*/
void apply_gaussian_filter_9x9(const uint3 GroupThreadID)
{
    // The left half is overwritten by the first half of the warps, the right half 
    // by the second half. Each thread only operates on one of the four moments. The 
    // strip of width RABGuardBandSize in the middle is read only. Thus, each warp 
    // has a distinct region it writes to and no race conditions can occur.
    bool RightHalf      = (GroupThreadID.y >= nRABHalfThread / RABInputBlockHeight);
    uint iThreadMoment  = GroupThreadID.y%nRABMoment;
    uint MomentOffset   = iThreadMoment*RABMomentPitch;

    // Each thread loads RABOutputBlockWidth/2+RABGuardBandSize moments into 
    // registers to blur them horizontally
    float pRowMoment[RABOutputBlockWidth / 2 + RABGuardBandSize];
    // We begin with the RABGuardBandSize/2 left-most samples
    uint iRow = GroupThreadID.x;
    uint RowOffset = iRow*RABInputBlockWidth + iRow / RABSpacerStride + MomentOffset;
    uint iMomentInMemory = RightHalf ? (RABOutputBlockWidth + RowOffset) : RowOffset;
    for (uint i = 0; i != RABGuardBandSize / 2; ++i)
    {
        pRowMoment[i] = pRABMoment[iMomentInMemory + 2 * i];
    }
    // Now the threads for the right half have to transition from the left half 
    // to the right half while the other threads just move on
    iMomentInMemory = RightHalf ? (iMomentInMemory + 1 - 2 * (RABOutputBlockWidth / 2)) : (iMomentInMemory + 2 * (RABGuardBandSize / 2));
    for (uint i = RABGuardBandSize / 2; i != RABInputBlockWidth / 2; ++i)
    {
        pRowMoment[i] = pRABMoment[iMomentInMemory + 2 * (i - RABGuardBandSize / 2)];
    }
    // Finally the threads for the left half may need to transition to the right 
    // half while the other threads just move on
    iMomentInMemory = RightHalf ? (iMomentInMemory + 2 * (RABHalfInputBlockWidth - RABGuardBandSize / 2)) : (iMomentInMemory + 1 - RABGuardBandSize);
    for (uint i = RABHalfInputBlockWidth; i != RABOutputBlockWidth / 2 + RABGuardBandSize; ++i)
    {
        pRowMoment[i] = pRABMoment[iMomentInMemory + 2 * (i - RABHalfInputBlockWidth)];
    }
    // Now we compute blurred moments
    float pRowBlurredMoment[RABOutputBlockWidth / 2];
    
    for (uint i = 0; i != RABOutputBlockWidth / 2; ++i)
    {
        pRowBlurredMoment[i] = 0.0f;
        for (uint j = 0; j != 9; ++j)
        {
            pRowBlurredMoment[i] += pRABGaussianWeight[j] * pRowMoment[i + j];
        }
    }
    // And write them back to shared memory without overwriting the read-only 
    // region
    iMomentInMemory = RightHalf ? (RABGuardBandSize + 1 + RowOffset) : RowOffset;
    for (uint i = 0; i != RABOutputBlockWidth / 2; ++i)
    {
        pRABMoment[iMomentInMemory + 2 * i] = pRowBlurredMoment[i];
    }
    // If the warp size on the target platform is not a multiple of 32, this 
    // synchronization has to be uncommented to avoid malicious race conditions
    //GroupMemoryBarrierWithGroupSync();
    // The vertical blur is next. For this step each thread still operates on either 
    // the left or the right half but additionally threads choose the upper or lower 
    // half. Each thread has to load RABOutputBlockHeight/2+RABGuardBandSize moments 
    // from a column
    float pColumnMoment[RABOutputBlockHeight / 2 + RABGuardBandSize];
    uint iColumn = GroupThreadID.x % (RABOutputBlockWidth / 2);
    iColumn = (RightHalf ? (iColumn + RABOutputBlockWidth / 2 + RABGuardBandSize) : iColumn);
    bool LowerHalf = (GroupThreadID.x >= RABOutputBlockWidth / 2);
    uint iFirstRow = LowerHalf ? (RABOutputBlockHeight / 2) : 0;
    uint iFirstMomentInMemory = iColumn * 2 + iFirstRow*RABInputBlockWidth + iFirstRow / RABSpacerStride + MomentOffset;
    iFirstMomentInMemory = (RightHalf ? (iFirstMomentInMemory + 1 - RABHalfInputBlockWidth * 2) : iFirstMomentInMemory);
    for (uint i = 0; i != RABOutputBlockHeight / 2 + RABGuardBandSize; ++i)
    {
        // This index arithmetic may look expensive but it deals with nothing 
        // but compile-time constants, so it is free
        uint iMemoryOffset = i*RABInputBlockWidth + i / RABSpacerStride;
        pColumnMoment[i] = pRABMoment[iFirstMomentInMemory + iMemoryOffset];
    }
    // Now we compute blurred moments
    float pColumnBlurredMoment[RABOutputBlockHeight / 2];
    for (uint i = 0; i != RABOutputBlockHeight / 2; ++i)
    {
        pColumnBlurredMoment[i] = 0.0f;
        for (uint j = 0; j != 9; ++j)
        {
            pColumnBlurredMoment[i] += pRABGaussianWeight[j] * pColumnMoment[i + j];
        }
    }
    // If the warp size on the target platform is not a multiple of 32, this 
    // synchronization has to be uncommented to avoid malicious race conditions
    //GroupMemoryBarrierWithGroupSync();
    // And write them back to shared memory
    for (uint i = 0; i != RABOutputBlockHeight / 2; ++i)
    {
        uint iMemoryOffset = i*RABInputBlockWidth + i / RABSpacerStride;
        pRABMoment[iFirstMomentInMemory + iMemoryOffset] = pColumnBlurredMoment[i];
    }
}


/*! Given a scalar this function outputs a vector consisting of the first, second,
third and fourth power of this scalar linearly transformed using an optimized
basis for optimal quantization.*/
float4 compute_moment_vector4_moments_optimized(float moment_0, float moment_1, float moment_2, float moment_3)
{
    const float4 moments = float4(moment_0, moment_1, moment_2, moment_3);
    const float4x4 transform = float4x4
        (
            -2.07224649f, 13.7948857237f, 0.105877704f, 9.7924062118f,
            32.23703778f, -59.4683975703f, -1.9077466311f, -33.7652110555f,
            -68.571074599f, 82.0359750338f, 9.3496555107f, 47.9456096605f,
            39.3703274134f, -35.364903257f, -6.6543490743f, -23.9728048165f
            );

    float4 moments_optimized = mul(moments, transform);

    //add bias
    moments_optimized[0] += 0.035955884801f;

    return moments_optimized;
}

void apply_resolve_gaussian8_64_bit(inout RWTexture2D<unorm float4> OutMomentShadowMap, Texture2DMS<float, 4> ShadowMap, uint3 GroupThreadID, uint3 GroupID)
{

    // In general the output block size is not a divisor of the shadow map 
    // dimensions, so we need the actual dimensions to disable threads appropriately
    uint OutputWidth, OutputHeight;
    OutMomentShadowMap.GetDimensions(OutputWidth, OutputHeight);
    uint2 MaxCoordinate = uint2(OutputWidth - 1, OutputHeight - 1);

    // Generate a moment shadow map of the input block in groupshared memory. Each 
    // warp loads one column at a time. The first half of the warps works in the 
    // left half of the block.
    bool RightHalf = (GroupThreadID.y >= nRABHalfThread / RABInputBlockHeight);
    uint2 iInputPosition = uint2(GroupThreadID.y, GroupThreadID.x);

    iInputPosition.x = RightHalf ? (iInputPosition.x + RABHalfInputBlockWidth - nRABHalfThread / RABInputBlockHeight) : iInputPosition.x;
    uint2 iSourceTexel = uint2( GroupID.x*RABOutputBlockWidth + iInputPosition.x - RABGuardBandSize / 2, clamp(GroupID.y*RABOutputBlockHeight + iInputPosition.y - RABGuardBandSize / 2, 0, MaxCoordinate.y) );
    uint iMomentInMemory = iInputPosition.x * 2 + iInputPosition.y*RABInputBlockWidth + iInputPosition.y / RABSpacerStride;
    iMomentInMemory = (RightHalf ? (iMomentInMemory + 1 - 2 * RABHalfInputBlockWidth) : iMomentInMemory);
    
    for (uint i = 0; i != nRABInputPerThread; ++i, iMomentInMemory += 2 * (nRABHalfThread / RABInputBlockHeight), iSourceTexel.x += nRABHalfThread / RABInputBlockHeight)
    {
        uint    ClampedX    = clamp(iSourceTexel.x, 0, MaxCoordinate.x);

        float z0 = 1.0f - ShadowMap.Load(uint2(ClampedX, iSourceTexel.y), 0);
        float z1 = 1.0f - ShadowMap.Load(uint2(ClampedX, iSourceTexel.y), 1);
        float z2 = 1.0f - ShadowMap.Load(uint2(ClampedX, iSourceTexel.y), 2);
        float z3 = 1.0f - ShadowMap.Load(uint2(ClampedX, iSourceTexel.y), 3);

        float moment_0 = (z0 + z1 + z2 + z3) / 4;
        float moment_1 = (p2(z0) + p2(z1) + p2(z2) + p2(z3)) / 4;
        float moment_2 = (p3(z0) + p3(z1) + p3(z2) + p3(z3)) / 4;
        float moment_3 = (p4(z0) + p4(z1) + p4(z2) + p4(z3)) / 4;

        pRABMoment[0 * RABMomentPitch + iMomentInMemory] = moment_0;
        pRABMoment[1 * RABMomentPitch + iMomentInMemory] = moment_1;
        pRABMoment[2 * RABMomentPitch + iMomentInMemory] = moment_2;
        pRABMoment[3 * RABMomentPitch + iMomentInMemory] = moment_3;
    }
    // Now we perform the blur. Before all of this can start, we have to wait for 
    // writes to finish.
    GroupMemoryBarrierWithGroupSync();
    apply_gaussian_filter_9x9(GroupThreadID);
    // Blurring is done and we are ready to write out the results but first we have 
    // to sync
    GroupMemoryBarrierWithGroupSync();
    // Each thread loads three vectors of moments from groupshared memory. This 
    // time one warp processes one row at a time because after filtering rows 
    // conveniently have the same length as one warp. 
    uint2 iOutputPosition = uint2(GroupThreadID.x, GroupThreadID.y);

    uint2 iOutputTexel = uint2(GroupID.x*RABOutputBlockWidth + iOutputPosition.x, GroupID.y*RABOutputBlockHeight + iOutputPosition.y);
    uint2 iSourcePosition = iOutputPosition;

    RightHalf = (iSourcePosition.x >= RABOutputBlockWidth / 2);

    iSourcePosition.x = RightHalf ? (iSourcePosition.x + RABGuardBandSize) : iSourcePosition.x;
    iMomentInMemory = iSourcePosition.x * 2 + iSourcePosition.y*RABInputBlockWidth + iSourcePosition.y / RABSpacerStride;
    iMomentInMemory = (RightHalf ? (iMomentInMemory + 1 - 2 * RABHalfInputBlockWidth) : iMomentInMemory);
    const uint iMemoryOffset = (nRABThread / RABOutputBlockWidth)*RABInputBlockWidth + nRABThread / (RABOutputBlockWidth*RABSpacerStride);
    float4 pMomentVector[nRABOutputPerThread];

    for (uint i = 0; i != nRABOutputPerThread; ++i, iMomentInMemory += iMemoryOffset)
    {
        pMomentVector[i].x = pRABMoment[0 * RABMomentPitch + iMomentInMemory];
        pMomentVector[i].y = pRABMoment[1 * RABMomentPitch + iMomentInMemory];
        pMomentVector[i].z = pRABMoment[2 * RABMomentPitch + iMomentInMemory];
        pMomentVector[i].w = pRABMoment[3 * RABMomentPitch + iMomentInMemory];
    }

    // Now each thread converts its moment vectors into a compact representation
    float4 pPackedDistribution[nRABOutputPerThread];
    for (uint i = 0; i != nRABOutputPerThread; ++i)
    {
        float m0 = pMomentVector[i].x;
        float m1 = pMomentVector[i].y;
        float m2 = pMomentVector[i].z;
        float m3 = pMomentVector[i].w;

        pPackedDistribution[i] = compute_moment_vector4_moments_optimized(m0, m1, m2, m3);
    }

    // And finally it outputs it
    for (uint i = 0; i != nRABOutputPerThread; ++i, iOutputTexel.y += nRABThread / RABOutputBlockWidth)
    {
        if (all(iOutputTexel <= MaxCoordinate))
        {
            OutMomentShadowMap[iOutputTexel] = pPackedDistribution[i];
        }
    }
}

void apply_resolve_64_bit(inout RWTexture2D<unorm float4> shadow_moments, Texture2DMS<float, 4> shadows_buffer, uint3 dtid : SV_DispatchThreadID)
{
    const uint2 location = dtid.xy;

    float z0 = 1.0f - shadows_buffer.Load(location,0);
    float z1 = 1.0f - shadows_buffer.Load(location,1);
    float z2 = 1.0f - shadows_buffer.Load(location,2);
    float z3 = 1.0f - shadows_buffer.Load(location,3);

    float moment_0 = (z0 + z1 + z2 + z3) / 4;
    float moment_1 = (p2(z0) + p2(z1) + p2(z2) + p2(z3)) / 4;
    float moment_2 = (p3(z0) + p3(z1) + p3(z2) + p3(z3)) / 4;
    float moment_3 = (p4(z0) + p4(z1) + p4(z2) + p4(z3)) / 4;

    shadow_moments[location] = compute_moment_vector4_moments_optimized( moment_0, moment_1, moment_2, moment_3);
}







