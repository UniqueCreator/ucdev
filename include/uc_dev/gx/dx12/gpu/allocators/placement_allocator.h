#pragma once

#include <d3d12.h>
#include <cstdint>
#include <mutex>

#include <wrl/client.h>

#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            inline uint64_t align(uint64_t size, uint64_t alignment)
            {
                return  (size + alignment - 1) & ~(alignment - 1);
            }

            inline uint32_t align(uint32_t size, uint32_t alignment)
            {
                return  (size + alignment - 1) & ~(alignment - 1);
            }

            inline size_t mb(size_t size)
            {
                return size * 1024 * 1024;
            }

            struct locking_policy_null
            {
                void lock()
                {

                }

                void unlock()
                {

                }
            };

            struct locking_policy_mutex
            {
                void lock()
                {
                    m_lock.lock();
                }

                void unlock()
                {
                    m_lock.unlock();
                }

               std::mutex m_lock;
            };


            //placement allocator increases and offset until it is full, you can reset it at any time
            //suitable for static data + per frame data, that is cleared every frame

            template <class locking_poliy > 
            class placement_heap_allocator : public locking_poliy, public util::noncopyable
            {

                public:

                placement_heap_allocator() = default;
                placement_heap_allocator(const placement_heap_allocator&) = default;

                placement_heap_allocator(ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12Heap> heap, uint64_t size) :
                    m_device(device)
                    , m_heap(heap)
                    , m_heap_offset(0)
                    , m_size(size)
                {

                }

                Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE *optimizedClearValue)
                {
                    auto  info = m_device->GetResourceAllocationInfo(1, 1, desc);

                    std::lock_guard< placement_heap_allocator > guard(*this);
                    if (align(m_heap_offset, info.Alignment) + info.SizeInBytes <= m_size)
                    {
                        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
                        m_heap_offset = align(m_heap_offset, info.Alignment);
                        throw_if_failed(m_device->CreatePlacedResource(m_heap.Get(), m_heap_offset, desc, initialState, optimizedClearValue, IID_PPV_ARGS(&resource)));
                        m_heap_offset += info.SizeInBytes;
                        return resource;
                    }
                    else
                    {
                        raise_out_of_memory_exception();
                        return Microsoft::WRL::ComPtr<ID3D12Resource>();
                    }
                }

                Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState)
                {
                    return create_placed_resource(desc, initialState, nullptr);
                }

                void reset()
                {
                    std::lock_guard< placement_heap_allocator > guard(*this);
                    m_heap_offset = 0;
                }


            private:
                ID3D12Device*                      m_device = nullptr;
                Microsoft::WRL::ComPtr<ID3D12Heap> m_heap;
                uint64_t                           m_heap_offset = 0;
                uint64_t                           m_size = 0;
            };

        }
    }
}

