#include "pch.h"
#include "uc_uwp_renderer_options_overlay_page.h"

#include <array>
#include <cstdint>
#include <string>
#include <ppl.h>
#include <Windows.h>

#include <imgui.h>

#include <autogen/shaders/imgui_graphics.h>

#include <uc_dev/math/math.h>
#include <uc_dev/sys/memcpy.h>
#include <uc_dev/util/utf8_conv.h>

#include <uc_dev/gx/dx12/gpu/texture_2d.h>

#include "uc_uwp_renderer_options_overlay_console.h"
#include "uc_uwp_renderer_options_overlay_log.h"

#include "uc_uwp_gx_render_world_manager.h"


namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            inline void throw_if_failed(HRESULT h)
            {
                gx::dx12::throw_if_failed(h);
            }

            inline D3D12_SUBRESOURCE_DATA sub_resource_data(uint32_t width, uint32_t height, const void* pixels)
            {

                //linear texture
                D3D12_SUBRESOURCE_DATA s = {};

                auto h = height;

                s.pData         = pixels;
                s.RowPitch      = width * sizeof(uint32_t);
                s.SlicePitch    = s.RowPitch * h;

                return s;
            }

            inline  math::float4x4 projection_matrix(ImGuiIO& io)
            {
                float L = 0.0f;
                float R = io.DisplaySize.x;
                float B = io.DisplaySize.y;
                float T = 0.0f;

                math::float4x4 m;
                
                /*
                float mvp[4][4] =
                {
                    { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
                    { 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
                    { 0.0f,         0.0f,           0.5f,       0.0f },
                    { (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
                };
                */

                m.r[0] = math::set( 2.0f / (R - L),     0.0f,               0.0f, 0.0f);
                m.r[1] = math::set( 0.0f,               2.0f / (T - B),     0.0f, 0.0f);
                m.r[2] = math::set( 0.0f,               0.0f,               0.5f, 0.0f);
                m.r[3] = math::set((R + L) / (L - R),   (T + B) / (B - T),  0.5f, 1.0f);

                return m;
            }

            inline D3D12_VIEWPORT viewport( ImGuiIO&io )
            {
                D3D12_VIEWPORT v = {};
                v.Width = static_cast<float>(io.DisplaySize.x);
                v.Height = static_cast<float>(io.DisplaySize.y);
                v.MinDepth = 0.0f;
                v.MaxDepth = 1.0f;
                return v;
            }

            inline D3D12_RECT scissor( ImGuiIO&io )
            {
                D3D12_RECT r = {};
                r.right = static_cast<LONG>(io.DisplaySize.x);
                r.bottom = static_cast<LONG>(io.DisplaySize.y);
                return r;
            }

            inline D3D12_RECT scissor( ImVec4 v)
            {
                D3D12_RECT r = { (LONG)v.x, (LONG)v.y, (LONG)v.z, (LONG)v.w };
                return r;
            }

            struct frame_constants
            {
                math::float4x4 m_view;
                math::float4x4 m_perspective;
            };

            struct draw_constants
            {
                math::float4x4 m_world;
            };

            options_page::options_page(initialize_context* ctx, gxu::render_world_manager* m ) : base(ctx), m_render_world_manager(m)
            {
                auto resources = ctx->m_resources;

                concurrency::task_group g;

                g.run([this, resources]
                {
                    uint8_t* pixels;
                    int32_t  width;
                    int32_t  height;

                    ImGuiIO& io = ImGui::GetIO();
                    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

                    m_font = gx::dx12::create_texture_2d(resources->resource_create_context(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
                    D3D12_SUBRESOURCE_DATA s = sub_resource_data(width, height, pixels);
                    resources->upload_queue()->upload_texture_2d(m_font.get(), 0, 1, &s);

                    io.Fonts->TexID = (void *)m_font.get();

                    io.MousePos = ImVec2(100, 100);
                });

                g.run([this, resources]
                {
                    m_imgui_pso = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::imgui_graphics::create_pso);
                });

                g.run([this]()
                {
                    ImGuiIO& io = ImGui::GetIO();
                    
                    io.KeyMap[ImGuiKey_Tab]         = ABI::Windows::System::VirtualKey::VirtualKey_Tab;// VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
                    io.KeyMap[ImGuiKey_LeftArrow]   = ABI::Windows::System::VirtualKey::VirtualKey_Left;    //= VK_LEFT;
                    io.KeyMap[ImGuiKey_RightArrow]  = ABI::Windows::System::VirtualKey::VirtualKey_Right;   //VK_RIGHT;
                    io.KeyMap[ImGuiKey_UpArrow]     = ABI::Windows::System::VirtualKey::VirtualKey_Up;      //VK_UP;
                    io.KeyMap[ImGuiKey_DownArrow]   = ABI::Windows::System::VirtualKey::VirtualKey_Down;    //VK_DOWN;
                    io.KeyMap[ImGuiKey_PageUp]      = ABI::Windows::System::VirtualKey::VirtualKey_PageUp;  //VK_PRIOR;
                    io.KeyMap[ImGuiKey_PageDown]    = ABI::Windows::System::VirtualKey::VirtualKey_PageDown; // VK_NEXT;
                    io.KeyMap[ImGuiKey_Home]        = ABI::Windows::System::VirtualKey::VirtualKey_Home; //VK_HOME;
                    io.KeyMap[ImGuiKey_End]         = ABI::Windows::System::VirtualKey::VirtualKey_End; //VK_END;
                    io.KeyMap[ImGuiKey_Delete]      = ABI::Windows::System::VirtualKey::VirtualKey_Delete; //VK_DELETE;
                    io.KeyMap[ImGuiKey_Backspace]   = ABI::Windows::System::VirtualKey::VirtualKey_Back;    //VK_BACK;
                    io.KeyMap[ImGuiKey_Enter]       = ABI::Windows::System::VirtualKey::VirtualKey_Enter;   //VK_RETURN;
                    io.KeyMap[ImGuiKey_Escape]      = ABI::Windows::System::VirtualKey::VirtualKey_Escape; //VK_ESCAPE;
                    io.KeyMap[ImGuiKey_A] = ABI::Windows::System::VirtualKey::VirtualKey_A; //'A';
                    io.KeyMap[ImGuiKey_C] = ABI::Windows::System::VirtualKey::VirtualKey_C; //'C';
                    io.KeyMap[ImGuiKey_V] = ABI::Windows::System::VirtualKey::VirtualKey_V; //'V';
                    io.KeyMap[ImGuiKey_X] = ABI::Windows::System::VirtualKey::VirtualKey_X; //'X';
                    io.KeyMap[ImGuiKey_Y] = ABI::Windows::System::VirtualKey::VirtualKey_Y; //'Y';
                    io.KeyMap[ImGuiKey_Z] = ABI::Windows::System::VirtualKey::VirtualKey_Z; //'Z';
                });

                g.run([this]()
                {
                    m_render_world_manager->show_world(gxu::world_id::non_linear_moment_shadow_maps_32);
                    //m_render_world_manager->show_world(gxu::world_id::scene2);
                });

                io::console::get_updater()->register_notifier( this );
                io::console::write(L"hello");

                g.wait();
            }

            options_page::~options_page()
            {
                io::console::get_updater()->register_notifier( this ) ;
            }

            static bool enable_statistics   = false;
            static bool enable_console      = false;
            static bool enable_log          = false;

            static void show_example_menu_file( update_context* ctx )
            {
                ImGui::MenuItem("Statistics", "", &enable_statistics);
                ImGui::MenuItem("Console", "", &enable_console);
                ImGui::MenuItem("Log", "", &enable_log);

                if (ImGui::MenuItem("Quit", "Alt+F4"))
                {
                    *ctx->m_window_close = true;
                }
            }
            /*
            static const char* menu_names[] =
            {
                "Robot",
                "Military Mechanic",
                "Bear",
                "Deer",
                "Skeleton",
                "Mid Poly Robot",
                "Full Screen Color",
                "Control Robot",
                "Indexed Mesh"
            };

            static gxu::world_id scene_id[] =
            {
                gxu::world_id::scene0,
                gxu::world_id::scene1,
                gxu::world_id::scene2,

                gxu::world_id::scene3,
                gxu::world_id::scene4,
                gxu::world_id::scene5,
                gxu::world_id::scene6,
                gxu::world_id::scene7,
                gxu::world_id::scene8
            };
            */

            static const char* menu_names[] =
            {
                "Bear",
                "Deer",
                "Full Screen Color",
                "Skeleton",
                "Robot",
                "Military Mechanic",
                "Shadows - Moment Shadow Maps",
                "Shadows - Non Linear Moment Shadow Maps 64",
                "Shadows - Non Linear Moment Shadow Maps 32"
            };

            static gxu::world_id scene_id[] =
            {
                gxu::world_id::scene2,
                gxu::world_id::scene3,
                gxu::world_id::scene6,
                gxu::world_id::scene4,
                gxu::world_id::scene0,
                gxu::world_id::scene1,
                gxu::world_id::moment_shadow_maps,
                gxu::world_id::non_linear_moment_shadow_maps_64,
                gxu::world_id::non_linear_moment_shadow_maps_32
            };

            constexpr size_t  menu_count = sizeof(menu_names) / sizeof(menu_names[0]);

            static std::array<bool, menu_count> show_scene_this_frame;
            static std::array<bool, menu_count> show_scene_last_frame;
            
            static void show_scenes()
            {
                show_scene_this_frame = show_scene_last_frame;

                for (auto i = 0U; i < menu_count; ++i)
                {
                    
                    if ( ImGui::MenuItem(menu_names[i], "", &show_scene_this_frame[i]) )
                    {
                        show_scene_this_frame[i] = true;

                        for (auto j = 0U; j < menu_count; ++j)
                        {
                            if (j != i)
                            {
                                show_scene_this_frame[j] = false;
                            }
                        }
                    }
                }
            }

            static void show_example_main_menu_bar(update_context* ctx)
            {
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        show_example_menu_file(ctx);
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Scenes"))
                    {
                        show_scenes();
                        ImGui::EndMenu();
                    }

                    ImGui::EndMainMenuBar();
                }
            }

            static void show_example_app_console(bool* p_open)
            {
                static ExampleAppConsole console;
                console.Draw("Example: Console", p_open);
            }

            static void show_example_app_log(bool* p_open, const std::vector<io::console::message>&  m)
            {
                static ExampleAppLog log;
                log.clear();

                for (auto&& i : m)
                {
                    std::string s;

                    s = util::utf8_from_utf16(i.m_text);
                    log.add_log("%s", s.c_str());
                }

                log.draw("Example: Log", p_open);
            }

            void options_page::do_update( update_context* ctx)
            {
                auto& io = ImGui::GetIO();

                // Setup display size (every frame to accommodate for window resizing)
                io.DisplaySize = ImVec2((float)(ctx->m_front_buffer_size.m_width), ctx->m_front_buffer_size.m_height);
                io.MouseDrawCursor  = true;
                io.DeltaTime = static_cast<float>(ctx->m_frame_time);

                const float movement_camera_speed = static_cast<float>( 300.0f * ctx->m_frame_time );

                auto pad = ctx->m_pad_state;

                if (pad.m_state.m_thumb_left_x != 0.0f || pad.m_state.m_thumb_left_y != 0.0f)
                {
                    auto move_x = std::copysign(movement_camera_speed, pad.m_state.m_thumb_left_x);
                    auto move_y = std::copysign(movement_camera_speed, pad.m_state.m_thumb_left_y);

                    auto a = fabsf(pad.m_state.m_thumb_left_x);
                    auto b = fabsf(pad.m_state.m_thumb_left_y);

                    if (a > b)
                    {
                        io.MousePos.x += move_x;
                    }
                    else
                    {
                        io.MousePos.y -= move_y;
                    }

                    io.MouseDown[0] = pad.is_button_down(io::pad_state::button_a);
                    io.MouseDown[1] = pad.is_button_down(io::pad_state::button_b);
                    io.MouseDown[2] = pad.is_button_down(io::pad_state::button_y);
                }

                if (pad.m_state.m_thumb_right_x != 0.0f || pad.m_state.m_thumb_right_x != 0.0f)
                {
                    const float mouse_wheel_speed = static_cast<float>(100.0f * ctx->m_frame_time);
                    auto move_y = std::copysign(mouse_wheel_speed, pad.m_state.m_thumb_right_x);

                    io.MouseWheel = -move_y;
                }

                auto keyboard = ctx->m_keyboard_state;
                for ( auto i = 0; i < 256; ++i )
                {
                    io.KeysDown[i] = keyboard.is_button_down(i);
                }

                if (!keyboard.m_character_codes.empty())
                {
                    for (auto&& i : keyboard.m_character_codes)
                    {
                        io.AddInputCharacter(static_cast<ImWchar>(i));
                    }
                }
                
                //active -> true
                if (true)
                {
                    auto mouse      = ctx->m_mouse_state;

                    io.MousePos.x   = mouse.x();
                    io.MousePos.y   = mouse.y();

                    io.MouseDown[0] = mouse.is_button_down(io::mouse_state::button_0);
                    io.MouseDown[1] = mouse.is_button_down(io::mouse_state::button_1);
                    io.MouseDown[2] = mouse.is_button_down(io::mouse_state::button_2);
                }

                // Start the frame
                ImGui::NewFrame();
                show_example_main_menu_bar(ctx);

                if (enable_console)
                {
                    show_example_app_console(&enable_statistics);
                }

                if( enable_statistics )
                {
                    ImGui::ShowMetricsWindow(&enable_console);
                }

                if (enable_log)
                {
                    show_example_app_log(&enable_log, m_message);
                }

                for ( auto i = 0U; i < menu_count; ++i )
                {
                    if (show_scene_this_frame[i] == true && show_scene_last_frame[i] == false)
                    {
                        m_render_world_manager->show_world( scene_id[i] );
                    }

                    show_scene_this_frame[i] = show_scene_last_frame[i];
                }
            }

            std::unique_ptr< submitable >  options_page::do_render( render_context* ctx )
            {
                ImGui::Render();

                auto draw_data = ImGui::GetDrawData();

                auto resources = ctx->m_resources;

                auto graphics = gx::dx12::create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::overlay));
                auto&& back_buffer = resources->back_buffer(device_resources::swap_chains::overlay);
                graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
                graphics->set_render_target(back_buffer);
                graphics->clear(back_buffer);

                if (draw_data)
                {
                    //Per pass  -> frequency 0
                    graphics->set_view_port(viewport( ImGui::GetIO() ));
                    graphics->set_scissor_rectangle(scissor(ImGui::GetIO()));

                    graphics->set_descriptor_heaps();

                    //Per many draw calls  -> frequency 1
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    graphics->set_pso(m_imgui_pso);

                    size_t vertex_buffer_size = 0;
                    size_t index_buffer_size = 0;

                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList* cmd_list = draw_data->CmdLists[n];
                        vertex_buffer_size  += cmd_list->VtxBuffer.Size;
                        index_buffer_size   += cmd_list->IdxBuffer.Size;
                    }

                    std::vector<ImDrawVert>   vertices(vertex_buffer_size);
                    std::vector<ImDrawIdx>    indices(index_buffer_size);

                    ImDrawVert* vtx_dst = &vertices[0];
                    ImDrawIdx* idx_dst  = &indices[0];

                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList* cmd_list = draw_data->CmdLists[n];

                        sys::memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                        sys::memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                        
                        vtx_dst += cmd_list->VtxBuffer.Size;
                        idx_dst += cmd_list->IdxBuffer.Size;
                    }

                    graphics->set_dynamic_index_buffer_16( resources->upload_queue()->upload_buffer(&indices[0], indices.size() * sizeof(ImDrawIdx)) );
                    graphics->set_dynamic_vertex_buffer(0, sizeof(ImDrawVert), resources->upload_queue()->upload_buffer( &vertices[0], vertices.size() * sizeof(ImDrawVert)) );

                    frame_constants frame;

                    frame.m_view = math::identity_matrix();
                    frame.m_perspective = math::transpose(projection_matrix(ImGui::GetIO()));

                    draw_constants draw;
                    draw.m_world = math::identity_matrix();

                    graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                    graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);

                    // Render command lists
                    int vtx_offset = 0;
                    int idx_offset = 0;
                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList* cmd_list = draw_data->CmdLists[n];
                        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
                        {
                            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                            if (pcmd->UserCallback)
                            {
                                pcmd->UserCallback(cmd_list, pcmd);
                            }
                            else
                            {
                                graphics->set_scissor_rectangle(scissor(pcmd->ClipRect));
                                gx::dx12::gpu_texture_2d* texture = reinterpret_cast<gx::dx12::gpu_texture_2d*> (pcmd->TextureId);
                                graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, texture->srv());
                                graphics->draw_indexed(pcmd->ElemCount, idx_offset, vtx_offset);
                            }
                            idx_offset += pcmd->ElemCount;
                        }
                        vtx_offset += cmd_list->VtxBuffer.Size;
                    }
                }

                graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
                //now we submit the graphics work also
                return std::make_unique<graphics_submitable>(std::move(graphics));
            }

            void options_page::do_handle_messages(const io::console::message* message_begin, const io::console::message* message_end)
            {
                message_begin;
                message_end;
                auto m = message_begin;

                //todo: put directly into the log
                if (m_message.size() > 4000)
                {
                    m_message.clear();
                }

                for ( m ; m!= message_end; m++)
                {
                    m_message.push_back(*m);
                }
            }
        }
    }
}