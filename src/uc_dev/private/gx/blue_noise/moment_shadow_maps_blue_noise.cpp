#include "pch.h"

#include <uc_dev/gx/blue_noise/moment_shadow_maps_blue_noise.h>

#include <uc_dev/gx/dx12/gpu/resource_create_context.h>
#include <uc_dev/gx/img/img.h>

namespace uc
{
    namespace gx
    {
        namespace blue_noise
        {
            struct noise_create_info
            {
                const uint8_t*    m_memory;
                size_t            m_memory_size;
            };

            static noise_create_info get_baked_noise()
            {
                #include "ldr_rg01_64x64.png.h"
                noise_create_info r = { &ldr_rg01_64x64_png[0], sizeof(ldr_rg01_64x64_png) };
                return r;
            }

            std::random_device& get_random_device()
            {
                static std::random_device d;
                return d;
            }

            std::unique_ptr<ldr_rg01_64x64> make_blue_noise( dx12::gpu_resource_create_context*  rc, dx12::gpu_upload_queue* upload )
            {
                noise_create_info r = get_baked_noise();
                auto img            = imaging::read_image(r.m_memory, r.m_memory_size);
                auto pixels         = img.pixels().get_pixels_cpu();

                auto blue_noise     = gx::dx12::create_texture_2d_array(rc, 64, 64, DXGI_FORMAT_R8G8_UINT, 64);

                std::vector< D3D12_SUBRESOURCE_DATA > s;
                s.resize(64);

                for (auto i = 0U; i < 64; ++i)
                {
                    auto&& s0     = s[i];
                    s0.pData      = pixels + i * 64 * sizeof(uint16_t);
                    s0.RowPitch   = 64 * 64 * sizeof(uint16_t);
                    s0.SlicePitch = 64 * 64 * 64 * sizeof(uint16_t);
                }

                upload->upload_texture_2d_array(blue_noise.get(), 0, 64, &s[0]);

                return std::make_unique<ldr_rg01_64x64>(std::move(blue_noise));
            }

            ldr_rg01_64x64::ldr_rg01_64x64(dx12::managed_gpu_texture_2d_array&& textures)
                : m_textures( std::move(textures) )
                , m_generator0( m_device() )
                , m_distribution0(0, 1)
                , m_distribution1(0, 1)
                , m_distribution2(0, 1)
            {
                random_index();
            }

            uint32_t ldr_rg01_64x64::random_index()
            {
                auto bit_0 = m_distribution0(m_generator0);
                auto bit_1 = m_distribution0(m_generator0);
                auto bit_2 = m_distribution0(m_generator0);
                auto bit_3 = m_distribution0(m_generator0);
                auto bit_4 = m_distribution0(m_generator0);
                auto bit_5 = m_distribution0(m_generator0);

                return (bit_5 << 5) | (bit_4 << 4) | (bit_3 << 3) | (bit_2 << 2) | (bit_1 << 1) | (bit_0 << 0);
            }

            uint32_t ldr_rg01_64x64::random_x()
            {
                auto bit_0 = m_distribution1(m_generator0);
                auto bit_1 = m_distribution1(m_generator0);
                auto bit_2 = m_distribution1(m_generator0);
                auto bit_3 = m_distribution1(m_generator0);
                auto bit_4 = m_distribution1(m_generator0);
                auto bit_5 = m_distribution1(m_generator0);

                return (bit_5 << 5) | (bit_4 << 4) | (bit_3 << 3) | (bit_2 << 2) | (bit_1 << 1) | (bit_0 << 0);
            }

            uint32_t ldr_rg01_64x64::random_y()
            {
                auto bit_0 = m_distribution2(m_generator0);
                auto bit_1 = m_distribution2(m_generator0);
                auto bit_2 = m_distribution2(m_generator0);
                auto bit_3 = m_distribution2(m_generator0);
                auto bit_4 = m_distribution2(m_generator0);
                auto bit_5 = m_distribution2(m_generator0);

                return (bit_5 << 5) | (bit_4 << 4) | (bit_3 << 3) | (bit_2 << 2) | (bit_1 << 1) | (bit_0 << 0);
            }
        }
    }
}


