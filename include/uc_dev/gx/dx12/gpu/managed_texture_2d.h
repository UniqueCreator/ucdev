#pragma once

#include <memory>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource_create_context;
            class gpu_texture_2d;

            namespace details
            {
                struct gpu_texture_2d_deleter
                {
                    gpu_resource_create_context* m_rc = nullptr;
                    gpu_texture_2d_deleter() noexcept = default;
                    gpu_texture_2d_deleter(gpu_resource_create_context* rc) noexcept : m_rc(rc) {}
                    void operator () (gpu_texture_2d* d);
                };
            }

            using managed_gpu_texture_2d = std::unique_ptr< gpu_texture_2d, details::gpu_texture_2d_deleter >;

            template <typename ...args>
            inline managed_gpu_texture_2d create_texture_2d(gpu_resource_create_context* rc, args&&... a)
            {
                return managed_gpu_texture_2d(rc->create_texture_2d(std::forward<args>(a)...), details::gpu_texture_2d_deleter(rc));
            }
        }
    }

}
