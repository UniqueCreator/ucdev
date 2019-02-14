#pragma once

#include <d3d12.h>
#include <cstdint>

#include <uc_dev/mem/alloc.h>
#include "pixel_format.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline D3D12_HEAP_PROPERTIES upload_properties()
            {
                D3D12_HEAP_PROPERTIES d;
                d.Type = D3D12_HEAP_TYPE_UPLOAD;
                d.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
                d.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
                d.VisibleNodeMask = 1;
                d.CreationNodeMask = 1;

                return d;
            }

            inline D3D12_HEAP_PROPERTIES default_properties()
            {
                D3D12_HEAP_PROPERTIES d;
                d.Type = D3D12_HEAP_TYPE_DEFAULT;
                d.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
                d.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
                d.VisibleNodeMask = 1;
                d.CreationNodeMask = 1;

                return d;
            }

            inline D3D12_HEAP_PROPERTIES read_back_properties()
            {
                D3D12_HEAP_PROPERTIES d;
                d.Type = D3D12_HEAP_TYPE_READBACK;
                d.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
                d.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
                d.VisibleNodeMask = 1;
                d.CreationNodeMask = 1;

                return d;
            }

            inline D3D12_HEAP_DESC upload_heap(uint64_t size, D3D12_HEAP_FLAGS flags )
            {
                D3D12_HEAP_DESC d = {};
                d.Properties = upload_properties();
                d.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
                d.SizeInBytes = mem::align(size, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
                d.Flags = flags;
                return d;
            }

            inline D3D12_HEAP_DESC default_heap(uint64_t size, D3D12_HEAP_FLAGS flags)
            {
                D3D12_HEAP_DESC d = {};
                d.Properties = default_properties();
                d.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
                d.SizeInBytes = mem::align(size, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
                d.Flags = flags;

                return d;
            }

            inline D3D12_HEAP_DESC render_target_heap(uint64_t size, D3D12_HEAP_FLAGS flags)
            {
                D3D12_HEAP_DESC d = {};
                d.Properties = default_properties();
                d.Alignment = D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT;
                d.SizeInBytes = mem::align(size, D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT);
                d.Flags = flags;

                return d;
            }

            inline D3D12_HEAP_DESC read_back_heap(uint64_t size, D3D12_HEAP_FLAGS flags)
            {
                D3D12_HEAP_DESC d = {};
                d.Properties = read_back_properties();

                d.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
                d.SizeInBytes = mem::align(size, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
                d.Flags = flags;

                return d;
            }
            

            inline D3D12_RESOURCE_DESC describe_buffer(uint64_t elements, uint64_t elementSize = 1)
            {
                D3D12_RESOURCE_DESC desc = {};
                desc.Alignment = 0;
                desc.DepthOrArraySize = 1;
                desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
                desc.Flags = D3D12_RESOURCE_FLAG_NONE;
                desc.Format = DXGI_FORMAT_UNKNOWN;
                desc.Height = 1;
                desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                desc.MipLevels = 1;
                desc.SampleDesc.Count = 1;
                desc.SampleDesc.Quality = 0;
                desc.Width = elements * elementSize;
                return desc;
            }

            inline D3D12_RESOURCE_DESC describe_texture(uint32_t width, uint32_t height, uint32_t depth, uint32_t mips, DXGI_FORMAT format, uint32_t flags)
            {
                D3D12_RESOURCE_DESC Desc = {};
                Desc.Alignment = 0;
                Desc.DepthOrArraySize = static_cast<uint16_t>(depth);
                Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                Desc.Flags = (D3D12_RESOURCE_FLAGS)flags;
                Desc.Format = get_base_format(format);
                Desc.Height = height;
                Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
                Desc.MipLevels = static_cast<uint16_t>(mips);
                Desc.SampleDesc.Count = 1;
                Desc.SampleDesc.Quality = 0;
                Desc.Width = width;
                return Desc;
            }

            inline D3D12_RESOURCE_DESC describe_texture_mipmapped_2d(uint32_t width, uint32_t height, uint32_t mips, DXGI_FORMAT format, uint32_t flags = D3D12_RESOURCE_FLAG_NONE)
            {
                return describe_texture(width, height, 1, mips, format, flags);
            }

            inline D3D12_RESOURCE_DESC describe_texture_2d( uint32_t width, uint32_t height, DXGI_FORMAT format, uint32_t flags = D3D12_RESOURCE_FLAG_NONE )
            {
                return describe_texture_mipmapped_2d(width, height, 1, format, flags);
            }

            inline D3D12_RESOURCE_DESC describe_render_target(uint32_t width, uint32_t height, DXGI_FORMAT format, UINT flags = D3D12_RESOURCE_FLAG_NONE)
            {
                D3D12_RESOURCE_DESC Desc = {};
                Desc.Alignment = 0;
                Desc.DepthOrArraySize = (UINT16)1;
                Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                Desc.Flags = (D3D12_RESOURCE_FLAGS)flags;
                Desc.Format = get_base_format(format);
                Desc.Height = height;
                Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
                Desc.MipLevels = 1;
                Desc.SampleDesc.Count = 1;
                Desc.SampleDesc.Quality = 0;
                Desc.Width = width;
                return Desc;
            }

            inline D3D12_RESOURCE_DESC describe_color_buffer(UINT width, UINT height, DXGI_FORMAT format)
            {
                return describe_render_target(width, height, format, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
            }

            inline D3D12_RESOURCE_DESC describe_depth_buffer(UINT width, UINT height, DXGI_FORMAT format)
            {
                return describe_render_target(width, height, format, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
            }

            inline D3D12_RESOURCE_DESC describe_msaa_depth_buffer(UINT width, UINT height, DXGI_FORMAT format, uint32_t samples)
            {
                auto d = describe_depth_buffer(width, height, format);
                d.SampleDesc.Count = samples;
                return d;
            }
            
        }
    }
}
