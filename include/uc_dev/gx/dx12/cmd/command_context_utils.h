#pragma once


#include "command_context.h"
#include "command_context_allocator.h"
#include <pix3.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace details
            {
                struct gpu_command_context_deleter
                {
                    gpu_command_context_allocator* m_rc = nullptr;

                    gpu_command_context_deleter() = default;
                    gpu_command_context_deleter(gpu_command_context_allocator* rc) : m_rc(rc) {}


                    void operator () (gpu_base_command_context* d)
                    {
                        m_rc->free_base_command_context(d);
                    }
                };
            }

            using managed_gpu_command_context = std::unique_ptr< gpu_command_context, details::gpu_command_context_deleter >;

            template <typename ...args>
            inline managed_gpu_command_context create_command_context(gpu_command_context_allocator* rc, args&&... a)
            {
                std::unique_ptr<gpu_base_command_context> base(rc->create_base_command_context(std::forward<args>(a)...));
                return managed_gpu_command_context(static_cast< gpu_command_context* > ( base.release() ), details::gpu_command_context_deleter(rc));
            }

        }
    }
}