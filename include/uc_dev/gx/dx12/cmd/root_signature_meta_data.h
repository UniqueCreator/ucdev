#pragma once

#include <cstdint>
#include <d3d12.h>

#include <uc_dev/gx/dx12/gpu/descriptor_heap.h>
#include <uc_dev/gx/dx12/api/helpers.h>

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            namespace default_root_singature
            {
                enum slots : uint32_t
                {
                    constant_buffer_0 = 0,
                    constant_buffer_1 = 3,
                    srv_0 = 1,
                    srv_1 = 5,

                    uav_0 = 2,
                    uav_1 = 4,
                    uav_2 = 8
                };
            };

            //represents a descriptor range wi
            struct descriptor_usage
            {
                uint32_t                     m_used_handles     = 0U;   //which handles are used in the range, up to 32

                bool empty() const
                {
                    return m_used_handles == 0;
                }

                uint32_t size() const
                {
                    unsigned long size;
                    _BitScanReverse(&size, m_used_handles);
                    return size + 1;
                }

                void reset()
                {
                    m_used_handles = 0;
                }

                void set_used(uint32_t index)
                {
                    m_used_handles |= (1 << index);
                }
            };

            struct descriptor_range
            {
                uint16_t m_r = 0;

                descriptor_range() : m_r(0) {}

                descriptor_range(uint16_t r) : m_r(r) {}

                operator uint16_t() const
                {
                    return m_r;
                }
            };

            //contains simple extract from the root signature
            //contains which parts are descriptor tables and which of them are set by the user
            template <uint32_t root_constants, uint32_t cached_descriptors>
            struct cached_root_signature_meta_data
            {
                static const uint32_t                root_indices_constants = root_constants;
                cpu_descriptor_handle                m_cached_descriptors[cached_descriptors];

                descriptor_usage                     m_usages[root_indices_constants];
                descriptor_range                     m_descriptor_ranges[root_indices_constants];   //holds the sum of all previous value
                uint32_t                             m_is_table = 0;                                //which constants are descriptor tables

                cached_root_signature_meta_data() = default;


                D3D12_CPU_DESCRIPTOR_HANDLE* begin( uint32_t index )
                {
                    return &m_cached_descriptors[ m_descriptor_ranges[index] ];
                }

                const D3D12_CPU_DESCRIPTOR_HANDLE* begin(uint32_t index) const
                {
                    return &m_cached_descriptors[m_descriptor_ranges[index]];
                }

                uint32_t    size( uint32_t index ) const
                {
                    return m_descriptor_ranges[ (index + 1) & ( root_indices_constants - 1 ) ] - m_descriptor_ranges[index];
                }

                const descriptor_usage* usage(uint32_t index) const
                {
                    return &m_usages[index];
                }

                descriptor_usage* usage(uint32_t index)
                {
                    return &m_usages[index];
                }

                uint32_t size() const
                {
                    return m_descriptor_ranges[0];
                }
            };



            struct srv_signature_meta_data : public cached_root_signature_meta_data< 32, 256>
            {

            };

            struct sampler_signature_meta_data : public cached_root_signature_meta_data< 32, 16 >
            {

            };

            struct root_signature_meta_data : public cached_root_signature_meta_data< 32, 256>
            {
                srv_signature_meta_data     m_srv;
                sampler_signature_meta_data m_sampler;
            };

            inline root_signature_meta_data parse_root_signature( const void* byte_code, size_t byte_code_size, D3D12_CPU_DESCRIPTOR_HANDLE null_cbv, D3D12_CPU_DESCRIPTOR_HANDLE null_srv, D3D12_CPU_DESCRIPTOR_HANDLE null_uav, D3D12_CPU_DESCRIPTOR_HANDLE null_sampler)
            {
                auto s      = create_root_signature_deserializer(byte_code, byte_code_size);
                auto desc   = s->GetRootSignatureDesc();

                root_signature_meta_data meta;

                for (auto i = 0U; i < desc->NumParameters; ++i)
                {
                    auto& param = desc->pParameters[i];

                    if ( param.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE )
                    {
                        if (param.DescriptorTable.pDescriptorRanges->RangeType != D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                        {
                            meta.m_is_table |= (1 << i);

                            uint32_t ranges = 0;

                            for (auto j = 0U; j < param.DescriptorTable.NumDescriptorRanges; ++j)
                            {
                                ranges += param.DescriptorTable.pDescriptorRanges[j].NumDescriptors;
                            }

                            meta.m_descriptor_ranges[i] = static_cast<uint16_t>(ranges);
                        }
                    }
                }

                uint16_t sum = 0;
                for (auto i = 0; i < root_signature_meta_data::root_indices_constants + 1; ++i )
                {
                    auto index = i & (root_signature_meta_data::root_indices_constants - 1);
                    uint16_t t = meta.m_descriptor_ranges[index];
                    meta.m_descriptor_ranges[index] = sum;
                    sum = sum + t;
                }

                for (auto i = 0U; i < desc->NumParameters; ++i)
                {
                    auto& param = desc->pParameters[i];

                    if (param.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
                    {
                        if (param.DescriptorTable.pDescriptorRanges->RangeType != D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                        {
                            uint32_t index = 0;

                            for (auto j = 0U; j < param.DescriptorTable.NumDescriptorRanges; ++j)
                            {
                                auto type = param.DescriptorTable.pDescriptorRanges[j].RangeType;

                                D3D12_CPU_DESCRIPTOR_HANDLE init_handle;
                                init_handle.ptr = 0;

                                switch (type)
                                {
                                    case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
                                        init_handle = null_srv;
                                        break;
                                    case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
                                        init_handle = null_uav;
                                        break;
                                    case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
                                        init_handle = null_cbv;
                                        break;
                                    case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
                                        init_handle = null_sampler;
                                        break;
                                }

                                for (auto k = 0U; k < param.DescriptorTable.pDescriptorRanges[j].NumDescriptors; ++k)
                                {

                                    D3D12_CPU_DESCRIPTOR_HANDLE* h = meta.begin(i);

                                    *(h + index) = init_handle;

                                    meta.m_usages[i].set_used(index);

                                    ++index;
                                }
                            }
                        }
                    }
                }

                return meta;
            }


        }
    }
}