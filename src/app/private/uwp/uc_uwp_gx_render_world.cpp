#include "pch.h"
#include "uc_uwp_gx_render_world.h"

#include <uc_dev/gx/dx12/gpu/pixel_buffer.h>
#include <uc_dev/gx/dx12/cmd/profiler.h>

#include "uc_uwp_device_resources.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            D3D12_VIEWPORT viewport(const gx::dx12::gpu_pixel_buffer* b)
            {
                D3D12_VIEWPORT v = {};
                auto desc = b->desc();
                v.Width = static_cast<float>(desc.Width);
                v.Height = static_cast<float>(desc.Height);
                v.MinDepth = 0.0f;
                v.MaxDepth = 1.0f;
                return v;
            }

            D3D12_RECT scissor(const gx::dx12::gpu_pixel_buffer* b)
            {
                D3D12_RECT r = {};
                auto desc = b->desc();
                r.right = static_cast<LONG>(desc.Width);
                r.bottom = static_cast<LONG>(desc.Height);
                return r;
            }


            render_world::render_world(initialize_context*)
            {

            }

            render_world::~render_world()
            {

            }

            void render_world::update(update_context* ctx)
            {
                do_update(ctx);
            }

            std::unique_ptr< submitable > render_world::render(render_context* ctx)
            {
                return do_render(ctx);
            }

            std::unique_ptr< submitable > render_world::render_depth(render_context* ctx)
            {
                return do_render_depth(ctx);
            }

            std::unique_ptr< submitable >render_world::render_shadows(shadow_render_context* ctx)
            {
                return do_render_shadows(ctx);
            }

            void render_world::begin_render( render_context* ctx, gx::dx12::gpu_graphics_command_context* graphics )
            {
                auto resources = ctx->m_resources;

                //now start new ones
                auto&& back_buffer = resources->back_buffer(device_resources::swap_chains::background);

                auto width = ctx->m_back_buffer_scaled_size.m_width;
                auto height = ctx->m_back_buffer_scaled_size.m_height;

                //may be not here
                resources->swap_chain(device_resources::swap_chains::background)->set_source_size(width, height);

                graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
                //graphics->transition_resource(ctx->m_view_depth_buffer, D3D12_RESOURCE_STATE_DEPTH_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

                graphics->set_render_target(back_buffer, ctx->m_view_depth_buffer);
                graphics->clear(back_buffer);
            }

            void render_world::end_render(render_context* ctx, gx::dx12::gpu_graphics_command_context* graphics)
            {
                auto resources      = ctx->m_resources;
                auto back_buffer    = resources->back_buffer(device_resources::swap_chains::background);

                //Per pass
                //graphics->transition_resource(ctx->m_view_depth_buffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_DEPTH_READ);
                graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            }

            void render_world::begin_render_depth(render_context* ctx, gx::dx12::gpu_graphics_command_context* graphics)
            {
                if (ctx->m_view_depth_buffer)
                {
                    graphics->set_render_target(ctx->m_view_depth_buffer);
                    graphics->clear_depth(ctx->m_view_depth_buffer, 1.0f);
                }
            }

            void render_world::end_render_depth( render_context* ctx, gx::dx12::gpu_graphics_command_context* graphics )
            {
                ctx;
                graphics;
            }

            void render_world::begin_render_shadows(shadow_render_context* ctx, gx::dx12::gpu_graphics_compute_command_context* graphics)
            {
                ctx;
                graphics;
            }

            void render_world::end_render_shadows(shadow_render_context* ctx, gx::dx12::gpu_graphics_compute_command_context* graphics)
            {
                ctx;
                graphics;
            }

            void render_world::set_view_port( const render_context* ctx, gx::dx12::gpu_graphics_command_context* graphics)
            {
                auto width = ctx->m_back_buffer_scaled_size.m_width;
                auto height = ctx->m_back_buffer_scaled_size.m_height;

                //Per pass  -> frequency 0
                graphics->set_view_port(viewport(width, height));
                graphics->set_scissor_rectangle(scissor(width, height));
            }

            void render_world::set_view_port(const shadow_render_context* ctx, gx::dx12::gpu_graphics_command_context* graphics)
            {
                auto width  = ctx->m_shadow_buffer_size.m_width;
                auto height = ctx->m_shadow_buffer_size.m_height;

                //Per pass  -> frequency 0
                graphics->set_view_port(viewport(width, height));
                graphics->set_scissor_rectangle(scissor(width, height));
            }

            void render_world::set_view_port(const render_context* ctx, gx::dx12::gpu_graphics_compute_command_context* graphics)
            {
                auto width = ctx->m_back_buffer_scaled_size.m_width;
                auto height = ctx->m_back_buffer_scaled_size.m_height;

                //Per pass  -> frequency 0
                graphics->set_view_port(viewport(width, height));
                graphics->set_scissor_rectangle(scissor(width, height));
            }

            void render_world::set_view_port(const shadow_render_context* ctx, gx::dx12::gpu_graphics_compute_command_context* graphics)
            {
                auto width = ctx->m_shadow_buffer_size.m_width;
                auto height = ctx->m_shadow_buffer_size.m_height;

                //Per pass  -> frequency 0
                graphics->set_view_port(viewport(width, height));
                graphics->set_scissor_rectangle(scissor(width, height));
            }


            std::unique_ptr< submitable >render_world::do_render_depth(render_context* ctx)
            {
                auto resources = ctx->m_resources;
                //now start new ones
                auto graphics = create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));
                begin_render_depth(ctx, graphics.get());
                end_render_depth(ctx, graphics.get());
                return std::make_unique<graphics_submitable>(std::move(graphics));
            }

            std::unique_ptr< submitable >render_world::do_render_shadows(shadow_render_context* ctx)
            {
                auto resources = ctx->m_resources;
                //now start new ones
                auto graphics = create_graphics_compute_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::background));

                {
                    //auto profile_event = uc::gx::dx12::make_profile_event(graphics.get(), L"do_render_shadows");
                    //begin_render_shadows(ctx, graphics.get());
                    //end_render_shadows(ctx, graphics.get());
                }
                return std::make_unique<graphics_compute_submitable>(std::move(graphics));
            }

            shadow_buffers_descriptor render_world::shadow_map_descriptor()
            {
                return on_shadow_map_descriptor();
            }

            shadow_buffers_descriptor render_world::on_shadow_map_descriptor()
            {
                return {};
            }

            void render_world::resize_buffers(device_resources* resources)
            {
                on_resize_buffers(resources);
            }

            void render_world::on_resize_buffers( device_resources* resources)
            {
                resources;
            }
        }
    }
}