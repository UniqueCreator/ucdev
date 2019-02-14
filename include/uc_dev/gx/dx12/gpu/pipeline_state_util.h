#pragma once

#include <d3d12.h>

#include "resource_create_context.h"


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            template < typename factory > inline auto create_pso(ID3D12Device* d, gpu_resource_create_context* ctx, factory f ) 
            {
                return f( d, ctx->null_cbv(), ctx->null_srv(), ctx->null_uav(), ctx->null_sampler() );
            }
        }
    }
}
