#pragma once

#include <memory>
#include <uc_dev/util/noncopyable.h>
#include <uc_dev/gx/dx12/dx12.h>
#include <random>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class gpu_resource_create_context;
            class gpu_upload_queue;
            class gpu_texture_2d_array;
        }

        namespace blue_noise
        {
            class ldr_rg01_64x64 : private util::noncopyable
            {
                public:

                ldr_rg01_64x64(dx12::managed_gpu_texture_2d_array&& textures);

                ldr_rg01_64x64(ldr_rg01_64x64&& o)              = default;
                ldr_rg01_64x64& operator= (ldr_rg01_64x64&&o)   = default;

                const dx12::gpu_texture_2d_array* texture_array_2d() const
                {
                    return m_textures.get();
                }

                dx12::gpu_texture_2d_array* texture_array_2d()
                {
                    return m_textures.get();
                }

                dx12::descriptor_handle srv() const
                {
                    return m_textures->srv();
                }

                uint32_t random_index();
                uint32_t random_x();
                uint32_t random_y();

                private:
                dx12::managed_gpu_texture_2d_array m_textures;
                uint32_t                           m_index;

                std::random_device                 m_device;
                std::mt19937                       m_generator0;         //Standard mersenne_twister_engine seeded with rd()
                std::mt19937                       m_generator1;         //Standard mersenne_twister_engine seeded with rd()
                std::mt19937                       m_generator2;         //Standard mersenne_twister_engine seeded with rd()
                std::uniform_int_distribution<>    m_distribution0;
                std::uniform_int_distribution<>    m_distribution1;
                std::uniform_int_distribution<>    m_distribution2;
            };

            std::unique_ptr<ldr_rg01_64x64> make_blue_noise(dx12::gpu_resource_create_context*  rc, dx12::gpu_upload_queue* upload );
        }
    }
}
