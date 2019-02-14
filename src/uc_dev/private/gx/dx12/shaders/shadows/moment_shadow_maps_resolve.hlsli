#include "../default_signature.hlsli"
#include "../shadows.hlsli"

#include "moment_shadow_maps.hlsli"
#include "moment_shadow_maps_32.hlsli"
#include "moment_shadow_maps_64.hlsli"

#if defined(MOMENT_SHADOW_MAPS)

Texture2DMS<float, 4>        g_shadows_buffer:register(t1);
RWTexture2D<unorm float4>    g_shadow_moments:register(u1);

[numthreads( 32, 8, 1 )]
[RootSignature( MyRS2 ) ]
void main( uint3 dtid : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint3 GroupID : SV_GroupID)
{
   apply_resolve_gaussian8_64_bit(g_shadow_moments, g_shadows_buffer, GroupThreadID, GroupID);
}

#endif

#if defined(NON_LINEAR_MOMENT_SHADOW_MAPS_32)

Texture2DMS<float, 4>        g_shadows_buffer:register(t1);
RWTexture2D<uint>            g_shadow_moments:register(u1);

[numthreads(8, 8, 1)]
[RootSignature(MyRS2)]
void main(uint3 dtid : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint3 GroupID : SV_GroupID)
{
    apply_resolve_non_linear_32_bit(g_shadow_moments, g_shadows_buffer, dtid);
}

#endif

#if defined(NON_LINEAR_MOMENT_SHADOW_MAPS_64)

Texture2DMS<float, 4>        g_shadows_buffer:register(t1);
RWTexture2D<unorm float4>    g_shadow_moments:register(u1);

[numthreads(8, 8, 1)]
[RootSignature(MyRS2)]
void main(uint3 dtid : SV_DispatchThreadID, uint3 GroupThreadID : SV_GroupThreadID, uint3 GroupID : SV_GroupID)
{
    apply_resolve_non_linear_64_bit(g_shadow_moments, g_shadows_buffer, dtid);
}

#endif

