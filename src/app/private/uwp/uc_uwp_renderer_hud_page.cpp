#include "pch.h"
#include "uc_uwp_renderer_hud_page.h"

#include <uc_dev/math/math.h>

#include <uc_dev/gx/dx12/gpu/texture_2d.h>

#include "librocket/uc_uwp_librocket_initializer.h"
#include <ppl.h>

#include <autogen/shaders/librocket_graphics.h>

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

            struct frame_constants
            {
                math::float4x4 m_view;
                math::float4x4 m_perspective;
            };

            struct draw_constants
            {
                math::float4x4 m_world;
            };

            inline  math::float4x4 projection_matrix(Rocket::Core::Vector2i & io)
            {
                float L = 0.0f;
                float R = static_cast<float>(io.x);
                float B = static_cast<float>(io.y);
                float T = 0.0f;

                math::float4x4 m;
                

                m.r[0] = math::set(2.0f / (R - L), 0.0f, 0.0f, 0.0f);
                m.r[1] = math::set(0.0f, 2.0f / (T - B), 0.0f, 0.0f);
                m.r[2] = math::set(0.0f, 0.0f, 0.5f, 0.0f);
                m.r[3] = math::set((R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f);


                //m2 = math::orthographic_lh( R, B, -1.0f, 1.0f);

                return m;
            }

            inline D3D12_VIEWPORT viewport(Rocket::Core::Vector2i & io)
            {
                D3D12_VIEWPORT v = {};
                v.Width = static_cast<float>(io.x);
                v.Height = static_cast<float>(io.y);
                v.MinDepth = 0.0f;
                v.MaxDepth = 1.0f;
                return v;
            }

            inline D3D12_RECT scissor(Rocket::Core::Vector2i & io)
            {
                D3D12_RECT r = {};
                r.right = static_cast<LONG>(io.x);
                r.bottom = static_cast<LONG>(io.y);
                return r;
            }

            inline int32_t get_key_modifier_state(const io::keyboard_state& state)
            {
                int32_t key_modifier_state = 0;

                if (io::button_is_down(state, io::keyboard_state::virtual_key_capital_lock))
                {
                    key_modifier_state |= Rocket::Core::Input::KM_CAPSLOCK;
                }

                if (io::button_is_down(state, io::keyboard_state::virtual_key_shift))
                {
                    key_modifier_state |= Rocket::Core::Input::KM_SHIFT;
                }

                if (io::button_is_down(state, io::keyboard_state::virtual_key_control))
                {
                    key_modifier_state |= Rocket::Core::Input::KM_CTRL;
                }


                if (io::button_is_down(state, io::keyboard_state::virtual_key_number_key_lock))
                {
                    key_modifier_state |= Rocket::Core::Input::KM_NUMLOCK;
                }

                if (io::button_is_down(state, io::keyboard_state::virtual_key_menu))
                {
                    key_modifier_state |= Rocket::Core::Input::KM_ALT;
                }

                return key_modifier_state;
            }


            hud_page::hud_page(initialize_context* ctx) : base(ctx)
            , m_initializer(&m_system_interface)
            , m_render_interface( ctx->m_resources->resource_create_context(), ctx->m_resources->upload_queue() )
            {
                auto resources = ctx->m_resources;

                Rocket::Core::SetFileInterface(&m_file_interface);
                Rocket::Core::SetRenderInterface(&m_render_interface);

                m_context = mem::intrusive_ptr<Rocket::Core::Context>(Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1600,900)), false);

                concurrency::task_group g;

                g.run([this, resources]
                {
                    m_pso = gx::dx12::create_pso(resources->device_d2d12(), resources->resource_create_context(), gx::dx12::librocket_graphics::create_pso);
                });

                //must load after the fonts
                g.run([this]
                {
                    Rocket::Core::String font_names[4];

                    font_names[0] = "Delicious-Roman.otf";
                    font_names[1] = "Delicious-Italic.otf";
                    font_names[2] = "Delicious-Bold.otf";
                    font_names[3] = "Delicious-BoldItalic.otf";

                    for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
                    {
                        Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String("appdata/ui/fonts/") + font_names[i]);
                    }

                    m_main_document = mem::intrusive_ptr<Rocket::Core::ElementDocument>(m_context->LoadDocument("appdata/ui/demo.rml"), false);
                    m_main_document->Show();
                });

                initialize_mapping();
                
                g.wait();

            }

            void hud_page::initialize_mapping()
            {
                std::memset(&m_key_mapping[0], 0, sizeof(m_key_mapping));

                m_key_mapping[io::keyboard_state::virtual_key_none]           = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_left_button]    = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_right_button]   = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_cancel]         = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_middle_button]  = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_x_button_1]     = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_x_button_2]     = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_back]           = Rocket::Core::Input::KI_BACK;
                m_key_mapping[io::keyboard_state::virtual_key_tab]            = Rocket::Core::Input::KI_TAB;
                m_key_mapping[io::keyboard_state::virtual_key_clear]          = Rocket::Core::Input::KI_CLEAR;
                m_key_mapping[io::keyboard_state::virtual_key_enter]          = Rocket::Core::Input::KI_RETURN;
                m_key_mapping[io::keyboard_state::virtual_key_shift]          = Rocket::Core::Input::KI_LSHIFT;
                m_key_mapping[io::keyboard_state::virtual_key_control]        = Rocket::Core::Input::KI_LCONTROL;
                m_key_mapping[io::keyboard_state::virtual_key_menu]           = Rocket::Core::Input::KI_LMENU;
                m_key_mapping[io::keyboard_state::virtual_key_pause]          = Rocket::Core::Input::KI_PAUSE;
                m_key_mapping[io::keyboard_state::virtual_key_capital_lock]   = Rocket::Core::Input::KI_CAPITAL;
                m_key_mapping[io::keyboard_state::virtual_key_kana]           = Rocket::Core::Input::KI_HANJA;
                m_key_mapping[io::keyboard_state::virtual_key_hangul]         = Rocket::Core::Input::KI_HANGUL;
                m_key_mapping[io::keyboard_state::virtual_key_junja]          = Rocket::Core::Input::KI_JUNJA;
                m_key_mapping[io::keyboard_state::virtual_key_final]          = Rocket::Core::Input::KI_FINAL;
                m_key_mapping[io::keyboard_state::virtual_key_hanja]          = Rocket::Core::Input::KI_HANJA;
                m_key_mapping[io::keyboard_state::virtual_key_kanji]          = Rocket::Core::Input::KI_KANJI;
                m_key_mapping[io::keyboard_state::virtual_key_escape]         = Rocket::Core::Input::KI_ESCAPE;
                m_key_mapping[io::keyboard_state::virtual_key_convert]        = Rocket::Core::Input::KI_CONVERT;
                m_key_mapping[io::keyboard_state::virtual_key_non_convert]    = Rocket::Core::Input::KI_NONCONVERT;
                m_key_mapping[io::keyboard_state::virtual_key_accept]         = Rocket::Core::Input::KI_ACCEPT;
                m_key_mapping[io::keyboard_state::virtual_key_mode_change]    = Rocket::Core::Input::KI_MODECHANGE;
                m_key_mapping[io::keyboard_state::virtual_key_space]          = Rocket::Core::Input::KI_SPACE;
                m_key_mapping[io::keyboard_state::virtual_key_page_up]        = Rocket::Core::Input::KI_PRIOR;
                m_key_mapping[io::keyboard_state::virtual_key_page_down]      = Rocket::Core::Input::KI_NEXT;
                m_key_mapping[io::keyboard_state::virtual_key_end]            = Rocket::Core::Input::KI_END;
                m_key_mapping[io::keyboard_state::virtual_key_home]           = Rocket::Core::Input::KI_HOME;
                m_key_mapping[io::keyboard_state::virtual_key_left]           = Rocket::Core::Input::KI_LEFT;
                m_key_mapping[io::keyboard_state::virtual_key_up]             = Rocket::Core::Input::KI_UP;
                m_key_mapping[io::keyboard_state::virtual_key_right]          = Rocket::Core::Input::KI_RIGHT;
                m_key_mapping[io::keyboard_state::virtual_key_down]           = Rocket::Core::Input::KI_DOWN;
                m_key_mapping[io::keyboard_state::virtual_key_select]         = Rocket::Core::Input::KI_SELECT;
                m_key_mapping[io::keyboard_state::virtual_key_print]          = Rocket::Core::Input::KI_PRINT;
                m_key_mapping[io::keyboard_state::virtual_key_execute]        = Rocket::Core::Input::KI_EXECUTE;
                m_key_mapping[io::keyboard_state::virtual_key_snapshot]       = Rocket::Core::Input::KI_SNAPSHOT;
                m_key_mapping[io::keyboard_state::virtual_key_insert]         = Rocket::Core::Input::KI_INSERT;
                m_key_mapping[io::keyboard_state::virtual_key_delete]         = Rocket::Core::Input::KI_DELETE;
                m_key_mapping[io::keyboard_state::virtual_key_help]           = Rocket::Core::Input::KI_HELP;
                m_key_mapping[io::keyboard_state::virtual_key_number_0]       = Rocket::Core::Input::KI_0;
                m_key_mapping[io::keyboard_state::virtual_key_number_1]       = Rocket::Core::Input::KI_1;
                m_key_mapping[io::keyboard_state::virtual_key_number_2]       = Rocket::Core::Input::KI_2;
                m_key_mapping[io::keyboard_state::virtual_key_number_3]       = Rocket::Core::Input::KI_3;
                m_key_mapping[io::keyboard_state::virtual_key_number_4]       = Rocket::Core::Input::KI_4;
                m_key_mapping[io::keyboard_state::virtual_key_number_5]       = Rocket::Core::Input::KI_5;
                m_key_mapping[io::keyboard_state::virtual_key_number_6]       = Rocket::Core::Input::KI_6;
                m_key_mapping[io::keyboard_state::virtual_key_number_7]       = Rocket::Core::Input::KI_7;
                m_key_mapping[io::keyboard_state::virtual_key_number_8]       = Rocket::Core::Input::KI_8;
                m_key_mapping[io::keyboard_state::virtual_key_number_9]       = Rocket::Core::Input::KI_9;
                m_key_mapping[io::keyboard_state::virtual_key_a] = Rocket::Core::Input::KI_A;
                m_key_mapping[io::keyboard_state::virtual_key_b] = Rocket::Core::Input::KI_B;
                m_key_mapping[io::keyboard_state::virtual_key_c] = Rocket::Core::Input::KI_C;
                m_key_mapping[io::keyboard_state::virtual_key_d] = Rocket::Core::Input::KI_D;
                m_key_mapping[io::keyboard_state::virtual_key_e] = Rocket::Core::Input::KI_E;
                m_key_mapping[io::keyboard_state::virtual_key_f] = Rocket::Core::Input::KI_F;
                m_key_mapping[io::keyboard_state::virtual_key_g] = Rocket::Core::Input::KI_G;
                m_key_mapping[io::keyboard_state::virtual_key_h] = Rocket::Core::Input::KI_H;
                m_key_mapping[io::keyboard_state::virtual_key_i] = Rocket::Core::Input::KI_I;
                m_key_mapping[io::keyboard_state::virtual_key_j] = Rocket::Core::Input::KI_J;
                m_key_mapping[io::keyboard_state::virtual_key_k] = Rocket::Core::Input::KI_K;
                m_key_mapping[io::keyboard_state::virtual_key_l] = Rocket::Core::Input::KI_L;
                m_key_mapping[io::keyboard_state::virtual_key_m] = Rocket::Core::Input::KI_M;
                m_key_mapping[io::keyboard_state::virtual_key_n] = Rocket::Core::Input::KI_N;
                m_key_mapping[io::keyboard_state::virtual_key_o] = Rocket::Core::Input::KI_O;
                m_key_mapping[io::keyboard_state::virtual_key_p] = Rocket::Core::Input::KI_P;
                m_key_mapping[io::keyboard_state::virtual_key_q] = Rocket::Core::Input::KI_Q;
                m_key_mapping[io::keyboard_state::virtual_key_r] = Rocket::Core::Input::KI_R;
                m_key_mapping[io::keyboard_state::virtual_key_s] = Rocket::Core::Input::KI_S;
                m_key_mapping[io::keyboard_state::virtual_key_t] = Rocket::Core::Input::KI_T;
                m_key_mapping[io::keyboard_state::virtual_key_u] = Rocket::Core::Input::KI_U;
                m_key_mapping[io::keyboard_state::virtual_key_v] = Rocket::Core::Input::KI_V;
                m_key_mapping[io::keyboard_state::virtual_key_w] = Rocket::Core::Input::KI_W;
                m_key_mapping[io::keyboard_state::virtual_key_x] = Rocket::Core::Input::KI_X;
                m_key_mapping[io::keyboard_state::virtual_key_y] = Rocket::Core::Input::KI_Y;
                m_key_mapping[io::keyboard_state::virtual_key_z] = Rocket::Core::Input::KI_Z;
                m_key_mapping[io::keyboard_state::virtual_key_left_windows]   = Rocket::Core::Input::KI_LWIN;
                m_key_mapping[io::keyboard_state::virtual_key_right_windows]  = Rocket::Core::Input::KI_RWIN;
                m_key_mapping[io::keyboard_state::virtual_key_application]    = Rocket::Core::Input::KI_APPS;
                m_key_mapping[io::keyboard_state::virtual_key_sleep]          = Rocket::Core::Input::KI_SLEEP;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_0]   = Rocket::Core::Input::KI_NUMPAD0;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_1]   = Rocket::Core::Input::KI_NUMPAD1;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_2]   = Rocket::Core::Input::KI_NUMPAD2;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_3]   = Rocket::Core::Input::KI_NUMPAD3;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_4]   = Rocket::Core::Input::KI_NUMPAD4;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_5]   = Rocket::Core::Input::KI_NUMPAD5;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_6]   = Rocket::Core::Input::KI_NUMPAD6;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_7]   = Rocket::Core::Input::KI_NUMPAD7;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_8]   = Rocket::Core::Input::KI_NUMPAD8;
                m_key_mapping[io::keyboard_state::virtual_key_number_pad_9]   = Rocket::Core::Input::KI_NUMPAD9;
                m_key_mapping[io::keyboard_state::virtual_key_multiply]       = Rocket::Core::Input::KI_MULTIPLY;
                m_key_mapping[io::keyboard_state::virtual_key_add]            = Rocket::Core::Input::KI_ADD;
                m_key_mapping[io::keyboard_state::virtual_key_separator]      = Rocket::Core::Input::KI_SEPARATOR;
                m_key_mapping[io::keyboard_state::virtual_key_subtract]       = Rocket::Core::Input::KI_SUBTRACT;
                m_key_mapping[io::keyboard_state::virtual_key_decimal]        = Rocket::Core::Input::KI_DECIMAL;
                m_key_mapping[io::keyboard_state::virtual_key_divide]         = Rocket::Core::Input::KI_DIVIDE;
                m_key_mapping[io::keyboard_state::virtual_key_f1]             = Rocket::Core::Input::KI_F1;
                m_key_mapping[io::keyboard_state::virtual_key_f2] = Rocket::Core::Input::KI_F2;
                m_key_mapping[io::keyboard_state::virtual_key_f3] = Rocket::Core::Input::KI_F3;
                m_key_mapping[io::keyboard_state::virtual_key_f4] = Rocket::Core::Input::KI_F4;
                m_key_mapping[io::keyboard_state::virtual_key_f5] = Rocket::Core::Input::KI_F5;
                m_key_mapping[io::keyboard_state::virtual_key_f6] = Rocket::Core::Input::KI_F6;
                m_key_mapping[io::keyboard_state::virtual_key_f7] = Rocket::Core::Input::KI_F7;
                m_key_mapping[io::keyboard_state::virtual_key_f8] = Rocket::Core::Input::KI_F8;
                m_key_mapping[io::keyboard_state::virtual_key_f9] = Rocket::Core::Input::KI_F9;
                m_key_mapping[io::keyboard_state::virtual_key_f10] = Rocket::Core::Input::KI_F10;
                m_key_mapping[io::keyboard_state::virtual_key_f11] = Rocket::Core::Input::KI_F11;
                m_key_mapping[io::keyboard_state::virtual_key_f12] = Rocket::Core::Input::KI_F12;
                m_key_mapping[io::keyboard_state::virtual_key_f13] = Rocket::Core::Input::KI_F13;
                m_key_mapping[io::keyboard_state::virtual_key_f14] = Rocket::Core::Input::KI_F14;
                m_key_mapping[io::keyboard_state::virtual_key_f15] = Rocket::Core::Input::KI_F15;
                m_key_mapping[io::keyboard_state::virtual_key_f16] = Rocket::Core::Input::KI_F16;
                m_key_mapping[io::keyboard_state::virtual_key_f17] = Rocket::Core::Input::KI_F17;
                m_key_mapping[io::keyboard_state::virtual_key_f18] = Rocket::Core::Input::KI_F18;
                m_key_mapping[io::keyboard_state::virtual_key_f19] = Rocket::Core::Input::KI_F19;
                m_key_mapping[io::keyboard_state::virtual_key_f20] = Rocket::Core::Input::KI_F20;
                m_key_mapping[io::keyboard_state::virtual_key_f21] = Rocket::Core::Input::KI_F21;
                m_key_mapping[io::keyboard_state::virtual_key_f22] = Rocket::Core::Input::KI_F22;
                m_key_mapping[io::keyboard_state::virtual_key_f23] = Rocket::Core::Input::KI_F23;
                m_key_mapping[io::keyboard_state::virtual_key_f24] = Rocket::Core::Input::KI_F24;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_view] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_menu] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_up] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_down] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_left] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_right] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_accept] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_navigation_cancel] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_number_key_lock] = Rocket::Core::Input::KI_NUMLOCK;
                m_key_mapping[io::keyboard_state::virtual_key_scroll] = Rocket::Core::Input::KI_SCROLL;
                m_key_mapping[io::keyboard_state::virtual_key_left_shift] = Rocket::Core::Input::KI_LSHIFT;
                m_key_mapping[io::keyboard_state::virtual_key_right_shift] = Rocket::Core::Input::KI_RSHIFT;
                m_key_mapping[io::keyboard_state::virtual_key_left_control] = Rocket::Core::Input::KI_LCONTROL;
                m_key_mapping[io::keyboard_state::virtual_key_right_control] = Rocket::Core::Input::KI_RCONTROL;
                m_key_mapping[io::keyboard_state::virtual_key_left_menu] = Rocket::Core::Input::KI_LMENU;
                m_key_mapping[io::keyboard_state::virtual_key_right_menu] = Rocket::Core::Input::KI_RMENU;
                m_key_mapping[io::keyboard_state::virtual_key_go_back] = Rocket::Core::Input::KI_BROWSER_BACK;
                m_key_mapping[io::keyboard_state::virtual_key_go_forward] = Rocket::Core::Input::KI_BROWSER_FORWARD;
                m_key_mapping[io::keyboard_state::virtual_key_refresh] = Rocket::Core::Input::KI_BROWSER_REFRESH;
                m_key_mapping[io::keyboard_state::virtual_key_stop] = Rocket::Core::Input::KI_BROWSER_STOP;
                m_key_mapping[io::keyboard_state::virtual_key_search] = Rocket::Core::Input::KI_BROWSER_SEARCH;
                m_key_mapping[io::keyboard_state::virtual_key_favorites] = Rocket::Core::Input::KI_BROWSER_FAVORITES;
                m_key_mapping[io::keyboard_state::virtual_key_goHome] = Rocket::Core::Input::KI_BROWSER_HOME;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_a] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_b] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_x] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_y] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_shoulder] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_shoulder] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_trigger] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_trigger] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_dpad_up] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_dpad_down] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_dpad_left] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_dpad_right] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_menu] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_view] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_thumbstick_button] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_thumbstick_button] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_thumbstick_up] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_thumbstick_down] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_thumbstick_right] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_left_thumbstick_left] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_thumbstick_up] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_thumbstick_down] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_thumbstick_right] = Rocket::Core::Input::KI_UNKNOWN;
                m_key_mapping[io::keyboard_state::virtual_key_gamepad_right_thumbstick_left] = Rocket::Core::Input::KI_UNKNOWN;
            }

            hud_page::~hud_page()
            {

            }

            void hud_page::do_update( update_context* ctx)
            {
                m_system_interface.set_frame_time( static_cast<float>(ctx->m_frame_time) );
                Rocket::Core::Vector2i d( ctx->m_front_buffer_size.m_width, ctx->m_front_buffer_size.m_height);
                m_context->SetDimensions(d);

                //active -> true
                if (true)
                {
                    auto mouse = ctx->m_mouse_state;

                    m_context->ProcessMouseMove(mouse.x(), mouse.y(), 0);
                   
                    if ( io::button_was_pressed(mouse, io::mouse_state::button_0 ) )
                    {
                        m_context->ProcessMouseButtonDown(0, 0);
                    }

                    if (io::button_was_released(mouse, io::mouse_state::button_0))
                    {
                        m_context->ProcessMouseButtonUp(0, 0);
                    }

                    if (io::button_was_pressed(mouse, io::mouse_state::button_1))
                    {
                        m_context->ProcessMouseButtonDown(1, 0);
                    }

                    if (io::button_was_released(mouse, io::mouse_state::button_1))
                    {
                        m_context->ProcessMouseButtonUp(1, 0);
                    }

                    if (io::button_was_pressed(mouse, io::mouse_state::button_2))
                    {
                        m_context->ProcessMouseButtonDown(2, 0);
                    }

                    if (io::button_was_released(mouse, io::mouse_state::button_2))
                    {
                        m_context->ProcessMouseButtonUp(2, 0);
                    }
                }

                if (true)
                {
                    auto keyboard = ctx->m_keyboard_state;
                    auto modifier_state = get_key_modifier_state(keyboard);
                    for (auto i = 0; i < io::keyboard_state::virtual_key_gamepad_right_thumbstick_left + 1; ++i)
                    {
                        if (io::button_was_pressed(keyboard, i))
                        {
                            m_context->ProcessKeyDown(m_key_mapping[i], modifier_state);
                        }

                        if (io::button_was_released(keyboard, i))
                        {
                            m_context->ProcessKeyUp(m_key_mapping[i], modifier_state);
                        }


                    }

                    if (!keyboard.m_character_codes.empty())
                    {
                        for (auto&& i : keyboard.m_character_codes)
                        {
                            m_context->ProcessTextInput(static_cast<Rocket::Core::word > ( i ));
                        }
                    }
                }

                m_context->Update();
            }

            std::unique_ptr< submitable >  hud_page::do_render( render_context* ctx )
            {
                m_context->Render();

                auto resources = ctx->m_resources;

                auto graphics = gx::dx12::create_graphics_command_context(resources->direct_command_context_allocator(device_resources::swap_chains::overlay));
                auto&& back_buffer = resources->back_buffer(device_resources::swap_chains::overlay);
                
                graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
                graphics->set_render_target(back_buffer);
                graphics->clear(back_buffer);

                if (m_render_interface.draw_data_size() > 0 )
                {
                    Rocket::Core::Vector2i view(ctx->m_front_buffer_size.m_width, ctx->m_front_buffer_size.m_height);

                    //Per pass  -> frequency 0
                    graphics->set_view_port( viewport(view));
                    graphics->set_scissor_rectangle(scissor(view));

                    graphics->set_descriptor_heaps();

                    //Per many draw calls  -> frequency 1
                    graphics->set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    graphics->set_pso(m_pso);

                    auto s = m_render_interface.draw_data_size();
                    for (auto i = 0U; i < s; ++i)
                    {
                        auto command = m_render_interface.draw_data(i);

                        graphics->set_dynamic_index_buffer_32(resources->upload_queue()->upload_buffer(command->m_indices, command->m_index_count * sizeof(uint32_t)));
                        graphics->set_dynamic_vertex_buffer( 0, sizeof(librocket::render_interface::vertex), resources->upload_queue()->upload_buffer(command->m_vertices, command->m_vertex_count * sizeof(librocket::render_interface::vertex)) );

                        frame_constants frame;
                        frame.m_view = math::identity_matrix();
                        frame.m_perspective = math::transpose(projection_matrix( view  ));

                        draw_constants draw;
                        draw.m_world = math::transpose(math::translation(command->m_translation[0], command->m_translation[1], 0.0f));

                        graphics->set_graphics_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_0, frame);
                        graphics->set_graphics_dynamic_constant_buffer(gx::dx12::default_root_singature::slots::constant_buffer_1, 0, draw);

                        gx::dx12::gpu_texture_2d* texture = reinterpret_cast<gx::dx12::gpu_texture_2d*> (command->m_texture);
                        graphics->set_graphics_dynamic_descriptor(gx::dx12::default_root_singature::slots::srv_1, texture->srv());


                       graphics->draw_indexed(command->m_index_count);

                    }
                }

                graphics->transition_resource(back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

                m_render_interface.reset();
                return std::make_unique<graphics_submitable>(std::move(graphics));
            }
        }
    }
}