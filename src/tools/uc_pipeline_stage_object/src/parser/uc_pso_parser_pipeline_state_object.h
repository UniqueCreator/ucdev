#pragma once
#include <parser/uc_pso_parser_includes.h>
#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_index_buffer_strip_cut_value.h"
#include "uc_pso_parser_primitive_topology_type.h"
#include "uc_pso_parser_dxgi_format.h"
#include "uc_pso_parser_dxgi_sample_desc.h"


namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator, typename ast0>
            struct optional_string : public qi::grammar< iterator, ast0, ascii::space_type >
            {
                using base = qi::grammar< iterator, ast0, ascii::space_type  >;

                optional_string(const std::string& named_value) : base(m_value, named_value)
                {
                    using qi::lexeme;
                    using qi::char_;
                    using qi::lit;
                    using qi::attr;

                    namespace px = ::boost::phoenix;

                    m_identifier = raw[lexeme[(alpha | '_') > *(alnum | '_')]];
                    m_value = ((lit(named_value) > '=' > m_identifier) | attr(ast::nil{}));
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                qi::rule<iterator, std::string(), ascii::space_type  >          m_identifier;
                qi::rule<iterator, ast0, ascii::space_type  > m_value;
            };

            template <typename iterator>
            struct optional_uint32 : public qi::grammar< iterator, ast::optional_uint32(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_uint32(), ascii::space_type  >;

                optional_uint32(const std::string& named_value) : base(m_value, named_value)
                {
                    //qi::uint_type uint_;
                    qi::hex_type  hex_;
                    using qi::lit;
                    using qi::attr;

                    namespace px = ::boost::phoenix;

                    m_value = ((lit(named_value) > '=' > ( uint_ |  hex_  ) ) | attr(ast::nil{}));
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                qi::rule<iterator, ast::optional_uint32(), ascii::space_type  >             m_value;
            };

            template <typename iterator>
            struct optional_strip_cut_value : public qi::grammar< iterator, ast::optional_strip_cut_value(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_strip_cut_value(), ascii::space_type  >;

                optional_strip_cut_value() : base(m_value)
                {
                    using qi::attr;
                    m_value = ( m_cut_value | attr(ast::nil{}));
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                index_buffer_strip_cut_value<iterator>                                      m_cut_value;
                qi::rule<iterator, ast::optional_strip_cut_value(), ascii::space_type  >    m_value;
            };

            template <typename iterator>
            struct optional_primitive_topology_type : public qi::grammar< iterator, ast::optional_primitive_topology_type(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_primitive_topology_type(), ascii::space_type  >;

                optional_primitive_topology_type() : base(m_value)
                {
                    using qi::attr;
                    m_value = (m_cut_value | attr(ast::nil{}));
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                primitive_topology_type<iterator>                                                   m_cut_value;
                qi::rule<iterator, ast::optional_primitive_topology_type(), ascii::space_type  >    m_value;
            };

            template <typename iterator>
            struct optional_dxgi_format : public qi::grammar< iterator, ast::optional_dxgi_format(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_dxgi_format(), ascii::space_type  >;

                optional_dxgi_format() : base(m_value)
                , m_cut_value(".DSVFormat")
                {
                    using qi::attr;
                    m_value = (m_cut_value | attr(ast::nil{}));
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                named_dxgi_format<iterator>                                             m_cut_value;
                qi::rule<iterator, ast::optional_dxgi_format(), ascii::space_type  >    m_value;
            };

            template <typename iterator>
            struct optional_shader : public optional_string<iterator, ast::optional_shader() >
            {
                using base = optional_string<iterator, ast::optional_shader() >;
                optional_shader(const std::string& named_value) : base(named_value)
                {

                }
            };

            template <typename iterator>
            struct vertex_shader : public optional_shader<iterator>
            {
                using base = optional_shader<iterator>;
                vertex_shader() : base(".VertexShader") {}
            };

            template <typename iterator>
            struct pixel_shader : public optional_shader<iterator>
            {
                using base = optional_shader<iterator>;
                pixel_shader() : base(".PixelShader") {}
            };

            template <typename iterator>
            struct domain_shader : public optional_shader<iterator>
            {
                using base = optional_shader<iterator>;
                domain_shader() : base(".DomainShader") {}
            };

            template <typename iterator>
            struct hull_shader : public optional_shader<iterator>
            {
                using base = optional_shader<iterator>;
                hull_shader() : base(".HullShader") {}
            };

            template <typename iterator>
            struct geometry_shader : public optional_shader<iterator>
            {
                using base = optional_shader<iterator>;
                geometry_shader() : base(".GeometryShader") {}
            };

            template <typename iterator>
            struct compute_shader : public optional_shader<iterator>
            {
                using base = optional_shader<iterator>;
                compute_shader() : base(".ComputeShader") {}
            };

            template <typename iterator>
            struct optional_stream_output : public optional_string<iterator, ast::optional_stream_output() >
            {
                using base = optional_string<iterator, ast::optional_stream_output() >;
                optional_stream_output() : base(".StreamOutput")
                {
                }
            };

            template <typename iterator>
            struct optional_blend_state : public optional_string<iterator, ast::optional_blend_state() >
            {
                using base = optional_string<iterator, ast::optional_stream_output() >;
                optional_blend_state() : base(".BlendState")
                {
                }
            };

            template <typename iterator>
            struct optional_rasterizer_desc : public optional_string<iterator, ast::optional_rasterizer_desc() >
            {
                using base = optional_string<iterator, ast::optional_rasterizer_desc() >;
                optional_rasterizer_desc() : base(".RasterizerState")
                {
                }
            };

            template <typename iterator>
            struct optional_depth_stencil_state : public optional_string<iterator, ast::optional_depth_stencil_state() >
            {
                using base = optional_string<iterator, ast::optional_stream_output() >;
                optional_depth_stencil_state() : base(".DepthStencilState")
                {
                }
            };

            template <typename iterator>
            struct optional_input_layout: public optional_string<iterator, ast::optional_input_layout() >
            {
                using base = optional_string<iterator, ast::optional_stream_output() >;
                optional_input_layout() : base(".InputLayout")
                {
                }
            };

            template <typename iterator>
            struct optional_root_signature : public optional_string<iterator, ast::optional_root_signature() >
            {
                using base = optional_string<iterator, ast::optional_root_signature() >;
                optional_root_signature() : base(".RootSignature")
                {
                }
            };

            template <typename iterator>
            struct optional_sample_mask : public optional_uint32<iterator>
            {
                using base = optional_uint32<iterator>;
                optional_sample_mask() : base(".SampleMask")
                {
                }
            };

            template <typename iterator>
            struct optional_num_render_targets : public optional_uint32<iterator>
            {
                using base = optional_uint32<iterator>;
                optional_num_render_targets() : base(".NumRenderTargets")
                {

                }
            };

            template <typename iterator>
            struct optional_dxgi_sample_desc : public qi::grammar< iterator, ast::optional_dxgi_sample_desc(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_dxgi_sample_desc(), ascii::space_type  >;

                optional_dxgi_sample_desc() : base(m_value)
                {
                    using qi::attr;
                    m_value = (m_cut_value | attr(ast::nil{}));
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                dxgi_sample_desc<iterator>                                                   m_cut_value;
                qi::rule<iterator, ast::optional_dxgi_sample_desc(), ascii::space_type  >    m_value;
            };

            
            template <typename iterator>
            struct dxgi_vector : public qi::grammar< iterator, std::vector<DXGI_FORMAT>(), ascii::space_type >
            {
                using base = qi::grammar< iterator, std::vector<DXGI_FORMAT>(), ascii::space_type  >;

                dxgi_vector() : base(m_value)
                {
                    using qi::lit;
                    m_value = *(m_cut_value);
                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                dxgi_format2                                                           m_cut_value;
                qi::rule<iterator, std::vector<DXGI_FORMAT>(), ascii::space_type  >    m_value;
            };
            
            
            template <typename iterator>
            struct optional_dxgi_vector : public qi::grammar< iterator, ast::optional_dxgi_vector(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_dxgi_vector(), ascii::space_type  >;

                optional_dxgi_vector() : base(m_value)
                {
                    using qi::attr;
                    using qi::lit;

                    m_value = ((lit(".RTVFormats") > '=' > '{' > m_cut_value > '}') | attr(ast::nil{}));

                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                dxgi_vector<iterator>                                                   m_cut_value;
                qi::rule<iterator, ast::optional_dxgi_vector(), ascii::space_type  >    m_value;
            };

            template <typename iterator>
            struct optional_base : public qi::grammar< iterator, ast::optional_base(), ascii::space_type >
            {
                using base = qi::grammar< iterator, ast::optional_base(), ascii::space_type  >;

                optional_base() : base(m_value)
                {
                    using qi::attr;
                    using qi::lit;

                    m_identifier    = raw[lexeme[ (alpha | '_') > *(alnum | '_') ] ];
                    m_value         = ( ':' > m_identifier ) | attr(ast::nil{});

                    BOOST_SPIRIT_DEBUG_NODE(m_value);
                }

                qi::rule<iterator, std::string(), ascii::space_type  >          m_identifier;
                qi::rule<iterator, ast::optional_base(), ascii::space_type  >   m_value;
            };
            
            template <typename iterator, typename error_handler>
            struct graphics_pipeline_state_object : public qi::grammar< iterator, ast::graphics_pipeline_state_object(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::graphics_pipeline_state_object(), ascii::space_type >;

                graphics_pipeline_state_object(error_handler& e) : base(m_start, "pipeline_state_object")
                {
                    using qi::lit;
                    //qi::raw_type raw;
                    //qi::lexeme_type lexeme;
                    //qi::alpha_type alpha;
                    //qi::alnum_type alnum;
                    qi::attr_type attr;

                    m_identifier = raw[lexeme[(alpha  | '_') > *(alnum | '_')]];
                    m_start = lit("PipelineStateObject") > m_identifier > m_optional_base > '=' >
                    '{'
                    > (m_root_signature)
                    > (m_vertex_shader) 
                    > (m_pixel_shader)
                    > (m_domain_shader)
                    > (m_hull_shader)
                    > (m_geometry_shader)
                    > (m_stream_output)
                    > (m_blend_state)
                    > (m_sample_mask)
                    > (m_rasterizer_desc)
                    > (m_depth_stencil_state)
                    > (m_input_layout)
                    > (m_ib_strip_cut_value)
                    > (m_pritmitive_topology_type)
                    > (m_rtv_formats)
                    > (m_dsv_format)
                    > (m_sample_desc)
                    > '}';

                    BOOST_SPIRIT_DEBUG_NODE(m_start);
                    
                    m_identifier.name("identifier");
                    {

                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        qi::_1_type     _1;
                        //qi::_3_type     _3;
                        qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        //on_error<fail>(m_start, error_handler_function(e)(_3));
                        auto success = annotation_function(annotation<iterator>(e.m_iters, e.m_first));
                        on_success(m_start, success(_val, _1));
                    }
                }
                
                optional_root_signature<iterator>                                      m_root_signature;
                vertex_shader<iterator>                                                m_vertex_shader;
                pixel_shader<iterator>                                                 m_pixel_shader;
                domain_shader<iterator>                                                m_domain_shader;
                hull_shader<iterator>                                                  m_hull_shader;
                geometry_shader<iterator>                                              m_geometry_shader;
                optional_stream_output<iterator>                                       m_stream_output;

                optional_blend_state<iterator>                                         m_blend_state;
                optional_rasterizer_desc<iterator>                                     m_rasterizer_desc;
                optional_depth_stencil_state<iterator>                                 m_depth_stencil_state;
                optional_input_layout<iterator>                                        m_input_layout;

                optional_sample_mask<iterator>                                         m_sample_mask;

                optional_strip_cut_value<iterator>                                     m_ib_strip_cut_value;
                optional_primitive_topology_type<iterator>                             m_pritmitive_topology_type;

                optional_dxgi_vector<iterator>                                         m_rtv_formats;
                optional_dxgi_format<iterator>                                         m_dsv_format;
                optional_dxgi_sample_desc<iterator>                                    m_sample_desc;
                optional_base<iterator>                                                m_optional_base;
                qi::rule<iterator, std::string(), ascii::space_type >                  m_identifier;
                qi::rule<iterator, ast::graphics_pipeline_state_object(), ascii::space_type >   m_start;
            };


            template <typename iterator, typename error_handler>
            struct compute_pipeline_state_object : public qi::grammar< iterator, ast::compute_pipeline_state_object(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::compute_pipeline_state_object(), ascii::space_type >;

                compute_pipeline_state_object(error_handler& e) : base(m_start, "pipeline_state_object")
                {
                    using qi::lit;
                    //qi::raw_type raw;
                    //qi::lexeme_type lexeme;
                    //qi::alpha_type alpha;
                    //qi::alnum_type alnum;
                    qi::attr_type attr;

                    m_identifier = raw[lexeme[(alpha | '_') > *(alnum | '_')]];
                    m_start = lit("PipelineStateObject") > m_identifier > m_optional_base > '=' >
                        '{'
                    > (m_root_signature)
                    > (m_compute_shader)
                    > '}';

                    BOOST_SPIRIT_DEBUG_NODE(m_start);

                    m_identifier.name("identifier");
                    {

                        using boost::phoenix::function;
                        using error_handler_function = function < error_handler >;
                        using annotation_function = function < annotation<iterator> >;

                        qi::_1_type     _1;
                        qi::_val_type   _val;

                        using qi::on_error;
                        using qi::on_success;
                        using qi::fail;

                        auto success = annotation_function(annotation<iterator>(e.m_iters, e.m_first));
                        on_success(m_start, success(_val, _1));
                    }
                }

                optional_root_signature<iterator>                                              m_root_signature;
                compute_shader<iterator>                                                       m_compute_shader;
                optional_base<iterator>                                                        m_optional_base;
                qi::rule<iterator, std::string(), ascii::space_type >                          m_identifier;
                qi::rule<iterator, ast::compute_pipeline_state_object(), ascii::space_type >   m_start;
            };
        }
    }
}