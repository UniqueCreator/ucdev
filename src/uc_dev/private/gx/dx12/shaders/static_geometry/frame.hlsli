#ifndef __per_frame_hlsli__
#define __per_frame_hlsli__
cbuffer per_frame   : register(b0)
{
    euclidean_transform_3d   m_view;
    projective_transform_3d  m_perspective;
};
#endif
