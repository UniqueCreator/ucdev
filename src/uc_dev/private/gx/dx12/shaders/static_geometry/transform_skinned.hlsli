#ifndef __transform_skinned_hlsli__
#define __transform_skinned_hlsli__

//todo: replace with quaternions
float4 skin_position(float4 position, float4x4 joint_transform, float weight)
{
    return mul( position, joint_transform) * weight;
}

//todo: rework
float3 skin_normal(float3 normal, float4x4 joint_transform, float weight)
{
    return ( mul( float4(normal, 0.0), joint_transform ) * weight ).xyz;
}

float4 skin_position(float4 position, float4 weights, float4 indices, float4x4 joints[127])
{
    float4 skinned_position = position;

    skinned_position =  skin_position(position, joints[indices.x], weights.x);
    skinned_position += skin_position(position, joints[indices.y], weights.y);
    skinned_position += skin_position(position, joints[indices.z], weights.z);
    skinned_position += skin_position(position, joints[indices.w], weights.w);

    skinned_position /= (weights.x + weights.y + weights.z + weights.w);

    return skinned_position;
}

float3 skin_normal(float3 normal, float4 weights, float4 indices, float4x4 joints[127])
{
    float3 skinned_normal = normal;

    skinned_normal =  skin_normal(normal, joints[indices.x], weights.x);
    skinned_normal += skin_normal(normal, joints[indices.y], weights.y);
    skinned_normal += skin_normal(normal, joints[indices.z], weights.z);
    skinned_normal += skin_normal(normal, joints[indices.w], weights.w);

    skinned_normal /= (weights.x + weights.y + weights.z + weights.w);

    return skinned_normal;
}



#endif
