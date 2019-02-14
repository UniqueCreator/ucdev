#pragma once

#include <d3d12.h>
#include <dxgi1_5.h>
#include <cstdint>
#include <wrl/client.h>

#include "error.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline  Microsoft::WRL::ComPtr<ID3D12Device> create_device(IDXGIAdapter* adapter = nullptr, D3D_FEATURE_LEVEL minimumFeatureLevel = D3D_FEATURE_LEVEL_11_0)
            {
                Microsoft::WRL::ComPtr<ID3D12Device> result;

                throw_if_failed(D3D12CreateDevice(adapter, minimumFeatureLevel, IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12CommandAllocator> create_command_allocator(ID3D12Device* d, D3D12_COMMAND_LIST_TYPE type)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandAllocator> result;
                throw_if_failed(d->CreateCommandAllocator(type, IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12CommandQueue> create_command_queue(ID3D12Device* d, const D3D12_COMMAND_QUEUE_DESC*  desc)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandQueue> result;

                throw_if_failed(d->CreateCommandQueue(desc, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12CommandQueue> create_heap(ID3D12Device* d, const D3D12_HEAP_DESC*  desc)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandQueue> result;
                throw_if_failed(d->CreateHeap(desc, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12CommandList> create_command_list(ID3D12Device* d, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator *command_allocator, ID3D12PipelineState *initial_state)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandList> result;
                throw_if_failed(d->CreateCommandList(nodeMask, type, command_allocator, initial_state, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> create_graphics_command_list(ID3D12Device* d, UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator *command_allocator, ID3D12PipelineState *initial_state)
            {
                Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> result;
                throw_if_failed(d->CreateCommandList(nodeMask, type, command_allocator, initial_state, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12CommandSignature> create_command_signature(ID3D12Device* d, const D3D12_COMMAND_SIGNATURE_DESC* desc, ID3D12RootSignature *root_signature)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandSignature> result;
                throw_if_failed(d->CreateCommandSignature(desc, root_signature, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12Resource> create_committed_resource(ID3D12Device* d, const D3D12_HEAP_PROPERTIES *heap_properties, D3D12_HEAP_FLAGS heap_flags, const D3D12_RESOURCE_DESC *resource_desc, D3D12_RESOURCE_STATES initial_resource_state, const D3D12_CLEAR_VALUE * optimized_clear_value)
            {
                Microsoft::WRL::ComPtr<ID3D12Resource> result;
                throw_if_failed(d->CreateCommittedResource(heap_properties, heap_flags, resource_desc, initial_resource_state, optimized_clear_value, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12PipelineState> create_compute_pipeline_state(ID3D12Device* d, const D3D12_COMPUTE_PIPELINE_STATE_DESC * desc)
            {
                Microsoft::WRL::ComPtr<ID3D12PipelineState> result;
                throw_if_failed(d->CreateComputePipelineState(desc, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> create_descriptor_heap(ID3D12Device* d, const D3D12_DESCRIPTOR_HEAP_DESC * desc)
            {
                Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> result;
                throw_if_failed(d->CreateDescriptorHeap(desc, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12Fence> create_fence(ID3D12Device* d, uint64_t initial_value, D3D12_FENCE_FLAGS flags)
            {
                Microsoft::WRL::ComPtr<ID3D12Fence> result;
                throw_if_failed(d->CreateFence(initial_value, flags, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12Fence> create_fence(ID3D12Device* d, uint64_t initial_value)
            {
                return create_fence(d, initial_value, D3D12_FENCE_FLAG_NONE);
            }

            inline  Microsoft::WRL::ComPtr<ID3D12Fence> create_fence(ID3D12Device* d)
            {
                return create_fence(d, 0, D3D12_FENCE_FLAG_NONE);
            }

            inline  Microsoft::WRL::ComPtr<ID3D12PipelineState> create_graphics_pipeline_state(ID3D12Device* d, const D3D12_GRAPHICS_PIPELINE_STATE_DESC * desc)
            {
                Microsoft::WRL::ComPtr<ID3D12PipelineState> result;
                throw_if_failed(d->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(ID3D12Device* d, ID3D12Heap * heap, UINT64 heap_offset, const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initial_state, const D3D12_CLEAR_VALUE * optimized_clear_value)
            {
                Microsoft::WRL::ComPtr<ID3D12Resource> result;
                throw_if_failed(d->CreatePlacedResource(heap, heap_offset, desc, initial_state, optimized_clear_value, IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12QueryHeap> create_query_heap(ID3D12Device* d, const D3D12_QUERY_HEAP_DESC * desc)
            {
                Microsoft::WRL::ComPtr<ID3D12QueryHeap> result;
                throw_if_failed(d->CreateQueryHeap(desc, IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12Resource> create_reserved_resource(ID3D12Device* d, const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initial_state, const D3D12_CLEAR_VALUE * optimized_clear_value)
            {
                Microsoft::WRL::ComPtr<ID3D12Resource> result;
                throw_if_failed(d->CreateReservedResource(desc, initial_state, optimized_clear_value, IID_PPV_ARGS(&result)));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12RootSignature> create_root_signature(ID3D12Device* d, uint32_t node_mask, const void *blob_with_root_signature, size_t blob_length_in_bytes)
            {
                Microsoft::WRL::ComPtr<ID3D12RootSignature> result;
                throw_if_failed(d->CreateRootSignature(node_mask, blob_with_root_signature, blob_length_in_bytes, IID_PPV_ARGS(&result)));
                return result;
            }

            inline HANDLE create_shared_handle(ID3D12Device* d, ID3D12DeviceChild *object, const SECURITY_ATTRIBUTES *attributes, DWORD access, LPCWSTR name)
            {
                HANDLE result;
                throw_if_failed(d->CreateSharedHandle(object, attributes, access, name, &result));
                return result;
            }

            inline Microsoft::WRL::ComPtr<ID3D12RootSignatureDeserializer> create_root_signature_deserializer(const void* byte_code, size_t byte_code_size)
            {
                Microsoft::WRL::ComPtr<ID3D12RootSignatureDeserializer> result;
                throw_if_failed(D3D12CreateRootSignatureDeserializer(byte_code, byte_code_size, IID_PPV_ARGS(&result)));
                return result;
            }

            inline  Microsoft::WRL::ComPtr<ID3D12Device> get_device(ID3D12DeviceChild* child)
            {
                Microsoft::WRL::ComPtr<ID3D12Device> result;
                throw_if_failed(child->GetDevice(IID_PPV_ARGS(&result)));
                return result;
            }
        }
    }
}
