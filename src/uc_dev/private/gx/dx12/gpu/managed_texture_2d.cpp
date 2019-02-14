#include "pch.h"

#include <uc_dev/gx/dx12/gpu/managed_texture_2d.h>
#include <uc_dev/gx/dx12/gpu/managed_texture_2d_array.h>
#include <uc_dev/gx/dx12/gpu/resource_create_context.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace details
            {
                void gpu_texture_2d_deleter::operator() (gpu_texture_2d* d)
                {
                    m_rc->free_texture_2d(d);
                }

                void gpu_texture_2d_array_deleter::operator() (gpu_texture_2d_array* d)
                {
                    m_rc->free_texture_2d_array(d);
                }
            }
        }
    }

}

