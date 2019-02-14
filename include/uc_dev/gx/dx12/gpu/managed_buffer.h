#pragma once

#include <memory>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource_create_context;
            class gpu_buffer;

            namespace details
            {
                struct gpu_buffer_deleter
                {
                    gpu_resource_create_context* m_rc = nullptr;

                    gpu_buffer_deleter() = default;
                    gpu_buffer_deleter(gpu_resource_create_context* rc) : m_rc(rc) {}
                    void operator () (gpu_buffer* d);
                };
            }

            using managed_gpu_buffer = std::unique_ptr< gpu_buffer, details::gpu_buffer_deleter >;

            template <typename ...args>
            inline managed_gpu_buffer create_buffer(gpu_resource_create_context* rc, args&&... a)
            {
                return managed_gpu_buffer(rc->create_buffer(std::forward<args>(a)...), details::gpu_buffer_deleter(rc));
            }
        }
    }

}
