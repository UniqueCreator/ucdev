#pragma once

#include <uc_dev/gx/dx12/api/error.h>
#include <uc_dev/gx/dx12/api/helpers.h>

#include "virtual_resource.h"
#include "descriptor_heap.h"

namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct gpu_root_signature_blob 
            {
                template <uint32_t s > gpu_root_signature_blob (const uint8_t(&arg)[s])
                {
                    m_code      = &arg[0];
                    m_code_size = s;
                }

                gpu_root_signature_blob ()
                {
                    m_code = nullptr;
                    m_code_size = 0;
                }

                const void* code() const
                {
                    return this->m_code;
                }

                size_t      code_size() const
                {
                    return m_code_size;
                }

                const uint8_t*  m_code;
                size_t          m_code_size;
            };

            inline Microsoft::WRL::ComPtr<ID3D12RootSignature> create_root_signature(ID3D12Device* d, gpu_root_signature_blob blob)
            {
                return create_root_signature(d, 0, blob.code(), blob.code_size());
            }
        }
    }
}
