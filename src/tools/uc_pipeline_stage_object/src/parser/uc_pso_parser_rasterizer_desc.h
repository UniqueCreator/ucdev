#pragma once
#include <parser/uc_pso_parser_includes.h>
#include <uc_pso_ast.h>
#include "uc_pso_ast_adapted.h"
#include "uc_pso_parser_fill_mode.h"
#include "uc_pso_parser_cull_mode.h"
#include "uc_pso_parser_conservative_rasterization_mode.h"
#include "uc_pso_parser_named_value.h"

namespace uc
{
    namespace pso
    {
        namespace parser
        {
            template <typename iterator, typename error_handler>
            struct rasterizer_desc : public qi::grammar< iterator, ast::rasterizer_desc(), ascii::space_type  >
            {
                using base = qi::grammar< iterator, ast::rasterizer_desc(), ascii::space_type >;

                rasterizer_desc(error_handler& e) : base(m_start, "rasterizer_desc")
                , m_front_counter_clockwise(".FrontCounterClockwise")
                , m_depth_bias(".DepthBias")
                , m_depth_bias_clamp(".DepthBiasClamp")
                , m_slope_scale_depth_bias(".SlopeScaledDepthBias")
                , m_depth_clip_enable(".DepthClipEnable")
                , m_multi_sample_enable(".MultisampleEnable")
                , m_antialiased_line_enable(".AntialiasedLineEnable")
                , m_forced_sample_count(".ForcedSampleCount")
                {
                    using qi::lit;
                    //qi::raw_type raw;
                    //qi::lexeme_type lexeme;
                    //qi::alpha_type alpha;
                    //qi::alnum_type alnum;

                    m_identifier = raw[lexeme[(alpha | '_') > *(alnum | '_')]];
                
                    m_start = lit("RasterizerDescription")
                    > m_identifier
                    > '=' > '{'
                    > m_fill_mode
                    > m_cull_mode
                    > m_front_counter_clockwise
                    > m_depth_bias
                    > m_depth_bias_clamp
                    > m_slope_scale_depth_bias
                    > m_depth_clip_enable
                    > m_multi_sample_enable
                    > m_antialiased_line_enable
                    > m_forced_sample_count
                    > m_conservative_rasterization_mode
                    > '}';

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

                fill_mode<iterator>                                                            m_fill_mode;
                cull_mode<iterator>                                                            m_cull_mode;
                named_value_bool<iterator>                                                     m_front_counter_clockwise;
                named_value_int32<iterator>                                                    m_depth_bias;
                named_value_float<iterator>                                                    m_depth_bias_clamp;
                named_value_float<iterator>                                                    m_slope_scale_depth_bias;
                named_value_bool<iterator>                                                     m_depth_clip_enable;
                named_value_bool<iterator>                                                     m_multi_sample_enable;
                named_value_bool<iterator>                                                     m_antialiased_line_enable;
                named_value_uint32<iterator>                                                   m_forced_sample_count;
                consertvative_rasterization_mode<iterator>                                     m_conservative_rasterization_mode;

                qi::rule<iterator, std::string(), ascii::space_type >        m_identifier;
                qi::rule<iterator, ast::rasterizer_desc(), ascii::space_type >  m_start;
            };
        }
    }
}