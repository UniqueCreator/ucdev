#pragma once

#include <generator/uc_pso_generator_includes.h>
#include "pipeline_state_types.h"

namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct depth_write_mask : karma::grammar< iterator, D3D12_DEPTH_WRITE_MASK() >
            {
                using base = karma::grammar< iterator, D3D12_DEPTH_WRITE_MASK() >;

            public:

                depth_write_mask() : base(m_start, "depth_write_mask")
                {

                    m_value.add
                        (D3D12_DEPTH_WRITE_MASK_ZERO, STRINGIFY_TYPE(D3D12_DEPTH_WRITE_MASK_ZERO))
                        (D3D12_DEPTH_WRITE_MASK_ALL, STRINGIFY_TYPE(D3D12_DEPTH_WRITE_MASK_ALL))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_DEPTH_WRITE_MASK, const char* >    m_value;
                karma::rule<iterator, D3D12_DEPTH_WRITE_MASK()>         m_start;
            };
        }
    }
}


