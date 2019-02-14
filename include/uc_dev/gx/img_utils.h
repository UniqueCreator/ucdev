#pragma once


#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/img/img.h>

namespace uc
{
    namespace gx
    {
        inline D3D12_SUBRESOURCE_DATA sub_resource_data( const imaging::cpu_texture& t)
        {
            D3D12_SUBRESOURCE_DATA s = {};

            s.pData = t.pixels().get_pixels_cpu();
            s.RowPitch = t.row_pitch();
            s.SlicePitch = t.slice_pitch();

            return s;
        }

    }
    
}
