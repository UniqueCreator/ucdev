#pragma once

#include <cstdint>
#include <d3d12.h>

#include <uc_dev/gx/dx12/gpu/descriptor_heap.h>
#include "root_signature_meta_data.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct descriptor_handle_cache
            {
                Microsoft::WRL::ComPtr<ID3D12Device> m_device;
                root_signature_meta_data             m_data;
                const root_signature_meta_data*      m_initial;
                uint32_t                             m_dirty_tables = 0;

                descriptor_handle_cache( ID3D12Device* d, const root_signature_meta_data& data) :
                    m_device(d)
                    , m_data(data)
                    , m_initial(&data)
                {
                    reset();
                    clear();
                }

                void clear()
                {
                    unsigned long s = m_data.m_is_table;
                    unsigned long root_index = 0;

                    while (_BitScanForward(&root_index, s))
                    {
                        s = s ^ (1 << root_index);

                        m_data.m_usages[root_index].reset();
                    }
                }

                void reset()
                {
                    mark_all_dirty();
                    clear();
                }

                void set_root_signature_meta_data(const root_signature_meta_data& data)
                {
                    if (&data != m_initial)
                    {
                        m_data = data;
                        m_initial = &data;
                        reset();
                    }
                    else
                    {
                        mark_all_dirty();
                    }
                }

                void set_descriptor_handles(uint32_t root_index, uint32_t offset, const D3D12_CPU_DESCRIPTOR_HANDLE h[], uint32_t count)
                {
                    assert(((1 << root_index) & m_data.m_is_table) != 0);// , "Root parameter is not a CBV_SRV_UAV descriptor table");
                    assert( offset + count <= m_data.size(root_index) );

                    D3D12_CPU_DESCRIPTOR_HANDLE* dest = m_data.begin(root_index) + offset;

                    for (UINT i = 0; i < count; ++i)
                        dest[i] = h[i];

                    m_data.m_usages[ root_index].m_used_handles |= ((1 << count) - 1) << offset;
                    set_dirty(root_index);
                }

                inline void set_dirty(uint32_t root_index)
                {
                    m_dirty_tables |= (1 << root_index);
                }

                inline void set_used_root_index(uint32_t root_index)
                {
                    m_data.m_is_table |= (1 << root_index);
                }

                void mark_all_dirty()
                {
                    unsigned long s = m_data.m_is_table;
                    unsigned long root_index = 0;

                    while (_BitScanForward(&root_index, s))
                    {
                        s = s ^ (1 << root_index);

                        if (!m_data.m_usages[root_index].empty())
                        {
                            set_dirty(root_index);
                        }
                    }
                }

                template <typename functor>
                void flush(gx::dx12::incrementable_descriptor_handle start, functor f)
                {
                    uint32_t dirty_param_count = 0;
                    uint32_t table_size[m_data.root_indices_constants];
                    uint32_t root_indices[m_data.root_indices_constants];

                    unsigned long s = m_dirty_tables;
                    unsigned long root_index = 0;

                    while (_BitScanForward(&root_index, s))
                    {
                        s = s ^ (1 << root_index);

                        root_indices[dirty_param_count] = root_index;
                        table_size[dirty_param_count]   = m_data.size(root_index);
                        ++dirty_param_count;
                    }

                    if (dirty_param_count > 0)
                    {
                        const uint32_t max_descriptors_per_copy = 64;

                        D3D12_CPU_DESCRIPTOR_HANDLE dest_range_start[1];
                        uint32_t                    dest_range_size[1];
                        D3D12_CPU_DESCRIPTOR_HANDLE src_range_start[max_descriptors_per_copy];
                        uint32_t                    src_range_size[max_descriptors_per_copy];

                        for (auto i = 0U; i < dirty_param_count; ++i)
                        {
                            root_index = root_indices[i];

                            f(root_index, start);

                            D3D12_CPU_DESCRIPTOR_HANDLE  destination = start;

                            D3D12_CPU_DESCRIPTOR_HANDLE* src        = m_data.begin(root_index);
                            uint32_t                     src_size   = m_data.size(root_index);

                            assert( src_size < max_descriptors_per_copy );

                            dest_range_start[0] = start;
                            dest_range_size[0]  = src_size;

                            for (auto j = 0U; j < src_size; ++j)
                            {
                                src_range_start[j] = src[j];
                                src_range_size[j] = 1;
                            }

                            m_device->CopyDescriptors(1, dest_range_start, dest_range_size, src_size, src_range_start, src_range_size, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                            start = start + src_size;
                        }

                        m_dirty_tables = 0;
                    }
                }

                uint32_t size() const
                {
                    uint32_t size = 0;
                    unsigned long s = m_dirty_tables;
                    unsigned long root_index = 0;

                    while (_BitScanForward(&root_index, s))
                    {
                        s = s ^ (1 << root_index);
                        size += m_data.size(root_index);
                    }

                    return size;
                }

                bool dirty() const
                {
                    return m_dirty_tables != 0;
                }

            };
        }
    }
}