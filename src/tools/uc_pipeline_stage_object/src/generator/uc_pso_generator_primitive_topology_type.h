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
            struct primitive_topology_type : karma::grammar< iterator, D3D12_PRIMITIVE_TOPOLOGY_TYPE() >
            {
                using base = karma::grammar< iterator, D3D12_PRIMITIVE_TOPOLOGY_TYPE() >;

            public:
                primitive_topology_type() : base(m_start, "primitive_topology_type")
                {
                    m_value.add

                        (D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED, STRINGIFY_TYPE(D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED))
                        (D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, STRINGIFY_TYPE(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT))
                        (D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, STRINGIFY_TYPE(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE))
                        (D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, STRINGIFY_TYPE(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE))
                        (D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH, STRINGIFY_TYPE(D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_PRIMITIVE_TOPOLOGY_TYPE, const char* > m_value;
                karma::rule<iterator, D3D12_PRIMITIVE_TOPOLOGY_TYPE()>      m_start;
            };
        }
    }
}


