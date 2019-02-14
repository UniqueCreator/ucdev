#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "pipeline_state_types.h"
#include <list>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

namespace uc
{
    namespace pso
    {
        namespace ast
        {
            ///////////////////////////////////////////////////////////////////////////
            //  The AST
            ///////////////////////////////////////////////////////////////////////////
            struct tagged
            {
                size_t id = { 0 }; // Used to annotate the AST with the iterator position.
                            // This id is used as a key to a map<int, Iterator>
                            // (not really part of the AST.)
            };

            struct nil {};

            inline std::ostream& operator<<(std::ostream& os, nil) { return os << "(nil)"; }

            struct identifier : tagged
            {
                std::string m_name;
            };

            struct import : tagged
            {
                identifier m_location;
            };

            struct input_layout_element : tagged
            {
                std::string                  m_semantic_name;
                uint32_t                     m_semantic_index          = { 0 };
                DXGI_FORMAT                  m_format                  = { DXGI_FORMAT_UNKNOWN };
                uint32_t                     m_input_slot              = { 0 };   //todo: between 0 and 16
                uint32_t                     m_aligned_byte_offset     = { 0 };
                D3D12_INPUT_CLASSIFICATION   m_input_slot_class        = { D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA };
                uint32_t                     m_instance_data_step_rate = { 0 };
            };

            struct input_layout_proxy
            {
                std::vector< input_layout_element >        m_elements;
                uint32_t                                   m_element_count;
            };

            struct input_layout : tagged
            {
                std::string                         m_identifier;
                std::vector< input_layout_element > m_elements;

                operator input_layout_proxy() const
                {
                    input_layout_proxy p = { m_elements, static_cast<uint32_t>(m_elements.size()) };
                    return p;
                }
            };

            struct render_target_blend_desc : tagged
            {
                bool            m_blend_enable      = { false };
                bool            m_logic_op_enable   = { false };
                D3D12_BLEND     m_src_blend         = { D3D12_BLEND_ZERO };
                D3D12_BLEND     m_dest_blend        = { D3D12_BLEND_ZERO };
                D3D12_BLEND_OP  m_blend_op          = {D3D12_BLEND_OP_ADD};
                D3D12_BLEND     m_src_blend_alpha   = { D3D12_BLEND_ZERO };
                D3D12_BLEND     m_dest_blend_alpha  = { D3D12_BLEND_ZERO };
                D3D12_BLEND_OP  m_blend_op_alpha    = { D3D12_BLEND_OP_ADD };
                D3D12_LOGIC_OP  m_logic_op          = { D3D12_LOGIC_OP_CLEAR };
                uint32_t        m_render_target_write_mask = { 0 };

                operator D3D12_RENDER_TARGET_BLEND_DESC() const
                {
                    D3D12_RENDER_TARGET_BLEND_DESC r = {};

                    #if defined(UC_BACKEND_DEV)

                    r.BlendEnable = m_blend_enable ? TRUE : FALSE;
                    r.LogicOpEnable = m_logic_op_enable ? TRUE : FALSE;

                    r.SrcBlend              = m_src_blend;
                    r.DestBlend             = m_dest_blend;
                    r.BlendOp               = m_blend_op;
                    r.SrcBlendAlpha         = m_src_blend_alpha;
                    r.DestBlendAlpha        = m_dest_blend_alpha;
                    r.BlendOpAlpha          = m_blend_op_alpha;

                    r.LogicOp               = m_logic_op;
                    r.RenderTargetWriteMask = static_cast<UINT8>(m_render_target_write_mask);
                    #else

                    r.m_blendEnable             = m_blend_enable ? true : false;
                    r.m_logicOperationEnable    = m_logic_op_enable ? true : false;

                    r.m_sourceBlend = m_src_blend;
                    r.m_destinationBlend = m_dest_blend;
                    r.m_blendOperation = m_blend_op;
                    r.m_sourceBlendAlpha        = m_src_blend_alpha;
                    r.m_destinationBlendAlpha   = m_dest_blend_alpha;
                    r.m_blendOperationAlpha     = m_blend_op_alpha;

                    r.m_logicOperation          = m_logic_op;
                    r.m_renderTargetWriteMask   = static_cast<uint8_t>(m_render_target_write_mask);

                    #endif

                    return r;
                }
            };

            struct blend_desc : tagged
            {
                std::string                             m_identifier;
                bool m_alpha_to_coverage_enable = { false };
                bool m_independent_blend_enable = { false };
                
                std::vector< render_target_blend_desc > m_render_targets;


                operator D3D12_BLEND_DESC() const
                {
                    D3D12_BLEND_DESC r = {};

                    #if defined(UC_BACKEND_DEV)
                    r.AlphaToCoverageEnable = m_alpha_to_coverage_enable;
                    r.IndependentBlendEnable = m_independent_blend_enable;

                    { 
                        auto k = 0U;
                        for ( auto& i : m_render_targets )
                        {
                            if (k < 8)
                            {
                                r.RenderTarget[k++] = i;
                            }
                        }

                        if (k < 8)
                        {
                            for (auto i = k; i < 8; ++i)
                            {
                                r.RenderTarget[i] = render_target_blend_desc();
                            }
                        }
                    }
                    #else

                    r.m_alphaToCoverageEnable  = m_alpha_to_coverage_enable;
                    r.m_independentBlendEnable = m_independent_blend_enable;

                    {
                        auto k = 0U;
                        for (auto& i : m_render_targets)
                        {
                            if (k < 8)
                            {
                                r.m_renderTarget[k++] = i;
                            }
                        }

                        if (k < 8)
                        {
                            for (auto i = k; i < 8; ++i)
                            {
                                r.m_renderTarget[i] = render_target_blend_desc();
                            }
                        }
                    }

                    #endif

                    return r;
                }
            };

            struct rasterizer_desc : tagged
            {
                std::string     m_identifier;

                D3D12_FILL_MODE m_fill_mode = { D3D12_FILL_MODE_SOLID };
                D3D12_CULL_MODE m_cull_mode = { D3D12_CULL_MODE_BACK };

                bool            m_front_counter_clockwise = { false };
                int32_t         m_depth_bias = { 0 };
                float           m_depth_bias_clamp = { 0.0f };
                float           m_slope_scale_depth_bias = { 0.0f };
                bool            m_depth_clip_enable = { false };
                bool            m_multi_sample_enable = { false };
                bool            m_antialiased_line_enable = { false };
                uint32_t        m_forced_sample_count = { 1 };

                D3D12_CONSERVATIVE_RASTERIZATION_MODE m_conservative_raster = { D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF } ;

                operator D3D12_RASTERIZER_DESC() const
                {
                    D3D12_RASTERIZER_DESC r = {};
                    #if defined(UC_BACKEND_DEV)
                    r.FillMode = m_fill_mode;
                    r.CullMode = m_cull_mode;
                    r.FrontCounterClockwise = m_front_counter_clockwise ? TRUE : FALSE;
                    r.DepthBias = m_depth_bias;
                    r.DepthBiasClamp = m_depth_bias_clamp;
                    r.SlopeScaledDepthBias = m_slope_scale_depth_bias;
                    r.DepthClipEnable = m_depth_clip_enable;
                    r.MultisampleEnable = m_multi_sample_enable ? TRUE : FALSE;
                    r.AntialiasedLineEnable = m_antialiased_line_enable ? TRUE : FALSE;
                    r.ForcedSampleCount = m_forced_sample_count;
                    r.ConservativeRaster = m_conservative_raster;
                    #else
                    r.m_fillMode                = m_fill_mode;
                    r.m_cullMode                = m_cull_mode;
                    r.m_frontCounterClockwise   = m_front_counter_clockwise ? true : false;
                    r.m_depthBias               = m_depth_bias;
                    r.m_depthBiasClamp          = m_depth_bias_clamp;
                    r.m_slopeScaledDepthBias    = m_slope_scale_depth_bias;
                    r.m_depthClipEnable         = m_depth_clip_enable;
                    r.m_multisampleEnable       = m_multi_sample_enable ? true : false;
                    r.m_antialiasedLineEnable   = m_antialiased_line_enable ? true : false;
                    r.m_forcedSampleCount       = m_forced_sample_count;
                    r.m_conservativeRaster      = m_conservative_raster;
                    #endif

                    return r;
                }
            };

            struct so_declaration_entry : tagged
            {
                uint32_t    m_stream = { 0 };
                std::string m_semantic_name;
                uint32_t    m_semantic_index = { 0 };
                uint32_t    m_start_component = { 0 };
                uint32_t    m_component_count = { 0 };
                uint32_t    m_output_slot = { 0 };
            };

            struct so_desc_proxy
            {
                std::vector<so_declaration_entry> m_entries;
                uint32_t                          m_entries_count;// = { 0 };
                std::vector<uint32_t>             m_strides;
                uint32_t                          m_strides_count;// = { 0 };
                uint32_t                          m_rasterized_stream;// = { 0 };
            };

            struct so_desc : tagged
            {
                std::string                       m_identifier;
                std::vector<so_declaration_entry> m_entries;
                std::vector<uint32_t>             m_strides;
                uint32_t                          m_rasterized_stream = { 0 };

                operator so_desc_proxy() const
                {
                    so_desc_proxy r =   { 
                                        m_entries, static_cast<uint32_t>(m_entries.size()),
                                        m_strides, static_cast<uint32_t>(m_strides.size()),
                                        m_rasterized_stream
                                    };

                    return r;
                }
            };

            struct depth_stencil_desc : tagged
            {
                std::string m_identifier;
                bool m_depth_enable                             = { false };
                D3D12_DEPTH_WRITE_MASK     m_depth_write_mask   = { D3D12_DEPTH_WRITE_MASK_ZERO };
                D3D12_COMPARISON_FUNC      m_depth_func         = { D3D12_COMPARISON_FUNC_NEVER };
                bool                       m_stencil_enable     = false;
                uint32_t                   m_stencil_read_mask  = { 0 };
                uint32_t                   m_stencil_write_mask = { 0 };
                D3D12_DEPTH_STENCILOP_DESC m_front_face         = { D3D12_STENCIL_OP_ZERO, D3D12_STENCIL_OP_ZERO , D3D12_STENCIL_OP_ZERO , D3D12_COMPARISON_FUNC_NEVER };
                D3D12_DEPTH_STENCILOP_DESC m_back_face          = { D3D12_STENCIL_OP_ZERO, D3D12_STENCIL_OP_ZERO , D3D12_STENCIL_OP_ZERO , D3D12_COMPARISON_FUNC_NEVER };

                operator D3D12_DEPTH_STENCIL_DESC() const
                {
                    D3D12_DEPTH_STENCIL_DESC r = {};

                    #if defined(UC_BACKEND_DEV)
                    r.DepthEnable           = m_depth_enable ? TRUE : FALSE;
                    r.DepthWriteMask        = m_depth_write_mask;
                    r.DepthFunc             = m_depth_func;
                    r.StencilEnable         = m_stencil_enable ? TRUE : FALSE;
                    r.StencilReadMask       = static_cast<UINT8>(m_stencil_read_mask);
                    r.StencilWriteMask      = static_cast<UINT8>(m_stencil_write_mask);
                    r.FrontFace             = m_front_face;
                    r.BackFace              = m_back_face;
                    #else
                    r.m_depthEnable         = m_depth_enable ? true : false;
                    r.m_depthWriteMask      = m_depth_write_mask;
                    r.m_depthFunc           = m_depth_func;
                    r.m_stencilEnable       = m_stencil_enable ? true : false;
                    r.m_stencilReadMask     = static_cast<uint8_t>(m_stencil_read_mask);
                    r.m_stencilWriteMask    = static_cast<uint8_t>(m_stencil_write_mask);
                    r.m_frontFace           = m_front_face;
                    r.m_backFace            = m_back_face;
                    #endif
                    return r;
                }
            };

            using optional_string                   = boost::variant< nil, std::string >;
            using optional_shader                   = boost::variant< nil, std::string >;

            using optional_vertex_shader            = optional_shader;
            using optional_pixel_shader             = optional_shader;
            using optional_domain_shader            = optional_shader;
            using optional_hull_shader              = optional_shader;
            using optional_geometry_shader          = optional_shader;
            using optional_compute_shader           = optional_shader;

            using optional_base                     = boost::variant< nil, std::string >;
            using optional_pipeline                 = boost::variant< nil, std::string >;
            using optional_stream_output            = boost::variant< nil, std::string >;
            using optional_blend_state              = boost::variant< nil, std::string >;
            using optional_rasterizer_desc          = boost::variant< nil, std::string >;
            using optional_depth_stencil_state      = boost::variant< nil, std::string >;
            using optional_input_layout             = boost::variant< nil, std::string >;
            using optional_root_signature           = boost::variant< nil, std::string >;
            using optional_uint32                   = boost::variant< nil, uint32_t>;

            using optional_strip_cut_value          = boost::variant< nil, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>;
            using optional_primitive_topology_type  = boost::variant< nil, D3D12_PRIMITIVE_TOPOLOGY_TYPE>;
            using optional_dxgi_format              = boost::variant< nil, DXGI_FORMAT>;
            using optional_dxgi_sample_desc         = boost::variant< nil, DXGI_SAMPLE_DESC>;
            using optional_dxgi_vector              = boost::variant< nil, std::vector<DXGI_FORMAT> >;
            using optional_pipeline_state_flags     = boost::variant< nil, D3D12_PIPELINE_STATE_FLAGS >;
            
            struct graphics_pipeline_state_object : tagged
            {
                std::string                        m_identifier;
                optional_base                      m_base;
                optional_root_signature            m_root_signature;
                optional_vertex_shader             m_vs;
                optional_pixel_shader              m_ps;
                optional_domain_shader             m_ds;
                optional_hull_shader               m_hs;
                optional_geometry_shader           m_gs;

                optional_stream_output             m_stream_output;
                optional_blend_state               m_blend_state;
                optional_uint32                    m_sample_mask;
                optional_rasterizer_desc           m_rasterizer_desc;
                optional_depth_stencil_state       m_depth_stencil_state;
                optional_input_layout              m_input_layout;
                optional_strip_cut_value           m_strip_cut_value;
                optional_primitive_topology_type   m_primitive_topology_type;

                //optional_uint32                    m_num_render_targets;
                optional_dxgi_format               m_dsv_format;
                optional_dxgi_vector               m_rtv_formats;
                optional_dxgi_sample_desc          m_sample_desc;
            };

            struct compute_pipeline_state_object : tagged
            {
                std::string                        m_identifier;
                optional_base                      m_base;
                optional_root_signature            m_root_signature;
                optional_compute_shader            m_cs;
            };

            using graphics_definition = boost::variant <
                nil
                , graphics_pipeline_state_object
                , so_desc
                , depth_stencil_desc
                , rasterizer_desc
                , blend_desc
                , input_layout
            >;

            using compute_definition = boost::variant <
                nil
                , compute_pipeline_state_object
            >;
       
            struct graphics_compile_unit : tagged
            {
                std::list< import >                 m_imports;
                std::list< graphics_definition >    m_definitions;
            };

            struct compute_compile_unit : tagged
            {
                std::list< import >             m_imports;
                std::list< compute_definition > m_definitions;
            };
        }
    }
}

#if defined(UC_BACKEND_DEV)
inline std::ostream& operator<<(std::ostream& os, DXGI_SAMPLE_DESC d) { return os << ".Count = " << d.Count << ", Quality =" << d.Quality; }
#else
inline std::ostream& operator<<(std::ostream& os, DXGI_SAMPLE_DESC d) { return os << ".Count = " << d.m_count << ", Quality =" << d.m_quality; }
#endif


