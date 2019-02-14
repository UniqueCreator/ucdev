#pragma once

#include <d3d12.h>
#include <cstdint>

#include <wrl/client.h>
#include <vector>
#include <set>

#include <uc_dev/util/noncopyable.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class buddy_heap_allocator : util::noncopyable
            {

            public:

                buddy_heap_allocator();
                buddy_heap_allocator(ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12Heap> heap, uint64_t size);

                Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE *optimizedClearValue);
                Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState);

                void free_placed_resource( const Microsoft::WRL::ComPtr<ID3D12Resource>& resource );

            private:
                ID3D12Device*                                 m_device;
                Microsoft::WRL::ComPtr<ID3D12Heap>            m_heap;
                uint64_t                                      m_heap_offset;
                uint64_t                                      m_size;
                std::vector < std::set <uint64_t >  >         m_buddies;    //todo: move here to a double list

				uint64_t                                      allocate_offset(uint64_t size);
                void                                          free_offset(uint64_t offset, uint64_t size);

                void   reset();

            };

        }
    }
}

