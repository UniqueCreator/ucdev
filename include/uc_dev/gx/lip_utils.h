#pragma once

#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/lip/model.h>

namespace uc
{
    namespace gx
    {
        inline uint32_t bits_per_pixel( lip::storage_format f )
        {
            switch (f)
            {
                case lip::storage_format::r8_g8_b8_a8_unorm: return 32;
                case lip::storage_format::bc1:         return 4;
                case lip::storage_format::bc2:         return 8;
                case lip::storage_format::bc3:         return 8;
                case lip::storage_format::bc4:         return 4;
                case lip::storage_format::bc5:         return 8;
                default: assert(false); return 0;
            }
        }

        inline uint32_t block_width(uint32_t width, uint32_t block_size)
        {
            return (width + block_size - 1) / block_size;
        }

        inline bool is_block_format(lip::storage_format f)
        {
            switch (f)
            {
                case lip::storage_format::r8_g8_b8_a8_unorm: return false;
                case lip::storage_format::bc1: return true;
                case lip::storage_format::bc2: return true;
                case lip::storage_format::bc3: return true;
                case lip::storage_format::bc4: return true;
                case lip::storage_format::bc5: return true;

                default: assert(false); 
                         return false;
            }
        }

        inline uint32_t row_pitch( lip::storage_format f, uint32_t base_width)
        {
            auto bpp                = bits_per_pixel(f);
            auto block_size_pixels  = is_block_format(f) ? 4U : 1U;
            auto bits_per_block     = bpp * block_size_pixels * block_size_pixels;
            auto bytes_per_block    = bits_per_block / 8U;

            auto wb                 = block_width(base_width, block_size_pixels) * bytes_per_block;
            return wb;
        }

        inline uint32_t row_pitch( lip::storage_format f, uint32_t base_width, uint32_t mip)
        {
            return row_pitch(f, base_width >> mip);
        }

        inline uint32_t slice_pitch(lip::storage_format f, uint32_t base_width, uint32_t base_height, uint32_t mip = 0)
        {
            uint32_t w              = row_pitch(f, base_width, mip);
            auto block_size_pixels  = is_block_format(f) ? 4 : 1;
            auto wh                 = block_width(base_height >> mip, block_size_pixels);
            return w * wh;
        }

        inline D3D12_SUBRESOURCE_DATA sub_resource_data(const lip::texture2d* t)
        {
            D3D12_SUBRESOURCE_DATA s = {};

            auto w = t->m_width;
            auto h = t->m_height;

            s.pData = t->data();
            s.RowPitch = row_pitch(t->storage(), w);
            s.SlicePitch = slice_pitch(t->storage(), w, h);

            return s;
        }

        inline D3D12_SUBRESOURCE_DATA sub_resource_data( const lip::texture2d_mip_level* t )
        {
            D3D12_SUBRESOURCE_DATA s = {};

            auto w          = t->m_width;
            auto h          = t->m_height;
            
            s.pData         = t->data();
            s.RowPitch      = row_pitch(t->storage() , w);
            s.SlicePitch    = slice_pitch(t->storage(), w, h);

            return s;
        }
    }
    
}
