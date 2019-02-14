#include "pch.h"

#include <uc_dev/gx/dx12/gpu/allocators/buddy_allocator.h>
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
                struct buddy_allocation_info
                {
                    uint64_t m_size;
                    uint64_t m_offset;
                };

                inline uint64_t log2(uint32_t x) throw()
                {
                    unsigned long result = 0;

                    auto result_function = _BitScanReverse(&result, x);

                    if (result_function)
                    {
                        return static_cast<size_t>(result);
                    }
                    else
                    {
                        return 0U;
                    }
                }

                inline uint64_t next_pow_of_2(uint32_t v) throw()
                {
                    //zero does not work
                    v--;
                    v |= v >> 1;
                    v |= v >> 2;
                    v |= v >> 4;
                    v |= v >> 8;
                    v |= v >> 16;
                    v++;

                    return v;
                }

                inline uint64_t buddy(const uint64_t offset, const uint64_t size) throw()
                {
                    // x + 2^k if x mod 2^(k+1) == 0
                    // x - 2^k if x mod 2^(k+1) == 2^k
                    //if we are at the even buddy, this code returns the odd buddy and vice versa
                    //note the calculations are in bytes, that is why we need the page_size
                    return offset ^ size;
                }

                inline uint64_t page_count(uint64_t heap_size) throw()
                {
                    static const uint32_t page_size = 65536;
                    return (heap_size + page_size - 1 ) / page_size;
                }

                inline uint64_t buddy_max_order(uint64_t page_count) throw()
                {
                    return log2(static_cast<uint32_t>(page_count));
                }

                inline uint64_t order_to_page_count(uint64_t order ) throw()
                {
                    return ( (uint64_t)1ULL ) << order;
                }

                inline uint64_t align(uint64_t size, uint64_t alignment)
                {
                    return  (size + alignment - 1) & ~(alignment - 1);
                }

                inline uint32_t align(uint32_t size, uint32_t alignment)
                {
                    return  (size + alignment - 1) & ~(alignment - 1);
                }

                inline uint64_t mb(uint64_t size)
                {
                    return size * 1024ULL * 1024ULL;
                }

                // {3D06001C-6015-45A1-9FEB-46834E43D151}
                static const GUID guid_buddy_allocator = { 0x3d06001c, 0x6015, 0x45a1,{ 0x9f, 0xeb, 0x46, 0x83, 0x4e, 0x43, 0xd1, 0x51 } };
            }

            buddy_heap_allocator::buddy_heap_allocator()
            {

            }

            buddy_heap_allocator::buddy_heap_allocator( ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12Heap> heap, uint64_t size ) :
                m_device(device)
                , m_heap(heap)
                , m_heap_offset(0)
                , m_size(details::next_pow_of_2(static_cast<uint32_t>(size)))
            {
                assert( (m_size & (m_size - 1)) == 0);   //power of two heaps are required
                reset();
            }

            Microsoft::WRL::ComPtr<ID3D12Resource> buddy_heap_allocator::create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE *optimizedClearValue)
            {
                static const uint64_t	page_size = 65536ULL;
                auto  info              = m_device->GetResourceAllocationInfo(1, 1, desc);
                auto  alignment         = std::max<uint64_t>(info.Alignment, page_size );
                auto  aligned_size      = details::align(info.SizeInBytes, alignment);
                auto  size_to_allocate  = details::next_pow_of_2(static_cast<uint32_t>(aligned_size));
                auto  offset            = allocate_offset( size_to_allocate );

                Microsoft::WRL::ComPtr<ID3D12Resource>  resource;

                throw_if_failed(m_device->CreatePlacedResource(m_heap.Get(), offset, desc, initialState, optimizedClearValue, IID_PPV_ARGS(&resource)));

                details::buddy_allocation_info buddy_info = { size_to_allocate, offset };

                //store physical offset in the interface private data, so we can fetch it later, during deallocation
                throw_if_failed( resource->SetPrivateData(details::guid_buddy_allocator, sizeof(info), &buddy_info) );

                return resource;
            }

            Microsoft::WRL::ComPtr<ID3D12Resource> buddy_heap_allocator::create_placed_resource(const D3D12_RESOURCE_DESC *desc, D3D12_RESOURCE_STATES initialState)
            {
                return create_placed_resource(desc, initialState, nullptr);
            }

            void buddy_heap_allocator::free_placed_resource( const Microsoft::WRL::ComPtr<ID3D12Resource>& resource )
            {
                auto  desc  = resource->GetDesc();
                auto  info  = m_device->GetResourceAllocationInfo(1, 1, &desc);

                details::buddy_allocation_info buddy_info = { };

                uint32_t data_size = sizeof(info);
                throw_if_failed(resource->GetPrivateData(details::guid_buddy_allocator, &data_size, &buddy_info) );
                free_offset(buddy_info.m_offset, buddy_info.m_size);
            }

            void buddy_heap_allocator::reset()
            {
                auto pages = details::page_count(m_size);
                auto order = static_cast<size_t>(details::buddy_max_order(pages));

                m_buddies.resize(order + 1ULL);
                m_buddies[order].insert( 0 );
            }

            uint64_t buddy_heap_allocator::allocate_offset(uint64_t size)
            {
                static const uint32_t page_size = 65536;
                auto pages      = details::page_count( size );
                auto order		= static_cast<size_t> (details::buddy_max_order(pages));

                auto max_pages  = details::page_count(m_size);
                auto max_order	= static_cast<size_t>(details::buddy_max_order(max_pages));
                auto k			= static_cast<size_t>(0U);
                auto offset     = m_size;

                for (k = order; k < max_order + 1; ++k)
                {
                    auto&& buddy = m_buddies[k];

                    if (!buddy.empty())
                    {
                        offset = *buddy.begin();
                        break;
                    }
                }

                if ( offset != m_size )
                {
                    while ( k > order )
                    {
                        auto&& buddy = m_buddies[k];
                        auto   it    = buddy.begin();
                        offset       = *it;

                        buddy.erase(it);

                        --k;

                        auto   s     = details::order_to_page_count(k) * page_size;
                        
                        auto   left  = offset;
                        auto   right = offset + s;

                        //split in two
                        m_buddies[k].insert(left);
                        m_buddies[k].insert(right);
                    }

                    auto&& buddy = m_buddies[k];
                    auto   it    = buddy.begin();
                    offset       = *it;

                    buddy.erase(it);

                    return offset;
                }
                else
                {
                    raise_out_of_memory_exception();
                }
                return 0;
            }

            void buddy_heap_allocator::free_offset(uint64_t offset, uint64_t size)
            {
                static const uint32_t page_size = 65536;

                auto pages          = details::page_count(details::next_pow_of_2(static_cast<uint32_t>(size)));
                auto order          = details::buddy_max_order(pages);

                auto max_pages      = details::page_count(m_size);
                auto max_order      = details::buddy_max_order(max_pages);

                auto   k			= static_cast<size_t>(order);
                auto   buddy_offset = details::buddy( offset, size );
                auto   empty		= m_buddies[k].empty();
                auto&& buddy		= m_buddies[k].find(static_cast<size_t>(buddy_offset));
                auto   merged_offset = offset;

                while ( k != max_order && !empty && buddy != m_buddies[k].end()  )
                {
                    merged_offset = std::min(offset, *buddy);
                    m_buddies[k].erase(buddy);
                    m_buddies[k].erase(offset);

                    ++k;

                    m_buddies[k].insert( merged_offset );

                    size = size * 2;

                    buddy = m_buddies[k].find(details::buddy(merged_offset, size) );
                    empty = m_buddies[k].empty();
                }

                m_buddies[k].insert(merged_offset);
            }
        }
    }
}

