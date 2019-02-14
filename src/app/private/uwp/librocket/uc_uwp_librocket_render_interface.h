#pragma once

#include <vector>

#include <Rocket/Core.h>

#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/gx/dx12/cmd/upload_queue.h>

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            class render_interface : public Rocket::Core::RenderInterface
            {
                using base = Rocket::Core::RenderInterface;

                public:
                struct vertex
                {
                    float       m_pos[2];
                    uint32_t    m_color;
                    float       m_uv[2];
                };


                struct draw_command
                {
                    vertex*                   m_vertices;
                    uint32_t                  m_vertex_count;

                    uint32_t*                 m_indices;
                    uint32_t                  m_index_count;

                    gx::dx12::gpu_texture_2d* m_texture;

                    float                     m_translation[2];
                };

                render_interface(gx::dx12::gpu_resource_create_context* ctx, gx::dx12::gpu_upload_queue* upload_queue);
                ~render_interface();

                void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation) override;
                Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture) override;
                void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)  override;
                void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry) override;
                void EnableScissorRegion(bool enable)  override;
                void SetScissorRegion(int x, int y, int width, int height)  override;
                bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)  override;
                bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions) override;
                void ReleaseTexture(Rocket::Core::TextureHandle texture) override;
                float GetHorizontalTexelOffset() override;
                float GetVerticalTexelOffset() override;
                float GetPixelsPerInch() override;
                void Release() override;

                const draw_command* draw_data() const
                {
                    return &m_draw_data[0];
                }

                size_t           draw_data_size() const
                {
                    return m_draw_data.size();
                }

                const draw_command* draw_data(uint32_t index) const
                {
                    return &m_draw_data[index];
                }

                void             reset();

                private:
                gx::dx12::gpu_resource_create_context* m_create_context;
                gx::dx12::gpu_upload_queue*            m_upload_queue;
                std::vector<draw_command>              m_draw_data;
            };
        }
    }
}
