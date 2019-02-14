#include "pch.h"

#include "uc_uwp_librocket_render_interface.h"

#include <uc_dev/util/utf8_conv.h>
#include <uc_dev/gx/img_utils.h>

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            render_interface::render_interface(gx::dx12::gpu_resource_create_context* ctx, gx::dx12::gpu_upload_queue* queue) : 
                m_create_context(ctx)
                , m_upload_queue(queue)
            {

            }

            render_interface::~render_interface()
            {

            }

            void render_interface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation )
            {
                draw_command d = { reinterpret_cast<vertex*>( vertices),static_cast<uint32_t> (num_vertices), reinterpret_cast<uint32_t*>(indices), static_cast<uint32_t>(num_indices), reinterpret_cast<gx::dx12::gpu_texture_2d*>(texture) };

                d.m_translation[0] = translation.x;
                d.m_translation[1] = translation.y;

                m_draw_data.emplace_back(std::move(d));
            }

            Rocket::Core::CompiledGeometryHandle render_interface::CompileGeometry(Rocket::Core::Vertex*, int32_t, int32_t*, int32_t, Rocket::Core::TextureHandle)
            {
                return 0;
            }

            void render_interface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle, const Rocket::Core::Vector2f&)
            {
                
            }

            void render_interface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle)
            {
                
            }

            void render_interface::EnableScissorRegion(bool )
            {
               
            }

            void render_interface::SetScissorRegion(int, int, int, int)
            {
            }

            bool render_interface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
            {
                const std::wstring p = util::utf16_from_utf8(source.CString());
                const auto r0 = gx::imaging::read_image(p.c_str());

                const auto w = r0.width();
                const auto h = r0.height();

                const auto texture = m_create_context->create_texture_2d(w, h, DXGI_FORMAT_R8G8B8A8_UNORM);

                D3D12_SUBRESOURCE_DATA s = {};
                s.pData = r0.pixels().get_pixels_cpu();
                s.RowPitch = (w * r0.bpp() + 7) / 8;
                s.SlicePitch = s.RowPitch * h;

                m_upload_queue->upload_texture_2d(texture, 0, 1, &s);

                texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
                texture_dimensions.x = w;
                texture_dimensions.y = h;
                return true;
            }

            bool render_interface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions)
            {
                const auto bpp = 32;
                const auto w = source_dimensions.x;
                const auto h = source_dimensions.y;

                const auto texture = m_create_context->create_texture_2d(w, h, DXGI_FORMAT_R8G8B8A8_UNORM);

                D3D12_SUBRESOURCE_DATA s = {};
                s.pData                  = source;
                s.RowPitch               = ( w * bpp + 7) / 8;
                s.SlicePitch             = s.RowPitch * h;

                m_upload_queue->upload_texture_2d(texture, 0, 1, &s);

                texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
                return true;
            }

            void render_interface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
            {
                const auto texture = reinterpret_cast<gx::dx12::gpu_texture_2d*>(texture_handle);
                m_create_context->free_texture_2d(texture);
            }

            float render_interface::GetHorizontalTexelOffset()
            {
                return 0.0f;
            }

            float render_interface::GetVerticalTexelOffset()
            {
                return 0.0f;
            }

            float render_interface::GetPixelsPerInch()
            {
                //todo: change dpi
                return 96.0f;// base::GetPixelsPerInch();
            }

            void render_interface::Release()
            {
                base::Release();
            }

            void render_interface::reset()
            {
                m_draw_data.clear();
            }
        }

    }
}
