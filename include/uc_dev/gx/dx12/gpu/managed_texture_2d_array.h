#pragma once

#include <memory>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource_create_context;
            class gpu_texture_2d_array;

            namespace details
            {
                struct gpu_texture_2d_array_deleter
                {
                    gpu_resource_create_context* m_rc = nullptr;

                    gpu_texture_2d_array_deleter() = default;
                    gpu_texture_2d_array_deleter(gpu_resource_create_context* rc) : m_rc(rc) {}
                    void operator () (gpu_texture_2d_array* d);
                };
            }

            using managed_gpu_texture_2d_array = std::unique_ptr< gpu_texture_2d_array, details::gpu_texture_2d_array_deleter >;

            template <typename ...args>
            inline managed_gpu_texture_2d_array create_texture_2d_array(gpu_resource_create_context* rc, args&&... a)
            {
                return managed_gpu_texture_2d_array(rc->create_texture_2d_array(std::forward<args>(a)...), details::gpu_texture_2d_array_deleter(rc));
            }
        }
    }

}
