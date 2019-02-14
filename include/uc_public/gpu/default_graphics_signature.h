#pragma once

#include <cstdint>

namespace UniqueCreator
{
    namespace Gpu
    {
        namespace DefaultRootSignature
        {
            enum Slots : uint32_t
            {
                ConstantBuffer0     = 0,
                ConstantBuffer1     = 3,
                SharedResourceView0 = 1,
                SharedResourceView1 = 5,

                UnorderedAccessView0 = 2,
                UnorderedAccessView1 = 4,
                UnorderedAccessView2 = 8
            };
        };
    }
}





