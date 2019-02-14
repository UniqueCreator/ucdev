#include "pch.h"

#include <uc_dev/gx/dx12/gpu/managed_buffer.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace details
            {
                    void gpu_buffer_deleter::operator () (gpu_buffer* d)
                    {
                        m_rc->free_buffer(d);
                    }
            }
        }
    }

}

