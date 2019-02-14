#pragma once

#include <d3d12.h>
#include <cstdint>

#include <wrl/client.h>
#include <list>

#include <uc_dev/util/noncopyable.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            class coalesceable_heap_allocator : util::noncopyable
            {
                public:

                    coalesceable_heap_allocator(ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12Heap> heap, uint64_t size);

                    Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE *optimizedClearValue);

                    Microsoft::WRL::ComPtr<ID3D12Resource> create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState);

                    void free_placed_resource( const Microsoft::WRL::ComPtr<ID3D12Resource>& resource );

                private:

                    struct allocation_info
                    {
                        uint64_t	                              m_offset;
                        uint64_t                                  m_size;
                        bool                                      m_free;
                    };

                    std::list< allocation_info >                  m_infos;
                    using info_pointer = std::list<allocation_info>::iterator;

                    ID3D12Device*                                 m_device;
                    Microsoft::WRL::ComPtr<ID3D12Heap>            m_heap;
                    uint64_t                                      m_offset;
                    uint64_t                                      m_size;

                    void reset();

                    static info_pointer inc(info_pointer i);
                    static info_pointer dec(info_pointer i);
            };

        }
    }
}

