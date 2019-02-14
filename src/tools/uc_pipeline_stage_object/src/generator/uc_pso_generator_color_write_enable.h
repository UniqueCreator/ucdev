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
            struct color_write_enable : karma::grammar< iterator, D3D12_COLOR_WRITE_ENABLE() >
            {
                using base = karma::grammar< iterator, D3D12_COLOR_WRITE_ENABLE() >;

            public:
                color_write_enable() : base(m_start, "color_write_enable")
                {
                    m_value.add
                        (D3D12_COLOR_WRITE_ENABLE_RED, STRINGIFY_TYPE(D3D12_COLOR_WRITE_ENABLE_RED_X))
                        (D3D12_COLOR_WRITE_ENABLE_GREEN, STRINGIFY_TYPE(D3D12_COLOR_WRITE_ENABLE_GREEN_X))
                        (D3D12_COLOR_WRITE_ENABLE_BLUE, STRINGIFY_TYPE(D3D12_COLOR_WRITE_ENABLE_BLUE_X))
                        (D3D12_COLOR_WRITE_ENABLE_ALPHA, STRINGIFY_TYPE(D3D12_COLOR_WRITE_ENABLE_ALPHA_X))
                        (D3D12_COLOR_WRITE_ENABLE_ALL, STRINGIFY_TYPE(D3D12_COLOR_WRITE_ENABLE_ALL_X))
                        ;

                    m_start = m_value;
                }

                karma::symbols<D3D12_COLOR_WRITE_ENABLE, const char* >                      m_value;
                karma::rule<iterator, D3D12_COLOR_WRITE_ENABLE()>                           m_start;
            };

            template <typename iterator>
            struct color_write_enable_flag : karma::grammar<iterator, uint32_t() >
            {
                using base = karma::grammar< iterator, uint32_t() >;

                color_write_enable_flag() : base(m_start)
                {
                    karma::uint_type uint_;
                    m_start = uint_;
                }

                karma::rule<iterator, uint32_t()>   m_start;
            };
        }
    }
}


