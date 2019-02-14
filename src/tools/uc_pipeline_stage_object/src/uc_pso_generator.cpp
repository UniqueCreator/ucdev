#include "pch.h"

#include "uc_pso_generator.h"
#include "uc_pso_resource_access.h"

#include <generator/uc_pso_generator_blend_desc.h>
#include <generator/uc_pso_generator_rasterizer_desc.h>
#include <generator/uc_pso_generator_depth_stencil_desc.h>
#include <generator/uc_pso_generator_input_layout.h>
#include <generator/uc_pso_generator_so_desc.h>
#include <generator/uc_pso_generator_optional_index_buffer_strip_cut_value.h>
#include <generator/uc_pso_generator_optional_root_signature.h>
#include <generator/uc_pso_generator_optional_shader.h>
#include <generator/uc_pso_generator_optional_primitive_topology_type.h>
#include <generator/uc_pso_generator_optional.h>
#include <generator/uc_pso_generator_optional_dxgi_format.h>
#include <generator/uc_pso_generator_optional_dxgi_sample_desc.h>


#include <string>
#include <boost/bind.hpp>
#include <filesystem>

namespace uc
{
    namespace pso
    {
        const std::string pre = "\t\t\t\t\t\t\t";
        const std::string suf = "\r\n";
        const std::string tab = "\t";

        enum shader_type : uint32_t
        {
            vertex      = 0,
            pixel       = 1,
            domain      = 2,
            hull        = 3, 
            geometry    = 4,
            compute     = 5
        };

        

        namespace
        {
            struct to_string_visitor : boost::static_visitor<std::string>
            {
                std::string operator()(const std::string& r) const
                {
                    return r;
                }

                std::string operator()(const ast::nil& ) const
                {
                    return std::string();
                }
            };

            struct is_nil_visitor : boost::static_visitor<bool>
            {
                template <typename t>
                bool operator()(const t& ) const
                {
                    return false;
                }

                bool operator()(const ast::nil& ) const
                {
                    return true;
                }
            };
        }

        std::string to_string(const ast::optional_string& v)
        {
            return boost::apply_visitor(to_string_visitor(), v);
        }

        template <typename t>
        bool is_nil(const t& t)
        {
            return boost::apply_visitor(is_nil_visitor(), t);
        }

        template <typename r, typename t>
        r value(const t& t)
        {
            return boost::apply_visitor(value_visitor<r>(), t);
        }

        std::string graphics_pso_header()
        {
            #if defined ( UC_BACKEND_DEV )
            auto resource0 = get_resource(IDR_DX12_GPU_PSO_MYPSO_H);
            #else
            auto resource0 = get_resource(IDR_UNIQUE_CREATOR_PSO_MYPSO_H);
            #endif
            scoped_resource r(resource0);
            return std::string( r.c_str());
        }

        std::string graphics_pso_cpp()
        {
            #if defined ( UC_BACKEND_DEV )
            auto resource0 = get_resource(IDR_DX12_GPU_PSO_MYPSO_CPP);
            #else
            auto resource0 = get_resource(IDR_UNIQUE_CREATOR_PSO_MYPSO_CPP);
            #endif
            scoped_resource r(resource0);
            return std::string(r.c_str());
        }

        std::string compute_pso_header()
        {
            #if defined ( UC_BACKEND_DEV )
            auto resource0 = get_resource(IDR_DX12_GPU_COMPUTE_PSO_MYPSO_H);
            #else
            auto resource0 = get_resource(IDR_UNIQUE_CREATOR_COMPUTE_PSO_MYPSO_H);
            #endif
            scoped_resource r(resource0);
            return std::string(r.c_str());
        }

        std::string compute_pso_cpp()
        {
            #if defined ( UC_BACKEND_DEV )
            auto resource0 = get_resource(IDR_DX12_GPU_COMPUTE_PSO_MYPSO_CPP);
            #else
            auto resource0 = get_resource(IDR_UNIQUE_CREATOR_COMPUTE_PSO_MYPSO_CPP);
            #endif
            scoped_resource r(resource0);
            return std::string(r.c_str());
        }

        std::string blend_desc(const ast::blend_desc& b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::blend_desc<iterator_type>  g;
            
            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string rasterizer_desc(const ast::rasterizer_desc& b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::rasterizer_desc<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string input_layout(const boost::variant<ast::nil, ast::input_layout>& b)
        {
            struct visit_input_layout : public boost::static_visitor<std::string>
            {
                std::string operator() (const ast::nil) const
                {
                    return pre + "static const " + STRINGIFY_TYPE(D3D12_INPUT_LAYOUT_DESC) + " layout = {};" + suf;
                }

                std::string operator() (const ast::input_layout& b) const
                {
                    using iterator_type = std::back_insert_iterator<std::string>;
                    std::string result;
                    iterator_type sink(result);
                    generator::input_layout_element_vector<iterator_type>  g;

                    auto res = boost::spirit::karma::generate(sink, g, b.m_elements);

                    if (!res)
                    {
                        raise_error();
                    }

                    std::string elements = pre + "static const "+ STRINGIFY_TYPE(D3D12_INPUT_ELEMENT_DESC) +" layouts[] = " + result + ";" + suf;
                    std::string layout = pre + "static const " + STRINGIFY_TYPE(D3D12_INPUT_LAYOUT_DESC)+" layout = {&layouts[0], sizeof(layouts) / sizeof( layouts[0]) };" + suf;

                    return elements + layout;
                }
            };

            return boost::apply_visitor( visit_input_layout(), b );
        }

        std::string depth_stencil_desc(const ast::depth_stencil_desc& b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::depth_stencil_desc<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string ib_cut_value(const ast::optional_strip_cut_value b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_index_buffer_strip_cut_value<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string root_signature(const ast::optional_root_signature b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_root_signature<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

#if !defined(UC_BACKEND_DEV)
        namespace generator
        {
            template <>
            std::string optional_shader_type_name<ast::optional_vertex_shader, shader_type::vertex>()
            {
                return "VertexShaderByteCode()";
            }

            template <>
            std::string optional_shader_type_name<ast::optional_pixel_shader, shader_type::pixel>()
            {
                return "PixelShaderByteCode()";
            }

            template <>
            std::string optional_shader_type_name<ast::optional_hull_shader, shader_type::hull>()
            {
                return "HullShaderByteCode()";
            }

            template <>
            std::string optional_shader_type_name<ast::optional_domain_shader, shader_type::domain>()
            {
                return "DomainShaderByteCode()";
            }

            template <>
            std::string optional_shader_type_name<ast::optional_geometry_shader, shader_type::geometry>()
            {
                return "GeometryShaderByteCode()";
            }

            template <>
            std::string optional_shader_type_name<ast::optional_compute_shader, shader_type::compute>()
            {
                return "ComputeShaderByteCode()";
            }
        }
#endif

        template < typename optional_shader_type, uint32_t n >
        std::string shader(const optional_shader_type b )
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_shader<iterator_type, optional_shader_type, n >  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }
            return result;
        }

        std::string primitive_topology(const ast::optional_primitive_topology_type  b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_primitive_topology_type<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string sample_mask(const ast::optional_uint32 b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_uint32<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string so_desc_entries(const ast::so_desc& b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::so_declaration_entry_element_vector<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b.m_entries);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string so_desc_strides(const ast::so_desc& b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::so_desc_strides_vector<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b.m_strides);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string so_desc(const boost::variant<ast::nil, ast::so_desc>& b)
        {
            struct visit_so_desc : public boost::static_visitor<std::string>
            {
                std::string operator() (const ast::nil) const
                {
                    return pre + "static const " + STRINGIFY_TYPE(D3D12_STREAM_OUTPUT_DESC)+" so_desc = { };" + suf;
                }

                std::string operator() (const ast::so_desc& b) const
                {
                    using iterator_type = std::back_insert_iterator<std::string>;

                    std::string entries = so_desc_entries(b);
                    std::string strides = so_desc_strides(b);

                    std::string result;

                    if (!b.m_entries.empty())
                    {
                        std::string so_entries   = "static const D3D12_SO_DECLARATION_ENTRY  so_entries[]    = " + entries + ";";
                        std::string declaration0 = "static const D3D12_SO_DECLARATION_ENTRY* so_entries_ptr  = &so_entries[0];";
                        std::string declaration1 = "static const UINT                        so_entries_size = sizeof(so_entries)/ sizeof(so_entries[0]);";

                        result += pre + so_entries + suf;
                        result += pre + declaration0 + suf;
                        result += pre + declaration1 + suf;
                    }
                    else
                    {
                        std::string declaration0 = "static const D3D12_SO_DECLARATION_ENTRY* so_entries_ptr  = nullptr;";
                        std::string declaration1 = "static const UINT                        so_entries_size = 0U;";

                        result += pre + declaration0 + suf;
                        result += pre + declaration1 + suf;
                    }

                    if (!b.m_strides.empty())
                    {
                        std::string so_strides  =  "static const UINT                        so_strides[]    = " + strides + ";";
                        std::string declaration0 = "static const UINT*                       so_strides_ptr  = &so_strides[0];";
                        std::string declaration1 = "static const UINT                        so_strides_size = sizeof(so_strides) / sizeof(so_strides[0]);";

                        result += pre + so_strides   + suf;
                        result += pre + declaration0 + suf;
                        result += pre + declaration1 + suf;
                    }
                    else
                    {
                        std::string declaration0 = "static const UINT*                       so_strides_ptr  = nullptr;";
                        std::string declaration1 = "static const UINT                        so_strides_size = 0U;";
                        result += pre + declaration0 + suf;
                        result += pre + declaration1 + suf;
                    }

                    std::string declaration0 = "static const UINT                        so_stream = " + std::to_string(b.m_rasterized_stream) + ";";
                    std::string declaration1 = "static const D3D12_STREAM_OUTPUT_DESC    so_desc = { so_entries_ptr, so_entries_size ,  so_strides_ptr, so_strides_size , so_stream };";

                    result += pre + declaration0 + suf;
                    result += pre + declaration1 + suf;

                    return result;
                }
            };

            return boost::apply_visitor(visit_so_desc(), b );
        }

        std::string num_render_targets(const ast::optional_uint32 b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_uint32<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string dsv_format(const ast::optional_dxgi_format b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_dxgi_format<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string sample_desc(const ast::optional_dxgi_sample_desc b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_dxgi_sample_desc<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        std::string rtv_formats(const ast::optional_dxgi_vector& b)
        {
            using iterator_type = std::back_insert_iterator<std::string>;
            std::string result;
            iterator_type sink(result);
            generator::optional_dxgi_vector<iterator_type>  g;

            auto res = boost::spirit::karma::generate(sink, g, b);

            if (!res)
            {
                raise_error();
            }

            return result;
        }

        struct referenced_graphics_structures
        {
            std::map < std::string, ast::graphics_pipeline_state_object > m_pipelines;
            std::map < std::string, ast::so_desc >                        m_so_desc;
            std::map < std::string, ast::depth_stencil_desc >             m_depth_stencil_desc;
            std::map < std::string, ast::rasterizer_desc >                m_rasterizer_desc;
            std::map < std::string, ast::blend_desc >                     m_blend_desc;
            std::map < std::string, ast::input_layout >                   m_input_layout;
        };

        struct referenced_compute_structures
        {
            std::map < std::string, ast::compute_pipeline_state_object > m_pipelines;
        };

        class graphics_split_structures_visitor : public boost::static_visitor<uint32_t>
        {
            public:

            uint32_t operator()(const ast::graphics_pipeline_state_object& b, referenced_graphics_structures& r) const
            {
                r.m_pipelines.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::so_desc& b, referenced_graphics_structures& r) const
            {
                r.m_so_desc.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::depth_stencil_desc& b, referenced_graphics_structures& r) const
            {
                r.m_depth_stencil_desc.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::rasterizer_desc& b, referenced_graphics_structures& r) const
            {
                r.m_rasterizer_desc.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::blend_desc& b, referenced_graphics_structures& r) const
            {
                r.m_blend_desc.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::input_layout& b, referenced_graphics_structures& r) const
            {
                r.m_input_layout.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::nil& , referenced_graphics_structures& ) const
            {
                return 0;
            }
        };

        class compute_split_structures_visitor : public boost::static_visitor<uint32_t>
        {
        public:

            uint32_t operator()(const ast::compute_pipeline_state_object& b, referenced_compute_structures& r) const
            {
                r.m_pipelines.insert(std::make_pair(b.m_identifier, b));
                return 0;
            }

            uint32_t operator()(const ast::nil&, referenced_compute_structures& ) const
            {
                return 0;
            }
        };

        const ast::graphics_pipeline_state_object& graphics_pipeline_state_desc(const std::string& name, const referenced_graphics_structures& r)
        {
            auto it = r.m_pipelines.find(name);

            if (it == std::end(r.m_pipelines))
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::pipeline_state_object);
            }

            return it->second;
        }

        const ast::compute_pipeline_state_object& compute_pipeline_state_desc(const std::string& name, const referenced_compute_structures& r)
        {
            auto it = r.m_pipelines.find(name);

            if (it == std::end(r.m_pipelines))
            {
                raise_error<missing_input_exception>(missing_input_exception::missing_part::pipeline_state_object);
            }

            return it->second;
        }
        
        const ast::blend_desc& blend_desc( const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r )
        {
            auto blend_state = to_string(p.m_blend_state);
            auto it          = r.m_blend_desc.find(blend_state);

            if ( it == std::end(r.m_blend_desc) )
            {
                auto base = to_string(p.m_base);
                if ( base !=  "")
                {
                    return blend_desc(graphics_pipeline_state_desc(base, r), r);
                }
                else
                {
                    raise_error<missing_variable_input_exception>(missing_input_exception::missing_part::blend_description, blend_state);
                }
            }

            return it->second;
        }

        const ast::rasterizer_desc& rasterizer_desc(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto state = to_string(p.m_rasterizer_desc);
            auto it = r.m_rasterizer_desc.find(state);

            if (it == std::end(r.m_rasterizer_desc))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return rasterizer_desc(graphics_pipeline_state_desc(base, r), r);
                }
                else
                {
                    raise_error<missing_variable_input_exception>(missing_input_exception::missing_part::rasterizer_description, state);
                }
            }

            return it->second;
        }

        const boost::variant< ast::nil, ast::input_layout > input_layout(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto state = to_string(p.m_input_layout);
            auto it = r.m_input_layout.find(state);

            if (it == std::end(r.m_input_layout))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return input_layout(graphics_pipeline_state_desc(base, r), r);
                }
                else
                {
                    return ast::nil();
                }
            }

            return it->second;
        }

        const ast::depth_stencil_desc& depth_stencil_desc(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto state = to_string(p.m_depth_stencil_state);
            auto it = r.m_depth_stencil_desc.find(state);

            if (it == std::end(r.m_depth_stencil_desc))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return depth_stencil_desc(graphics_pipeline_state_desc(base, r), r);
                }
                else
                {
                    raise_error<missing_variable_input_exception>(missing_input_exception::missing_part::depth_stencil_description, state);
                    raise_error();
                }
            }

            return it->second;
        }

        const boost::variant<ast::nil, ast::so_desc> so_desc(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto state = to_string(p.m_stream_output);
            auto it    = r.m_so_desc.find(state);

            if (it == std::end(r.m_so_desc))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return so_desc(graphics_pipeline_state_desc(base, r), r);
                }
                else
                {
                    return ast::nil();
                }
            }

            return it->second;
        }

        template <typename functor>
        typename functor::return_type pipeline_value(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r, functor f)
        {
            typename functor::return_type value = f(p);

            if (is_nil(value))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return pipeline_value(graphics_pipeline_state_desc(base, r), r, f);
                }
                else
                {
                    raise_error();
                }
            }

            return value;
        }

        template <typename functor>
        typename functor::return_type pipeline_value(const ast::compute_pipeline_state_object& p, const referenced_compute_structures& r, functor f)
        {
            typename functor::return_type value = f(p);

            if (is_nil(value))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return pipeline_value(compute_pipeline_state_desc(base, r), r, f);
                }
                else
                {
                    raise_error();
                }
            }

            return value;
        }

        const ast::optional_strip_cut_value   ib_cut_value(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_strip_cut_value return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_strip_cut_value;
                }
            };

            return pipeline_value(p, r, local());
        }

        
        const ast::optional_primitive_topology_type   primitive_topology(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_primitive_topology_type return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_primitive_topology_type;
                }
            };

            return pipeline_value(p, r, local());
        }

        const ast::optional_uint32   sample_mask(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_uint32 return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_sample_mask;
                }
            };

            return pipeline_value(p, r, local());
        }


        const ast::optional_dxgi_vector   rtv_formats(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_dxgi_vector return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_rtv_formats;
                }
            };

            return pipeline_value(p, r, local());
        }

        const ast::optional_uint32   num_render_targets(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            struct local : boost::static_visitor< ast::optional_uint32>
            {
                auto operator() ( const ast::nil& ) const
                {
                    return ast::optional_uint32(0);
                }

                auto operator() (const std::vector<DXGI_FORMAT>& ast) const
                {
                    return ast::optional_uint32( static_cast<uint32_t>( ast.size() ) );
                }

            };

            return boost::apply_visitor(local(),  rtv_formats( p, r )  );

        }

        const ast::optional_dxgi_format   dsv_format(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_dxgi_format return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_dsv_format;
                }
            };

            return pipeline_value(p, r, local());
        }

        const ast::optional_dxgi_sample_desc   sample_desc(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_dxgi_sample_desc return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_sample_desc;
                }
            };

            return pipeline_value(p, r, local());
        }

        const ast::optional_root_signature   root_signature(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_root_signature return_type;

                auto operator()(const ast::graphics_pipeline_state_object& p) const
                {
                    return p.m_root_signature;
                }
            };

            return pipeline_value(p, r, local());
        }

        const ast::optional_root_signature   root_signature(const ast::compute_pipeline_state_object& p, const referenced_compute_structures& r)
        {
            //cannot deduce the return type from lambda somehow
            struct local
            {
                typedef const ast::optional_root_signature return_type;

                auto operator()(const ast::compute_pipeline_state_object& p) const
                {
                    return p.m_root_signature;
                }
            };

            return pipeline_value(p, r, local());
        }

        

        template <typename return_type, typename functor>
        return_type shader(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r, functor f)
        {
            auto value = f( p );

            if ( is_nil(value) )
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return shader<return_type, functor>(graphics_pipeline_state_desc(base, r), r, f);
                }
                else
                {
                    return return_type(ast::nil());
                }
            }

            return value;
        }

        template <typename return_type, typename functor>
        return_type shader(const ast::compute_pipeline_state_object& p, const referenced_compute_structures& r, functor f)
        {
            auto value = f(p);

            if (is_nil(value))
            {
                auto base = to_string(p.m_base);
                if (base != "")
                {
                    return shader<return_type, functor>(compute_pipeline_state_desc(base, r), r, f);
                }
                else
                {
                    return return_type(ast::nil());
                }
            }

            return value;
        }

        const ast::optional_vertex_shader   vertex_shader(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto lambda = 
            [](const auto& p) -> ast::optional_vertex_shader
            {
                return p.m_vs;
            };

            return shader< ast::optional_vertex_shader, decltype(lambda) > (p, r, lambda );
        }

        const ast::optional_pixel_shader   pixel_shader(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto lambda = [](const auto& p) -> ast::optional_pixel_shader
            {
                return p.m_ps;
            };
            return shader< ast::optional_pixel_shader, decltype(lambda) >(p, r, lambda);
        }

        const ast::optional_domain_shader   domain_shader(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto lambda = [](const auto& p) -> ast::optional_domain_shader
            {
                return p.m_ds;
            };

            return shader< ast::optional_domain_shader, decltype(lambda) >(p, r, lambda);
        }

        const ast::optional_geometry_shader   geometry_shader(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto lambda = [](const auto& p) -> ast::optional_geometry_shader
            {
                return p.m_gs;
            };

            return shader< ast::optional_geometry_shader, decltype(lambda) >(p, r, lambda);
        }

        const ast::optional_hull_shader   hull_shader(const ast::graphics_pipeline_state_object& p, const referenced_graphics_structures& r)
        {
            auto lambda = [](const auto& p) -> ast::optional_hull_shader
            {
                return p.m_hs;
            };

            return shader< ast::optional_hull_shader, decltype(lambda) >(p, r, lambda);
        }

        const ast::optional_compute_shader   compute_shader(const ast::compute_pipeline_state_object& p, const referenced_compute_structures& r)
        {
            auto lambda = [](const auto& p) -> ast::optional_compute_shader
            {
                return p.m_cs;
            };

            return shader< ast::optional_compute_shader, decltype(lambda) >(p, r, lambda);
        }

        std::string shader_value(const ast::optional_shader& shader)
        {
            struct shader_visitor : boost::static_visitor<std::string>
            {
                std::string operator() (const ast::nil& ) const
                {
                    return std::string();
                }

                std::string operator() (const std::string& s ) const
                {
                    return s;
                }
            };

            return boost::apply_visitor(shader_visitor(), shader);
        }

        referenced_graphics_structures referenced(const graphics_parsed_unit& p)
        {
            referenced_graphics_structures    referenced;

            for (auto&& d : p.m_ast.m_definitions)
            {
                boost::apply_visitor(boost::bind(graphics_split_structures_visitor(), _1, boost::ref(referenced)), d);
            }

            return referenced;
        }

        referenced_compute_structures referenced(const compute_parsed_unit& p)
        {
            referenced_compute_structures    referenced;

            for (auto&& d : p.m_ast.m_definitions)
            {
                boost::apply_visitor(boost::bind(compute_split_structures_visitor(), _1, boost::ref(referenced)), d);
            }

            return referenced;
        }

        std::string shader_include( const std::string& shader_name )
        {
            std::string shader_include_prefix = "#include <autogen/shaders/";

            std::stringstream s;

            s << shader_include_prefix << shader_name << ".h>";
            return s.str();
        }

        std::string root_signature_include(const std::string& signature_name)
        {
            return shader_include(signature_name);
        }

        template <typename iterator>
        std::vector< std::string> includes( iterator begin, iterator end )
        {
            std::vector< std::string > r;
            
            std::for_each( begin, end, [&r]( const auto& v )
            {
                if (!is_nil(v))
                {
                    r.push_back(shader_include(shader_value(v)));
                }
            });

            return r;
        }

        std::string to_string( const std::vector<std::string>& v )
        {
            std::string r;

            for ( auto&& i : v )
            {
                r += i;
                r.push_back('\r');
                r.push_back('\n');
            }

            return r;
        }

        static std::string replace_string(const std::string& base, const::std::string& search, const::std::string& replace)
        {
            if (search == replace)
            {
                return base;
            }
            else
            {
                const std::string s = search;
                std::string header = base;
                const std::string r = replace;

                auto pos = header.find(s);

                while (pos != std::string::npos)
                {
                    header = header.replace(pos, s.length(), replace);
                    pos = header.find(s);
                }

                return header;
            }
        }

        unit_data generate_graphics_pso_unit(const graphics_parsed_unit& p, const std::string& main_function, const std::string& header_name)
        {
            namespace fs            = std::experimental::filesystem;
            const auto r            = referenced(p);
            const auto& pipe        = graphics_pipeline_state_desc(main_function, r);

            const auto  blend       = blend_desc( blend_desc(pipe, r) );
            const auto raster       = rasterizer_desc(rasterizer_desc(pipe, r));
            const auto layout_desc  = input_layout(pipe, r);
            const auto layout       = input_layout(layout_desc);
            const auto depth        = depth_stencil_desc(depth_stencil_desc(pipe, r));
            const auto ibcut        = ib_cut_value(ib_cut_value(pipe, r));
            const auto rsig         = root_signature(pipe, r);

            const auto vs           = vertex_shader(pipe, r);
            const auto vs_text      = shader<ast::optional_vertex_shader, shader_type::vertex> (vs);

            const auto ps           = pixel_shader(pipe, r);
            const auto ps_text      = shader<ast::optional_pixel_shader, shader_type::pixel>(ps);

            const auto ds           = domain_shader(pipe, r);
            const auto ds_text      = shader<ast::optional_domain_shader, shader_type::domain>(ds);

            const auto hs           = hull_shader(pipe, r);
            const auto hs_text      = shader<ast::optional_hull_shader, shader_type::hull>(hs);

            const auto gs           = geometry_shader(pipe, r);
            const auto gs_text      = shader<ast::optional_geometry_shader, shader_type::geometry> (gs);

            const auto prim_type    = primitive_topology(primitive_topology(pipe, r));
            const auto smask        = sample_mask(sample_mask(pipe, r));
            const auto so           = so_desc(pipe, r);
            const auto sodesc       = so_desc(so);
            const auto rt_count     = num_render_targets(num_render_targets(pipe, r));
            const auto dsv          = dsv_format(dsv_format(pipe, r));
            const auto sdesc        = sample_desc(sample_desc(pipe, r));
            const auto rtv_format   = rtv_formats(rtv_formats(pipe, r));

            std::stringstream s;

            s << layout;
            s << sodesc;

#if defined( UC_BACKEND_DEV )
            s << pre << "static const D3D12_GRAPHICS_PIPELINE_STATE_DESC state = " << "\r\n";
#else
            s << pre << "static const GraphicsPipelineStateDescription state = " << "\r\n";
#endif

            s << pre << "{" << suf;
#if defined( UC_BACKEND_DEV )
            s << pre << tab << "/*root_signature*/"            << tab << tab << "nullptr"   << "," << suf;
#endif
            s << pre << tab << "/*vertex shader*/"             << tab << tab << vs_text     << "," << suf;
            s << pre << tab << "/*pixel shader*/"              << tab << tab << ps_text     << "," << suf;
            s << pre << tab << "/*domain shader*/"             << tab << tab << ds_text     << "," << suf;
            s << pre << tab << "/*hull shader*/"               << tab << tab << hs_text     << "," << suf;
            s << pre << tab << "/*geometry shader*/"           << tab << tab << gs_text     << "," << suf;
            s << pre << tab << "/*stream output*/"             << tab << tab << "so_desc"   << "," << suf;
            s << pre << tab << "/*blend state*/"               << tab << tab << blend       << "," << suf;
            s << pre << tab << "/*sample mask*/"               << tab << tab << smask       << "," << suf;
            s << pre << tab << "/*rasterizer state*/"          << tab << tab << raster      << "," << suf;
            s << pre << tab << "/*depth stencil state*/"       << tab << tab << depth       << "," << suf;
            s << pre << tab << "/*input layout*/"              << tab << tab << "layout"    << "," << suf;
            s << pre << tab << "/*input buffer strip cut value*/" << tab << tab << ibcut    << "," << suf;
            s << pre << tab << "/*primitive topology type*/"   << tab << tab << prim_type   << "," << suf;
            s << pre << tab << "/*num_render_targets*/"        << tab << tab << rt_count    << "," << suf;
            s << pre << tab << "/*rtv formats*/"               << tab << tab << rtv_format  << "," << suf;
            s << pre << tab << "/*depth stencil view format*/" << tab << tab << dsv         << "," << suf;

            s << pre << tab << "/*sample desc*/"               << tab << tab << sdesc       << "," << suf;
#if defined( UC_BACKEND_DEV )
            s << pre << tab << "/*nodemask*/"                  << tab << tab << '0'         << "," << suf;
            s << pre << tab << "/*cached pipeline state*/"     << tab << tab << "{nullptr,0}"  << "," << suf;
            s << pre << tab << "/*pipeline state flags*/"      << tab << tab << "D3D12_PIPELINE_STATE_FLAG_NONE" << "," << suf;
#endif
            s << pre << "};" << suf;

            //convert to string
            std::array< ast::optional_shader, 6 >  shaders = { vs, ps, ds, hs, gs, rsig };

            auto files  = includes(std::begin(shaders), std::end(shaders));
            unit_data u = { files, s.str() };

            auto header   = replace_string(graphics_pso_header(), "mypso", main_function);
            auto cpp      = replace_string(graphics_pso_cpp(), "dx12_gpu_pso_mypso.h", fs::path(header_name).filename().string());


            auto rsig_text = shader_value(rsig);

            auto gen_cpp0 = replace_string(std::move(cpp), "mypso_state", u.m_pipeline_state);
            auto gen_cpp1 = replace_string(std::move(gen_cpp0), "mypso_signature_constructor", rsig_text);
            auto gen_cpp2 = replace_string(std::move(gen_cpp1), "mypso_signature", rsig_text);
            
            auto gen_cpp3 = replace_string(std::move(gen_cpp2), "mypso_includes", to_string(files));
            auto gen_cpp4 = replace_string(std::move(gen_cpp3), "mypso", main_function);

            u.m_header    = std::move(header);
            u.m_cpp       = std::move(gen_cpp4);

            return u;
        }

        unit_data generate_compute_pso_unit(const compute_parsed_unit& p, const std::string& main_function, const std::string& header_name)
        {
            namespace fs = std::experimental::filesystem;
            const auto r = referenced(p);
            const auto& pipe = compute_pipeline_state_desc(main_function, r);

            const auto rsig = root_signature(pipe, r);

            const auto cs = compute_shader(pipe, r);
            const auto cs_text = shader<ast::optional_shader, 5>(cs);

            std::stringstream s;

            #if defined( UC_BACKEND_DEV )
            s << pre << "static const D3D12_COMPUTE_PIPELINE_STATE_DESC state = " << "\r\n";
            #else
            s << pre << "static const ComputePipelineStateDescription state = " << "\r\n";
            #endif

            s << pre << "{" << suf;
            s << pre << tab << "/*root_signature*/" << tab << tab << "nullptr" << "," << suf;
            s << pre << tab << "/*compute shader*/" << tab << tab << cs_text << "," << suf;

            #if defined( UC_BACKEND_DEV )
            s << pre << tab << "/*nodemask*/" << tab << tab << '0' << "," << suf;
            s << pre << tab << "/*cached pipeline state*/" << tab << tab << "{nullptr,0}" << "," << suf;
            s << pre << tab << "/*pipeline state flags*/" << tab << tab << "D3D12_PIPELINE_STATE_FLAG_NONE" << "," << suf;
            #endif

            s << pre << "};" << suf;

            //convert to string
            std::array< ast::optional_shader, 2 >  shaders = { cs, rsig };

            auto files = includes(std::begin(shaders), std::end(shaders));
            unit_data u = { files, s.str() };

            auto header     = replace_string(compute_pso_header(), "mypso", main_function);
            auto cpp        = replace_string(compute_pso_cpp(), "dx12_gpu_pso_mypso.h", fs::path(header_name).filename().string() );

            auto rsig_text  = shader_value(rsig);

            auto gen_cpp0 = replace_string(std::move(cpp), "mypso_state", u.m_pipeline_state);
            auto gen_cpp1 = replace_string(std::move(gen_cpp0), "mypso_signature_constructor", rsig_text);
            auto gen_cpp2 = replace_string(std::move(gen_cpp1), "mypso_signature", rsig_text);

            auto gen_cpp3 = replace_string(std::move(gen_cpp2), "mypso_includes", to_string(files));
            auto gen_cpp4 = replace_string(std::move(gen_cpp3), "mypso", main_function);

            u.m_header = std::move(header);
            u.m_cpp = std::move(gen_cpp4);

            return u;
        }
    }
}






