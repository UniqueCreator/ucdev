#pragma once

#include <memory>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource_create_context;
            class gpu_read_write_texture_2d;

            namespace details
            {
                struct gpu_read_write_texture_2d_deleter
                {
                    gpu_resource_create_context* m_rc = nullptr;

                    gpu_read_write_texture_2d_deleter() = default;
                    gpu_read_write_texture_2d_deleter(gpu_resource_create_context* rc) : m_rc(rc) {}
                    void operator () (gpu_read_write_texture_2d* d);
                };
            }

            using managed_gpu_read_write_texture_2d = std::unique_ptr< gpu_read_write_texture_2d, details::gpu_texture_2d_deleter >;

            template <typename ...args>
            inline managed_gpu_read_write_texture_2d create_read_write_texture_2d(gpu_resource_create_context* rc, args&&... a)
            {
                return managed_gpu_read_write_texture_2d(rc->create_read_write_texture_2d(std::forward<args>(a)...), details::gpu_read_write_texture_2d_deleter(rc));
            }
        }
    }

}
