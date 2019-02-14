#ifndef __shadows_hlsli__
#define __shadows_hlsli__

#include "shadows/moment_shadow_maps_compute.hlsli"
#include "shadows/moment_shadow_maps_32.hlsli"
#include "static_geometry/vector_space.hlsli"

struct sampler_moments_64
{
    Texture2D<float4>  m_shadow_moments_buffer;
    SamplerState	   m_sampler;
};

struct sampler_non_linear_moments_64
{
    Texture2D<uint4>        m_shadow_moments_buffer;
    Texture2DArray<uint2>   m_blue_noise;
};

struct sampler_non_linear_moments_32
{
    Texture2D<uint>         m_shadow_moments_buffer;
    Texture2DArray<uint2>   m_blue_noise;
};

struct view_transforms
{
    euclidean_transform_3d   m_view;
    projective_transform_3d  m_perspective;

    euclidean_transform_3d view()           { return m_view; }
    projective_transform_3d perspective()   { return m_perspective; }
};

struct shadow_view_transforms : view_transforms
{

};

struct main_view_transforms : view_transforms
{

};

struct moments_64_context
{
    shadow_view_transforms  m_shadow_transforms;

    euclidean_transform_3d  shadow_view()        { return m_shadow_transforms.view();}
    projective_transform_3d shadow_perspective() { return m_shadow_transforms.perspective();}
};

struct non_linear_moments_context
{
    shadow_view_transforms  m_shadow_transforms;
    main_view_transforms    m_main_view_transforms;

    uint3                   m_randomness;
    uint2                   m_shadow_buffer_size;
    uint2                   m_view_port_pixel_index;

    uint2                   blue_noise_xy() { return m_randomness.xy; }
    uint                    blue_noise_z() { return m_randomness.z; }

    uint2                   shadow_buffer_size() { return m_shadow_buffer_size; }

    euclidean_transform_3d  shadow_view() { return m_shadow_transforms.view(); }
    projective_transform_3d shadow_perspective() { return m_shadow_transforms.perspective(); }

    euclidean_transform_3d  main_view() { return m_main_view_transforms.view(); }
    projective_transform_3d main_perspective() { return m_main_view_transforms.perspective(); }

};

struct non_linear_moments_64_context : non_linear_moments_context
{
};

struct non_linear_moments_32_context : non_linear_moments_context
{
};

sampler_moments_64 make_sampler_moments_64( Texture2D<float4> t, SamplerState s )
{
   sampler_moments_64 r = { t, s };
   return r;
}

sampler_non_linear_moments_64 make_sampler_non_linear_moments_64(Texture2D<uint4> s, Texture2DArray<float2> noise)
{
    sampler_non_linear_moments_64 r = { s, noise };
    return r;
}

sampler_non_linear_moments_32 make_sampler_non_linear_moments_32(Texture2D<uint> s, Texture2DArray<uint2> noise)
{
    sampler_non_linear_moments_32 r = { s, noise };
    return r;
}

shadow_view_transforms make_shadow_transforms( euclidean_transform_3d v, projective_transform_3d p )
{
    shadow_view_transforms r = { v, p };
    return r;
}

moments_64_context make_moments_64_context(shadow_view_transforms t)
{
    moments_64_context r = {t};
    return r;
}

struct shadow_fragment_depth
{
    float2 m_shadow_buffer_uv;
    float  m_fragment_depth;

    float  fragment_depth() { return m_fragment_depth; }
    float2 shadow_buffer_uv() { return m_shadow_buffer_uv; }
};

shadow_fragment_depth compute_fragment_depth(float3 position_ws, euclidean_transform_3d shadow_view, projective_transform_3d shadow_perspective)
{
    float4 light_ps         = project_p_ws(make_point_ws(position_ws), shadow_view, shadow_perspective).m_value;
    light_ps                = light_ps / light_ps.w;
    float2 shadow_map_uv    = mad(light_ps.xy, float2(0.5f, -0.5f), float2(0.5f, 0.5f));
    float  fragment_depth   = 1.0f - light_ps.z; //shadows are stored in 1-0, not 0-1
    shadow_fragment_depth r = { shadow_map_uv, fragment_depth };
    return r;
}

float compute_moment4_shadow_maps(sampler_moments_64 moments, float3 position_ws, moments_64_context ctx )
{
    shadow_fragment_depth s           = compute_fragment_depth(position_ws, ctx.shadow_view(), ctx.shadow_perspective());
    float4 optimized_moments4         = moments.m_shadow_moments_buffer.Sample( moments.m_sampler, s.shadow_buffer_uv() ) .xyzw;
    float4 moments4                   = compute_deoptimized_moments(optimized_moments4);

    float  shadow_intensity           = compute4_moment_shadow_intensity( moments4, s.fragment_depth());
    shadow_intensity                  = 1.0f - shadow_intensity;
    return shadow_intensity;
}


float compute_nonlinear_moment4_shadow_maps_64(sampler_non_linear_moments_64 moments, float3 position_ws, non_linear_moments_64_context ctx)
{
    shadow_fragment_depth s         = compute_fragment_depth(position_ws, ctx.shadow_view(), ctx.shadow_perspective());

    uint2  view_port_pixel_index    = ctx.m_view_port_pixel_index;
    float2 blue_noise               = moments.m_blue_noise.Load(uint4( ( view_port_pixel_index.xy + ctx.blue_noise_xy() ) & 0x3F, ctx.blue_noise_z() & 0x3F, 0));
    float4 optimized_moments4       = moments.m_shadow_moments_buffer.Load( uint3( s.shadow_buffer_uv() * ctx.shadow_buffer_size() + blue_noise.xy - 0.5f, 0 ) );
    float4 moments4                 = compute_deoptimized_moments(optimized_moments4);

    float  shadow_intensity         = compute4_moment_shadow_intensity(moments4, s.fragment_depth());
    shadow_intensity                = 1.0f - shadow_intensity;

    return shadow_intensity;
}



float compute_nonlinear_moment4_shadow_maps_32(sampler_non_linear_moments_32 moments, float3 position_ws, non_linear_moments_32_context ctx)
{
    shadow_fragment_depth s         = compute_fragment_depth(position_ws, ctx.shadow_view(), ctx.shadow_perspective());
    uint2  view_port_pixel_index    = ctx.m_view_port_pixel_index;

    float2  blue_noise = moments.m_blue_noise.Load(uint4((view_port_pixel_index.xy + ctx.blue_noise_xy()) & 0x3F, ctx.blue_noise_z() & 0x3F, 0)) / float2(63.0f, 63.0f);

    //blue_noise = float2(0.5f, 0.5f);
    uint   packed_distribution      = moments.m_shadow_moments_buffer.Load(uint3(s.shadow_buffer_uv() * ctx.shadow_buffer_size() + blue_noise.xy - 0.5f, 0));

    float2 out_depth;
    float  out_weight;
    float  out_fourth_moment;

    unpack_moments_non_linear_32_bit(out_depth, out_weight, out_fourth_moment, packed_distribution);

    float  depth_bias       = 0.0010f;
    float  biased_depth     = s.fragment_depth() - depth_bias;
    float  shadow_intensity = compute_moment4_non_linear_lower_bound(biased_depth, out_depth,out_weight, out_fourth_moment);
    shadow_intensity        = 1.0f - shadow_intensity;

    return shadow_intensity;
}

    


#endif
