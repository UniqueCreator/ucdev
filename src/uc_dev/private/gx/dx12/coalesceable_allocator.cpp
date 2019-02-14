#include "pch.h"

#include <uc_dev/gx/dx12/gpu/allocators/coalesceable_allocator.h>
#include <uc_dev/gx/dx12/api/error.h>

#include <algorithm>
#include <assert.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
                namespace details
                {
                    // {3D06001C-6015-45A1-9FEB-46834E43D152}
                    static const GUID guid_coalesceable_heap_allocator = { 0x3d06001c, 0x6015, 0x45a1,{ 0x9f, 0xeb, 0x46, 0x83, 0x4e, 0x43, 0xd1, 0x52 } };

                    inline uint64_t align(uint64_t size, uint64_t alignment)
                    {
                        return  (size + alignment - 1) & ~(alignment - 1);
                    }

                    inline uint32_t align(uint32_t size, uint32_t alignment)
                    {
                        return  (size + alignment - 1) & ~(alignment - 1);
                    }

                }

                coalesceable_heap_allocator::coalesceable_heap_allocator( ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12Heap> heap, uint64_t size ) :
                    m_device(device)
                    , m_heap(heap)
                    , m_offset(0)
                    , m_size(size)
                {
                    reset();
                }

                coalesceable_heap_allocator::info_pointer coalesceable_heap_allocator::inc(info_pointer i)
                {
                    auto r = i++;
                    return i;
                }

                coalesceable_heap_allocator::info_pointer coalesceable_heap_allocator::dec(info_pointer i)
                {
                    auto r = i--;
                    return i;
                }

                Microsoft::WRL::ComPtr<ID3D12Resource> coalesceable_heap_allocator::create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE *optimizedClearValue)
                {
                    Microsoft::WRL::ComPtr<ID3D12Resource> resource;


                    auto  info          = m_device->GetResourceAllocationInfo(1, 1, desc);
                    auto  alignment     = std::max(info.Alignment, 4096ULL);
                    auto  aligned_size  = details::align(info.SizeInBytes, alignment);

                    allocation_info c;
                    info_pointer    i = m_infos.end();

                    //todo: speed this, this is o(n)

                    //search through the free list
                    for (auto&& j = m_infos.begin(); j!= m_infos.end(); ++j)
                    {
                        if ( j->m_size > aligned_size && j->m_free )
                        {
                            //split

                            c.m_offset  = j->m_offset + aligned_size;
                            c.m_size    = j->m_size - aligned_size;
                            c.m_free    = true;

                            j->m_size   = aligned_size;
                            j->m_free   = false;

                            i = j;
                            m_infos.insert(inc(j), c);


                            

                            break;
                        }
                    }

                    if (i == m_infos.end())
                    {
                        raise_out_of_memory_exception();
                    }

                    throw_if_failed(m_device->CreatePlacedResource(m_heap.Get(), i->m_offset, desc, initialState, optimizedClearValue, IID_PPV_ARGS(&resource)));

                    //store physical offset in the interface private data, so we can fetch it later, during deallocation
                    throw_if_failed(resource->SetPrivateData(details::guid_coalesceable_heap_allocator, sizeof(info_pointer), &i));

                    return resource;
                }

                Microsoft::WRL::ComPtr<ID3D12Resource> coalesceable_heap_allocator::create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState)
                {
                    return create_placed_resource(desc, initialState, nullptr);
                }

                void coalesceable_heap_allocator::free_placed_resource( const Microsoft::WRL::ComPtr<ID3D12Resource>& resource )
                {
                    auto  desc = resource->GetDesc();
                    auto  info = m_device->GetResourceAllocationInfo(1, 1, &desc);

                    info_pointer    i   = m_infos.end();
                    uint32_t data_size  = sizeof(i);

                    throw_if_failed(resource->GetPrivateData(details::guid_coalesceable_heap_allocator, &data_size, &i));

                    i->m_free = true;

                    if (i == m_infos.begin() )
                    {
                        auto j = inc(i);

                        if (j != m_infos.end())
                        {
                            if (j->m_free)
                            {
                                i->m_size = i->m_size + j->m_size;
                                m_infos.erase(j);
                            }
                        }
                    }
                    else if ( i == m_infos.end()-- )
                    {
                        auto j = dec(i);

                        if ( j != m_infos.begin()-- )
                        {
                            if (j->m_free)
                            {
                                i->m_size = i->m_size + j->m_size;
                                m_infos.erase(j);
                            }
                        }
                    }
                    else
                    {
                        auto j = inc(i);
                        auto k = dec(i);

                        if (k->m_free)
                        {
                            i->m_offset = k->m_offset;
                            i->m_size   = i->m_size + k->m_size;
                            m_infos.erase(k);
                        }

                        if (j->m_free)
                        {
                            i->m_size = i->m_size + j->m_size;
                            m_infos.erase(j);
                        }
                    }
                }

                void coalesceable_heap_allocator::reset()
                {
                    m_infos.clear();

                    allocation_info i;
                    i.m_offset = 0;
                    i.m_size = m_size;
                    i.m_free = true;

                    m_infos.push_front( std::move( i ) );
                }
        }
    }
}

