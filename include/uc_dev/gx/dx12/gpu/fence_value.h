#pragma once

#include <cstdint>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_fence_value
            {
                public:
                gpu_fence_value( uint64_t value = 0 ) : m_value(value)
                {

                }

                ~gpu_fence_value() = default;
                gpu_fence_value(const gpu_fence_value&) = default;
                gpu_fence_value(gpu_fence_value&&) = default;

                gpu_fence_value& operator=(const gpu_fence_value&) = default;
                gpu_fence_value& operator=(gpu_fence_value&&) = default;




                operator uint64_t () const
                {
                    return m_value;
                }

                private:

                uint64_t m_value = 0;
            };


            struct gpu_fence
            {
                ID3D12Fence*    m_fence = nullptr;
                gpu_fence_value m_value = 0;


                bool is_fence_completed() const
                {
                    return m_value <= m_fence->GetCompletedValue();
                }
            };

        }
    }
}