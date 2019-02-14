#pragma once

#include <generator/uc_pso_generator_dxgi_sample_desc.h>
                    


namespace uc
{
    namespace pso
    {
        namespace generator
        {
            template <typename iterator >
            struct optional_dxgi_sample_desc : karma::grammar< iterator, ast::optional_dxgi_sample_desc() >
            {
                using base = karma::grammar< iterator, ast::optional_dxgi_sample_desc() >;

            public:

                optional_dxgi_sample_desc() : base(m_value)
                {
                    m_value = ( m_cut_value | "{}");
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                dxgi_sample_desc<iterator>                                       m_cut_value;
                karma::rule<iterator, ast::optional_dxgi_sample_desc()  >        m_value;
            };
        }
    }
}


