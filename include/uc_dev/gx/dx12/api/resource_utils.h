#pragma once

#include <d3d12.h>
#include <cstdint>
#include <wrl/client.h>

#include <uc_dev/mem/alloc.h>
#include <uc_dev/sys/memcpy.h>

#include "helpers.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            //copies subresources row by row
            inline void memcpy_subresource(const D3D12_MEMCPY_DEST* __restrict destination, const D3D12_SUBRESOURCE_DATA* __restrict source, uint64_t row_size_in_bytes, uint32_t row_count, uint32_t slice_count)
            {
                auto source_address      = reinterpret_cast<uintptr_t> (source->pData);
                auto destination_address = reinterpret_cast<uintptr_t> (destination->pData);

                for (auto z = 0U; z < slice_count; ++z)
                {
                    auto destination_slice = destination_address + destination->SlicePitch * z;
                    auto source_slice      = source_address + source->SlicePitch * z;

                    for (auto y = 0U; y < row_count; ++y)
                    {
                        auto destination_row = destination_slice + destination->RowPitch * y;
                        auto source_row      = source_slice + source->RowPitch * y;

                        sys::memcpy(reinterpret_cast<void*>(destination_row), reinterpret_cast<const void*>(source_row), static_cast<size_t>(row_size_in_bytes));
                    }
                }
            }

            //------------------------------------------------------------------------------------------------
            // Returns required size of a buffer to be used for data upload
            inline uint64_t resource_size( ID3D12Resource* resource, _In_range_(0, D3D12_REQ_SUBRESOURCES) uint32_t first_sub_resource, _In_range_(0, D3D12_REQ_SUBRESOURCES - first_sub_resource) uint32_t sub_resource_count)
            {
                D3D12_RESOURCE_DESC desc = resource->GetDesc();
                uint64_t r = 0;

                Microsoft::WRL::ComPtr<ID3D12Device> d;

                throw_if_failed(resource->GetDevice(IID_PPV_ARGS(&d)));
                d->GetCopyableFootprints(&desc, first_sub_resource, sub_resource_count, 0, nullptr, nullptr, nullptr, &r);

                return r;
            }

            //------------------------------------------------------------------------------------------------
            // All arrays must be populated (e.g. by calling GetCopyableFootprints)
            inline UINT64 UpdateSubresources(
                _In_ ID3D12GraphicsCommandList* __restrict      cmd_list,
                _In_ ID3D12Resource* __restrict                 destination,
                _In_ ID3D12Resource* __restrict                 intermediate,
                _In_range_(0, D3D12_REQ_SUBRESOURCES) uint32_t  first_sub_resource,
                _In_range_(0, D3D12_REQ_SUBRESOURCES - first_sub_resource) uint32_t sub_resource_count,
                UINT64 required_size,
                _In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* __restrict layouts,
                _In_reads_(NumSubresources) const uint32_t* __restrict row_count,
                _In_reads_(NumSubresources) const uint64_t* __restrict row_sizes_in_bytes,
                _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* __restrict source)
            {
                // Minor validation
                D3D12_RESOURCE_DESC desc0 = intermediate->GetDesc();
                D3D12_RESOURCE_DESC desc1 = destination->GetDesc();

                if ( desc0.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER || desc0.Width < required_size + layouts[0].Offset || required_size >(SIZE_T) - 1 ||
                    (desc1.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&  (first_sub_resource != 0 || sub_resource_count != 1)))
                {
                    throw_if_failed(E_FAIL);
                }

                uint8_t* pData;
                throw_if_failed( intermediate->Map(0, NULL, reinterpret_cast<void**>(&pData)) );

                //1. copy row by row, subresource by subresource to the upload data
                for (UINT i = 0; i < sub_resource_count; ++i)
                {
                    if ( row_sizes_in_bytes[i] >(SIZE_T)-1 )
                    {
                        intermediate->Unmap(0, NULL);
                        throw_if_failed(E_FAIL);
                    }

                    D3D12_MEMCPY_DEST DestData = { pData + layouts[i].Offset, layouts[i].Footprint.RowPitch, layouts[i].Footprint.RowPitch * row_count[i] };
                    
                    memcpy_subresource(&DestData, &source[i], (SIZE_T) row_sizes_in_bytes[i], row_count[i], layouts[i].Footprint.Depth);
                }

                intermediate->Unmap(0, NULL);

                //copy form the upload heap to the gpu
                if (desc1.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
                {
                    D3D12_BOX SrcBox = { UINT(layouts[0].Offset), UINT(layouts[0].Offset + layouts[0].Footprint.Width) };
                    cmd_list->CopyBufferRegion( destination, 0, intermediate, layouts[0].Offset, layouts[0].Footprint.Width);
                }
                else
                {
                    for (UINT i = 0; i < sub_resource_count; ++i)
                    {
                        D3D12_TEXTURE_COPY_LOCATION Dst = { destination,   D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX, i + first_sub_resource };
                        D3D12_TEXTURE_COPY_LOCATION Src = { intermediate,  D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,  layouts[i] };
                        cmd_list->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
                    }
                }

                return required_size;
            }


            //------------------------------------------------------------------------------------------------
            // Stack-allocating UpdateSubresources implementation
            template <UINT max_sub_resources>
            inline UINT64 UpdateSubresources(
                _In_ ID3D12GraphicsCommandList* __restrict cmd_list,
                _In_ ID3D12Resource* __restrict destination,
                _In_ ID3D12Resource* __restrict intermediate,
                uint64_t intermediate_offset,
                _In_range_(0, max_sub_resources) uint32_t first_sub_resource,
                _In_range_(1, max_sub_resources - first_sub_resource) uint32_t sub_resource_count,
                _In_reads_(sub_resource_count) D3D12_SUBRESOURCE_DATA*__restrict source )
            {
                uint64_t                           required_size = 0;
                D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[max_sub_resources];
                uint32_t                           row_count[max_sub_resources];
                uint64_t                           row_sizes_in_bytes[max_sub_resources];

                {
                    D3D12_RESOURCE_DESC desc = pDestinationResource->GetDesc();
                    Microsoft::WRL::ComPtr<ID3D12Device> d;
                    throw_if_failed(destination->GetDevice(IID_PPV_ARGS(&d)));
                    d->GetCopyableFootprints(&desc, first_sub_resource, sub_resource_count, intermediate_offset, layouts, row_count, row_sizes_in_bytes, &required_size);
                }

                return UpdateSubresources(cmd_list, destination, intermediate, first_sub_resource, sub_resource_count, required_size, layouts, row_count, row_sizes_in_bytes, source);
            }

            //------------------------------------------------------------------------------------------------
            // Stack-allocating UpdateSubresources implementation
            template <UINT max_sub_resources>
            inline UINT64 UpdateSubresources(
                _In_ ID3D12GraphicsCommandList* __restrict cmd_list,
                _In_ ID3D12Resource* __restrict destination,
                _In_ ID3D12Resource* __restrict intermediate,
                _In_range_(0, max_sub_resources) uint32_t first_sub_resource,
                _In_range_(1, max_sub_resources - first_sub_resource) uint32_t sub_resource_count,
                _In_reads_(sub_resource_count) D3D12_SUBRESOURCE_DATA*__restrict source)
            {
                return UpdateSubresources<max_sub_resources>(cmd_list, destination, intermediate, 0, first_sub_resource, sub_resource_count, source);
            }
        }
    }
}
