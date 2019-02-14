#pragma once

#include <uc_pso_ast.h>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::input_layout_element, 
    m_semantic_name, 
    m_semantic_index, 
    m_format, 
    m_input_slot, 
    m_aligned_byte_offset, 
    m_input_slot_class,
    m_instance_data_step_rate
)

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::input_layout, 
    m_identifier, 
    m_elements
)

BOOST_FUSION_ADAPT_STRUCT
(
    uc::pso::ast::identifier,
    m_name
)

BOOST_FUSION_ADAPT_STRUCT
(   
    uc::pso::ast::import,
    m_location
)

BOOST_FUSION_ADAPT_STRUCT
(
    uc::pso::ast::render_target_blend_desc,
    m_blend_enable,
    m_logic_op_enable,
    m_src_blend,
    m_dest_blend,
    m_blend_op,
    m_src_blend_alpha,
    m_dest_blend_alpha,
    m_blend_op_alpha,
    m_logic_op,
    m_render_target_write_mask
)

BOOST_FUSION_ADAPT_STRUCT
(
    uc::pso::ast::blend_desc,
    m_identifier,
    m_alpha_to_coverage_enable,
    m_independent_blend_enable,
    m_render_targets
)

BOOST_FUSION_ADAPT_STRUCT
(
    uc::pso::ast::rasterizer_desc,
    m_identifier,
    m_fill_mode,
    m_cull_mode,
    m_front_counter_clockwise,
    m_depth_bias,
    m_depth_bias_clamp,
    m_slope_scale_depth_bias,
    m_depth_clip_enable,
    m_multi_sample_enable,
    m_antialiased_line_enable,
    m_forced_sample_count,
    m_conservative_raster
)

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::so_declaration_entry, 
    m_stream, 
    m_semantic_name, 
    m_semantic_index, 
    m_start_component, 
    m_component_count, 
    m_output_slot
)

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::so_desc, 
    m_identifier, 
    m_entries, 
    m_strides, 
    m_rasterized_stream
)


#if defined(UC_BACKEND_DEV)
BOOST_FUSION_ADAPT_STRUCT
(
    DXGI_SAMPLE_DESC,
    Count,
    Quality
)
#else
BOOST_FUSION_ADAPT_STRUCT
(
    DXGI_SAMPLE_DESC,
    m_count,
    m_quality
)
#endif

#if defined(UC_BACKEND_DEV)
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_DEPTH_STENCILOP_DESC,
    StencilFailOp,
    StencilDepthFailOp,
    StencilPassOp,
    StencilFunc
)
#else
BOOST_FUSION_ADAPT_STRUCT
(
    D3D12_DEPTH_STENCILOP_DESC,
    m_stencilFailOperation,
    m_stencilDepthFailOperation,
    m_stencilPassOperation,
    m_stencilFunction
)
#endif

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::depth_stencil_desc,
    m_identifier,
    m_depth_enable,
    m_depth_write_mask,
    m_depth_func,
    m_stencil_enable,
    m_stencil_read_mask,
    m_stencil_write_mask,
    m_front_face,
    m_back_face
)


BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::graphics_pipeline_state_object,
    m_identifier,
    m_base,
    m_root_signature,
    m_vs,
    m_ps,
    m_ds,
    m_hs,
    m_gs,
    m_stream_output,
    m_blend_state,
    m_sample_mask,
    m_rasterizer_desc,
    m_depth_stencil_state,
    m_input_layout,
    m_strip_cut_value,
    m_primitive_topology_type,
    m_rtv_formats,
    m_dsv_format,
    m_sample_desc
)

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::compute_pipeline_state_object,
    m_identifier,
    m_base,
    m_root_signature,
    m_cs
)


BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::graphics_compile_unit,
    m_imports,
    m_definitions
)

BOOST_FUSION_ADAPT_STRUCT
(
    ::uc::pso::ast::compute_compile_unit,
    m_imports,
    m_definitions
)


