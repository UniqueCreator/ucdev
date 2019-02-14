#include "pch.h"

#include <uc_dev/gx/lip/structs.h>

namespace uc
{
        namespace lip
        {
            LIP_BEGIN_DEFINE_RTTI(float2)
                LIP_RTTI_MEMBER(float2, m_x)
                LIP_RTTI_MEMBER(float2, m_y)
            LIP_END_DEFINE_RTTI(float2)

            LIP_BEGIN_DEFINE_RTTI(float3)
                LIP_RTTI_MEMBER(float3, m_x)
                LIP_RTTI_MEMBER(float3, m_y)
                LIP_RTTI_MEMBER(float3, m_z)
            LIP_END_DEFINE_RTTI(float3)

            LIP_BEGIN_DEFINE_RTTI(float4)
                LIP_RTTI_MEMBER(float4, m_x)
                LIP_RTTI_MEMBER(float4, m_y)
                LIP_RTTI_MEMBER(float4, m_z)
                LIP_RTTI_MEMBER(float4, m_w)
            LIP_END_DEFINE_RTTI(float4)

            LIP_BEGIN_DEFINE_RTTI(float4a)
                LIP_RTTI_MEMBER(float4a, m_x)
                LIP_RTTI_MEMBER(float4a, m_y)
                LIP_RTTI_MEMBER(float4a, m_z)
                LIP_RTTI_MEMBER(float4a, m_w)
            LIP_END_DEFINE_RTTI(float4a)

            LIP_BEGIN_DEFINE_RTTI(ubyte4)
                LIP_RTTI_MEMBER(ubyte4, m_x)
                LIP_RTTI_MEMBER(ubyte4, m_y)
                LIP_RTTI_MEMBER(ubyte4, m_z)
                LIP_RTTI_MEMBER(ubyte4, m_w)
            LIP_END_DEFINE_RTTI(ubyte4)

            LIP_BEGIN_DEFINE_RTTI(uint4)
                LIP_RTTI_MEMBER(uint4, m_x)
                LIP_RTTI_MEMBER(uint4, m_y)
                LIP_RTTI_MEMBER(uint4, m_z)
                LIP_RTTI_MEMBER(uint4, m_w)
            LIP_END_DEFINE_RTTI(uint4)

            LIP_BEGIN_DEFINE_RTTI(float8a)

                LIP_RTTI_MEMBER(float8a, m_x)
                LIP_RTTI_MEMBER(float8a, m_y)
                LIP_RTTI_MEMBER(float8a, m_z)
                LIP_RTTI_MEMBER(float8a, m_w)

                LIP_RTTI_MEMBER(float8a, m_a)
                LIP_RTTI_MEMBER(float8a, m_b)
                LIP_RTTI_MEMBER(float8a, m_c)
                LIP_RTTI_MEMBER(float8a, m_d)

            LIP_END_DEFINE_RTTI(float8a)


            LIP_BEGIN_DEFINE_RTTI(float8)

                LIP_RTTI_MEMBER(float8, m_x)
                LIP_RTTI_MEMBER(float8, m_y)
                LIP_RTTI_MEMBER(float8, m_z)
                LIP_RTTI_MEMBER(float8, m_w)

                LIP_RTTI_MEMBER(float8, m_a)
                LIP_RTTI_MEMBER(float8, m_b)
                LIP_RTTI_MEMBER(float8, m_c)
                LIP_RTTI_MEMBER(float8, m_d)

            LIP_END_DEFINE_RTTI(float8)
        }
}
