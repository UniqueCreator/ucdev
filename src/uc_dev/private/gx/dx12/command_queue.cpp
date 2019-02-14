#include "pch.h"

#include <uc_dev/gx/dx12/cmd/command_queue.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            void gpu_command_queue::signal_fence(ID3D12Fence* f, uint64_t value) 
            {
                throw_if_failed(m_queue->Signal(f, value));
            }

            void gpu_command_queue::signal_fence(uint64_t value) 
            {
                throw_if_failed(m_queue->Signal(m_fence.Get(), value));
            }
        }

    }
}

