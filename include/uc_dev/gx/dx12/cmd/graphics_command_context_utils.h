#pragma once


#include "graphics_command_context.h"
#include "command_context_utils.h"


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            using managed_graphics_command_context = std::unique_ptr< gpu_graphics_command_context, details::gpu_command_context_deleter >;

            template <typename ...args>
            inline managed_graphics_command_context create_graphics_command_context(gpu_command_context_allocator* rc, args&&... a)
            {
                std::unique_ptr<gpu_base_command_context> base(rc->create_base_command_context(std::forward<args>(a)...));
                return managed_graphics_command_context(static_cast< gpu_graphics_command_context* > ( base.release() ), details::gpu_command_context_deleter(rc));
            }

            inline void graphics_reset_vertex_buffer(managed_graphics_command_context& ctx, uint32_t slot)
            {
                ctx->set_vertex_buffer(slot, {});
            }

            inline void graphics_reset_index_buffer(managed_graphics_command_context& ctx)
            {
                ctx->set_index_buffer({});
            }
        }
    }
}