#include "pch.h"
#include "uc_uwp_renderer_impl.h"

#include <array>

#include <uc_dev/gxu/camera_command.h>

#include "uc_uwp_renderer_impl_window.h"

#include <uc_dev/math/math.h>
#include <uc_dev/gx/img/img.h>

#include <uc_dev/gx/lip/file.h>
#include <uc_dev/gx/lip_utils.h>
#include <uc_dev/gx/img_utils.h>

#include <ppl.h>
#include "uc_uwp_ui_helper.h"

#include <windows.ui.xaml.media.dxinterop.h>

namespace uc
{
    namespace uwp
    {
        renderer_impl::renderer_impl( bool* window_close, const winrt::Windows::UI::Core::CoreWindow& window, const winrt::Windows::Graphics::Display::DisplayInformation& display_information, ISwapChainPanelNative* swap_chain_panel) : m_main_window(window_close)
        {
            set_window(window);
            set_display_info(display_information);
            if (swap_chain_panel)
            {
                m_resources.add_swapchain(swap_chain::swap_chain_type::background_composition);
                refresh_display_layout();
                flush_prerender_queue();

                auto resource = m_resources.swap_chain(0);
                swap_chain_panel->SetSwapChain( resource->swap_chain() );
            }
            else
            {
                m_resources.add_swapchain(swap_chain::swap_chain_type::background_core_window);
                m_resources.add_swapchain(swap_chain::swap_chain_type::foreground_core_window);
            }

            gx::geo::geometry_allocator_options o = {};
            o.m_index_count = 1000000;
            o.m_skinned_mesh_vertex_count = 1000000;
            o.m_static_mesh_vertex_count = 1000000;
            o.m_normal_mesh_vertex_count = 1000000;

            m_geometry_allocator            = std::make_unique<gx::geo::geometry_allocator>(m_resources.resource_create_context(), o);
        }

        void renderer_impl::initialize_resources()
        {
            using namespace gx::dx12;

            concurrency::task_group g;

            g.run([this]()
            {
                gxu::initialize_context c0;
                c0.m_geometry = m_geometry_allocator.get();
                c0.m_resources = &m_resources;		
                m_render_world_manager = std::make_unique<gxu::render_world_manager>(&c0);
                m_render_world_manager->show_world(uc::uwp::gxu::world_id::scene2);

                
                {
                    overlay::initialize_context c1;
                    c1.m_resources = &m_resources;
                    m_overlay_page_manager = std::make_unique<overlay::page_manager>(&c1, m_render_world_manager.get());
                }
                
                
            });

            g.wait();

            //flush all uploaded resources previous frame
            m_resources.direct_queue(device_resources::swap_chains::background )->insert_wait_on(m_resources.upload_queue()->flush());
            m_resources.direct_queue(device_resources::swap_chains::background )->insert_wait_on(m_resources.compute_queue()->signal_fence());
        }

        static inline uint64_t next_frame(uint64_t frame)
        {
            uint64_t r = frame + 1;
            return r % 3;
        }

        void renderer_impl::set_display_info(const winrt::Windows::Graphics::Display::DisplayInformation& display_information)
        {
            m_display_information = display_information;
        }

        void renderer_impl::set_window( const winrt::Windows::UI::Core::CoreWindow& window)
        {
            m_window = window;
        }

        void renderer_impl::set_swapchainpanel(const winrt::Windows::UI::Xaml::Controls::SwapChainPanel&)
        {

        }

        void renderer_impl::refresh_display_layout()
        {
            resize_window_command c;
            c.m_window_environment = build_environment(m_window, m_display_information);
            m_prerender_queue.push(std::move(c));
            //ui stuff
            m_mouse.set_window(m_window, c.m_window_environment.m_effective_dpi);
            m_keyboard.set_window(m_window);
        }

        renderer_impl::~renderer_impl()
        {
            ui_run_sync(m_window, [this]
            {
                m_mouse.release();
                m_keyboard.release();
            });

            m_resources.wait_for_gpu();
        }

        void renderer_impl::process_user_input()
        {
            m_pad_state         = m_pad.update(m_pad_state);
            m_mouse_state       = m_mouse.update(m_mouse_state);
            m_keyboard_state    = m_keyboard.update(m_keyboard_state);

            io::keyboard_state s = m_keyboard_state;

            if ( io::button_was_pressed(s, io::keyboard_state::button_192))
            {
                overlay::pageid pages[] =
                {
                    overlay::pageid::hud,
                    overlay::pageid::debug_page,
                    overlay::pageid::options_page,
                    overlay::pageid::none
                };

                static uint32_t index = 0;
                this->m_overlay_page_manager->show_page( pages[ index] );
                index++;
                index %= 4;
            }
            

            if ( m_overlay_page_manager->get_active_page_id() != overlay::pageid::options_page)
            {
                auto camera = m_render_world_manager->camera();

                if (camera)
                {
                    const float movement_camera_speed = static_cast<float>(5000.0f * m_frame_timer.seconds());
                    m_frame_timer.reset();

                    const float rotation_camera_speed = 0.002f * 3.1415f;

                    auto pad = m_pad_state;

                    if (pad.m_state.m_thumb_left_x != 0.0f || pad.m_state.m_thumb_left_y != 0.0f)
                    {
                        auto move_x = std::copysign(movement_camera_speed, pad.m_state.m_thumb_left_x);
                        auto move_y = std::copysign(movement_camera_speed, pad.m_state.m_thumb_left_y);

                        auto a = fabsf(pad.m_state.m_thumb_left_x);
                        auto b = fabsf(pad.m_state.m_thumb_left_y);

                        uc::gxu::camera_command command;

                        if (a > b)
                        {
                            command = uc::gxu::create_move_camera_xy_command(move_x, 0.0f);
                        }
                        else
                        {
                            command = uc::gxu::create_move_camera_xy_command(0.0f, move_y);
                        }

                        uc::gxu::pinhole_camera_command_dispatcher procesor(camera);
                        procesor.process(&command);
                    }

                    if (pad.m_state.m_thumb_right_x != 0.0f || pad.m_state.m_thumb_right_y != 0.0f)
                    {
                        auto move_x = movement_camera_speed;
                        auto move_y = movement_camera_speed;

                        auto v = math::set(move_x, move_y, 0.0f, 0.0f);

                        float magnitude = math::get_x(math::length2(v));

                        if (pad.m_state.m_thumb_right_y < 0.0f)
                        {
                            magnitude = -magnitude;
                        }

                        uc::gxu::camera_command command = uc::gxu::create_move_camera_z_command(magnitude);
                        uc::gxu::pinhole_camera_command_dispatcher procesor(camera);
                        procesor.process(&command);
                    }

                    if (io::button_was_pressed(pad, io::pad_state::button_a))
                    {
                        if (m_scale_render > 0.20f)
                        {
                            m_scale_render -= 0.25f;
                        }
                        else
                        {
                            m_scale_render = 1.0f;
                        }
                    }

                    //clamp the camera
                    {
                        auto camera_position = camera->position();
                        auto camera_min = math::set(-100.0f, -100.0f, -125.0f, 1.0f);
                        auto camera_max = math::set(+100.0f, +100.0f, +125.0f, 1.0f);
                        camera_position = math::clamp(camera_position, camera_min, camera_max);
                        camera->set_view_position(camera_position);
                    }
                }
            }
        }

        void renderer_impl::update()
        {
            m_frame_time = m_frame_timer.seconds();
            m_frame_timer.reset();

            process_user_input();

            concurrency::task_group g;

            g.run([this]
            {
                gxu::update_context ctx;
                auto&& back_buffer = m_resources.back_buffer(device_resources::swap_chains::background);
                ctx.m_back_buffer_size.m_width = static_cast<uint16_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                ctx.m_back_buffer_size.m_height = static_cast<uint16_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                ctx.m_front_buffer_size.m_width = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->width());
                ctx.m_front_buffer_size.m_height = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->height());

                ctx.m_resources = &m_resources;
                ctx.m_pad_state = m_pad_state;
                ctx.m_mouse_state = m_mouse_state;
                ctx.m_keyboard_state = m_keyboard_state;
                ctx.m_frame_time = m_frame_time;

                ctx.m_window_close = m_main_window;
                m_render_world_manager->update(&ctx);
            });

            if ( m_overlay_page_manager->get_active_page_id() != overlay::pageid::none )
            {
                overlay::update_context ctx;
                auto&& back_buffer = m_resources.back_buffer(device_resources::swap_chains::background);
                ctx.m_back_buffer_size.m_width = static_cast<uint16_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                ctx.m_back_buffer_size.m_height = static_cast<uint16_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                ctx.m_front_buffer_size.m_width = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->width());
                ctx.m_front_buffer_size.m_height = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->height());
                ctx.m_resources         = &m_resources;
                ctx.m_pad_state         = m_pad_state;
                ctx.m_mouse_state       = m_mouse_state;
                ctx.m_keyboard_state    = m_keyboard_state;
                ctx.m_frame_time        = m_frame_time;

                ctx.m_window_close = m_main_window;

                m_overlay_page_manager->update(&ctx);
            }

            g.wait();
        }

        void renderer_impl::flush_prerender_queue()
        {
            resize_window_command c;
            while (m_prerender_queue.try_pop(c))
            {
                resize_buffers(&c.m_window_environment);
            }
        }

        void renderer_impl::resize_buffers( const window_environment* environment )
        {
            m_resources.wait_for_gpu();
            m_resources.set_window(environment);
            m_window = environment->m_window;

            //Recreate view depth buffer and the msaa shadows depth buffer
            m_render_world_manager->resize_buffers(&m_resources);
        }

        void renderer_impl::pre_render()
        {
            //skeleton of a render phases, which will get complicated over time
            flush_prerender_queue();
        }

        void renderer_impl::render()
        {
            using namespace gx::dx12;

            concurrency::task_group g;

            //flush all uploaded resources previous frame
            //make sure the gpu waits for the newly uploaded resources if any
            //flush the previous
            m_resources.direct_queue(device_resources::swap_chains::background)->pix_begin_event(L"Frame");
            m_resources.direct_queue(device_resources::swap_chains::background)->insert_wait_on(m_resources.upload_queue()->flush());
            m_resources.direct_queue(device_resources::swap_chains::background)->insert_wait_on(m_resources.compute_queue()->signal_fence());

            m_resources.direct_queue(device_resources::swap_chains::overlay)->insert_wait_on(m_resources.upload_queue()->flush());
            m_resources.direct_queue(device_resources::swap_chains::background)->insert_wait_on(m_resources.compute_queue()->signal_fence());


            std::unique_ptr<submitable> pending_depth;
            std::unique_ptr<submitable> pending_main;
            std::unique_ptr<submitable> pending_overlay;
            std::unique_ptr<submitable> pending_shadows;


            //std::unique_ptr<gx::dx12::gpu_frame_depth_buffer> frame_depth_buffer;

            m_frame_index += 1;
            m_frame_index %= 3;
            
            //create depth buffer for this frame
            {
                uint32_t width = 0;
                uint32_t height = 0;
                auto&& back_buffer = m_resources.back_buffer(device_resources::swap_chains::background);

                width = static_cast<uint32_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                height = static_cast<uint32_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                m_frame_depth_buffer[m_frame_index] = std::unique_ptr<gx::dx12::gpu_frame_depth_buffer>(m_resources.resource_create_context()->create_frame_depth_buffer(width, height, DXGI_FORMAT_D32_FLOAT));
            }


            //create shadow buffers for this frame
            {

                using namespace gx::dx12;

                auto shadow_d = m_render_world_manager->shadow_map_descriptor();

                auto rc = m_resources.resource_create_context();

                if ((shadow_d.m_shadow_flags & gxu::shadow_buffers_descriptor::shadow_buffer_used) !=0)
                {
                    m_frame_shadow_buffer[m_frame_index] = std::unique_ptr<gpu_frame_msaa_depth_buffer>(rc->create_frame_msaa_depth_buffer(shadow_d.m_shadow_buffer.m_width, shadow_d.m_shadow_buffer.m_height, shadow_d.m_shadow_buffer.m_format, shadow_d.m_shadow_buffer.m_initial_value));
                }
                else
                {
                    m_frame_shadow_buffer[m_frame_index].reset();
                }

                if ((shadow_d.m_shadow_flags & gxu::shadow_buffers_descriptor::shadow_map_used) != 0)
                {
                    auto& m = shadow_d.m_shadow_map;
                    m_frame_shadow_map[m_frame_index] = std::unique_ptr < gpu_frame_color_buffer>(rc->create_frame_color_buffer(m.m_width, m.m_height, m.m_format, m.m_initial_state));
                }
                else
                {
                    m_frame_shadow_map[m_frame_index].reset();
                }
            }

            g.run([this,&pending_main]
            {
                uint32_t width      = 0;
                uint32_t height     = 0;
                auto&& back_buffer  = m_resources.back_buffer(device_resources::swap_chains::background);

                width               = static_cast<uint32_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                height              = static_cast<uint32_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                width               = std::max(width, 8U);
                height              = std::max(height, 8U);

                gxu::render_context ctx;
                ctx.m_view_depth_buffer                 = m_frame_depth_buffer[m_frame_index].get();
                ctx.m_shadow_depth_buffer               = m_frame_shadow_buffer[m_frame_index].get();
                ctx.m_shadow_map                        = m_frame_shadow_map[m_frame_index].get();
                ctx.m_shadow_buffer_size.m_width        = static_cast<uint16_t>(ctx.m_shadow_depth_buffer ? ctx.m_shadow_depth_buffer->width() : 0);
                ctx.m_shadow_buffer_size.m_height       = static_cast<uint16_t>(ctx.m_shadow_depth_buffer ? ctx.m_shadow_depth_buffer->height() : 0);
                ctx.m_geometry                          = m_geometry_allocator.get();
                ctx.m_resources                         = &m_resources;
                ctx.m_frame_time                        = m_frame_time;
                ctx.m_scale_render                      = m_scale_render;
                ctx.m_back_buffer_size.m_width          = static_cast<uint16_t>(width);
                ctx.m_back_buffer_size.m_height         = static_cast<uint16_t>(height);

                ctx.m_front_buffer_size.m_width         = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->width());
                ctx.m_front_buffer_size.m_height        = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->height());
                ctx.m_back_buffer_scaled_size.m_width   = static_cast<uint16_t>(width);
                ctx.m_back_buffer_scaled_size.m_height  = static_cast<uint16_t>(height);
                pending_main                            = std::move(m_render_world_manager->render(&ctx));
            });

            g.run([this, &pending_depth]
            {
                uint32_t width = 0;
                uint32_t height = 0;
                auto&& back_buffer = m_resources.back_buffer(device_resources::swap_chains::background);

                width = static_cast<uint32_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                height = static_cast<uint32_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                width = std::max(width, 8U);
                height = std::max(height, 8U);

                gxu::render_context ctx;
                ctx.m_view_depth_buffer                 = m_frame_depth_buffer[m_frame_index].get();
                ctx.m_shadow_depth_buffer               = m_frame_shadow_buffer[m_frame_index].get();
                ctx.m_shadow_map                        = m_frame_shadow_map[m_frame_index].get();
                ctx.m_shadow_buffer_size.m_width        = static_cast<uint16_t>(ctx.m_shadow_depth_buffer ? ctx.m_shadow_depth_buffer->width() : 0);
                ctx.m_shadow_buffer_size.m_height       = static_cast<uint16_t>(ctx.m_shadow_depth_buffer ? ctx.m_shadow_depth_buffer->height() : 0);
                ctx.m_resources                         = &m_resources;
                ctx.m_geometry                          = m_geometry_allocator.get();
                ctx.m_frame_time                        = m_frame_time;
                ctx.m_scale_render                      = m_scale_render;
                ctx.m_back_buffer_size.m_width          = static_cast<uint16_t>(width);
                ctx.m_back_buffer_size.m_height         = static_cast<uint16_t>(height);

                ctx.m_front_buffer_size.m_width         = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->width());
                ctx.m_front_buffer_size.m_height        =  static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->height());
                ctx.m_back_buffer_scaled_size.m_width   = static_cast<uint16_t>(width);
                ctx.m_back_buffer_scaled_size.m_height  = static_cast<uint16_t>(height);
                pending_depth = std::move(m_render_world_manager->render_depth(&ctx));
            });

            g.run([this, &pending_shadows]
            {
                uint32_t width = 0;
                uint32_t height = 0;
                auto&& back_buffer = m_resources.back_buffer(device_resources::swap_chains::background);

                width = static_cast<uint32_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                height = static_cast<uint32_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                width = std::max(width, 8U);
                height = std::max(height, 8U);

                gxu::shadow_render_context ctx;
                ctx.m_view_depth_buffer                 = m_frame_depth_buffer[m_frame_index].get();
                ctx.m_shadow_depth_buffer               = m_frame_shadow_buffer[m_frame_index].get();
                ctx.m_shadow_map                        = m_frame_shadow_map[m_frame_index].get();
                ctx.m_shadow_buffer_size.m_width        = static_cast<uint16_t>(ctx.m_shadow_depth_buffer ? ctx.m_shadow_depth_buffer->width() : 0 );
                ctx.m_shadow_buffer_size.m_height       = static_cast<uint16_t>(ctx.m_shadow_depth_buffer ? ctx.m_shadow_depth_buffer->height(): 0 );
                ctx.m_geometry                          = m_geometry_allocator.get();
                ctx.m_resources                         = &m_resources;
                ctx.m_frame_time                        = m_frame_time;
                ctx.m_scale_render                      = m_scale_render;
                ctx.m_back_buffer_size.m_width          = static_cast<uint16_t>(width);
                ctx.m_back_buffer_size.m_height         = static_cast<uint16_t>(height);

                ctx.m_front_buffer_size.m_width         = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->width());
                ctx.m_front_buffer_size.m_height        = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->height());
                ctx.m_back_buffer_scaled_size.m_width   = static_cast<uint16_t>(width);
                ctx.m_back_buffer_scaled_size.m_height  = static_cast<uint16_t>(height);
                pending_shadows                         = std::move(m_render_world_manager->render_shadows(&ctx));
            });

       
            //generate overalay
            g.run([this, &pending_overlay]()
            {
                uint32_t width = 0;
                uint32_t height = 0;

                {
                    auto&& back_buffer = m_resources.back_buffer(device_resources::swap_chains::background);
                    width = static_cast<uint32_t>(static_cast<float>(back_buffer->width()) * m_scale_render);
                    height = static_cast<uint32_t>(static_cast<float>(back_buffer->height()) * m_scale_render);

                    width = std::max(width, 8U);
                    height = std::max(height, 8U);
                }

                //second block, can be in parallel, overlay
                {
                    //now start new ones
                    if (m_overlay_page_manager->get_active_page_id() != overlay::pageid::none)
                    {
                        overlay::render_context ctx;
                        ctx.m_resources = &m_resources;
                        ctx.m_back_buffer_size.m_width      = static_cast<uint16_t>(width);
                        ctx.m_back_buffer_size.m_height     = static_cast<uint16_t>(height);
                        ctx.m_front_buffer_size.m_width     = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->width());
                        ctx.m_front_buffer_size.m_height    = static_cast<uint16_t>(m_resources.back_buffer(device_resources::swap_chains::overlay)->height());

                        pending_overlay = std::move(m_overlay_page_manager->render(&ctx));
                    }
                    else
                    {
                        auto resources = &m_resources;

                        auto graphics = gx::dx12::create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::overlay));
                        auto&& back_buffer = resources->back_buffer(device_resources::swap_chains::overlay);
                        graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
                        graphics->set_render_target(back_buffer);
                        graphics->clear(back_buffer);

                        graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

                        //now we submit the graphics work also
                        //graphics->submit(gx::dx12::gpu_command_context::flush_operation::do_not_wait_to_execute);

                        pending_overlay = std::make_unique<graphics_submitable>(std::move(graphics));
                    }
                }
            });
            g.wait();

            //if we did upload through the pci bus, insert waits
            m_resources.direct_queue(device_resources::swap_chains::background)->insert_wait_on(m_resources.upload_queue()->flush());
            m_resources.direct_queue(device_resources::swap_chains::background)->insert_wait_on(m_resources.compute_queue()->signal_fence());

            m_resources.direct_queue(device_resources::swap_chains::overlay)->insert_wait_on(m_resources.upload_queue()->flush());
            m_resources.direct_queue(device_resources::swap_chains::background)->insert_wait_on(m_resources.compute_queue()->signal_fence());

            if (pending_shadows)
            {
                pending_shadows->submit();
            }

            if (pending_depth)
            {
                pending_depth->submit();
            }

            if (pending_main)
            {
                pending_main->submit();
            }

            if (pending_overlay)
            {
                pending_overlay->submit();
            }

            m_resources.direct_queue(device_resources::swap_chains::background)->pix_end_event();
        }

        void renderer_impl::present()
        {
            m_resources.present();
            m_resources.move_to_next_frame();
            m_resources.sync();
            m_geometry_allocator->sync();
        }

        void renderer_impl::resize()
        {

        }

    }
}