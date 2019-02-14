// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <algorithm>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <iostream>
#include <set>
#include <type_traits>

#include <boost/program_options.hpp>

#include <uc_dev/fnd/string_hash.h>
#include <uc_dev/lip/lip.h>
#include <uc_dev/gx/lip/geo.h>
#include <uc_dev/lip/tools_time_utils.h>
#include <uc_dev/gx/img/img.h>
#include <uc_dev/os/windows/com_initializer.h>
#include <uc_dev/lzham/lzham_compressor.h>

#include <uc_dev/gx/import/assimp/indexed_mesh.h>
#include <uc_dev/gx/import/fbx/indexed_mesh.h>

#include "uc_model_command_line.h"

#include "uc_model_compressonator.h"
#include "uc_model_swizzle.h"
#include "uc_model_texture.h"
#include "uc_model_geometry.h"
#include "uc_model_multi_textured_mesh.h"
#include "uc_model_skeleton.h"

#include <uc_dev/gx/import/assimp/indexed_mesh.h>
#include <uc_dev/gx/import/fbx/indexed_mesh.h>

#include <uc_dev/gx/import/geo/multi_material_mesh.h>
#include <uc_dev/gx/import/geo/merge_attributes_indexed_mesh.h>
#include <uc_dev/gx/import/fbx/multi_material_mesh.h>
#include <uc_dev/gx/import/assimp/multi_material_mesh.h>

#include <uc_dev/gx/import/geo/skinned_mesh.h>
#include <uc_dev/gx/import/geo/merge_attributes_skinned_mesh.h>
#include <uc_dev/gx/import/assimp/skinned_mesh.h>
#include <uc_dev/gx/import/fbx/skinned_mesh.h>



#include <uc_dev/mem/alloc.h>

#include <uc_dev/gx/import/assimp/assimp_options.h>

namespace uc
{
    namespace model
    {
        using assimp_flags_t  = uint32_t;
        using file_name_t     = std::string;

        template <typename t> struct has_positions_trait { static const bool value = false; };
        template <typename t> struct has_indices_trait { static const bool value = false; };
        template <typename t> struct has_uvs_trait { static const bool value = false; };
        template <typename t> struct has_normals_trait { static const bool value = false; };
        template <typename t> struct has_tangents_trait { static const bool value = false; };


        template <typename source, typename destination> struct copy_attributes_struct
        {
            private:
            //////////////////////////////////////////////////////////////////////////////////////////////
            template <bool> static void do_copy_tangents(destination* d, const source* s);

            template <> static void do_copy_tangents<false>(destination* , const source* ) {}

            template <> static void do_copy_tangents<true>(destination* d, const source* s)
            {
                d->m_tangents.m_data.resize(s->m_tangents.size());
                copy_tangents(s->m_tangents, d->m_tangents.m_data);
            }

            //////////////////////////////////////////////////////////////////////////////////////////////
            template <bool> static void do_copy_normals(destination* d, const source* s);

            template <> static void do_copy_normals<false>(destination*, const source* ) {}

            template <> static void do_copy_normals<true>(destination* d, const source* s)
            {
                d->m_normals.m_data.resize(s->m_normals.size());
                copy_normals(s->m_normals, d->m_normals.m_data);
            }
            //////////////////////////////////////////////////////////////////////////////////////////////
            template <bool> static void do_copy_uvs(destination* d, const source* s);

            template <> static void do_copy_uvs<false>(destination* , const source* ) {}

            template <> static void do_copy_uvs<true>(destination* d, const source* s)
            {
                d->m_uv.m_data.resize(s->m_uv.size());
                copy_uv(s->m_uv, d->m_uv.m_data);
            }
            //////////////////////////////////////////////////////////////////////////////////////////////
            template <bool> static void do_copy_positions(destination* d, const source* s);

            template <> static void do_copy_positions<false>(destination* , const source* ) {}

            template <> static void do_copy_positions<true>(destination* d, const source* s)
            {
                d->m_positions.m_data.resize(s->m_positions.size());
                copy_positions(s->m_positions, d->m_positions.m_data);
            }
            //////////////////////////////////////////////////////////////////////////////////////////////
            template <bool> static void do_copy_indices(destination* d, const source* s);

            template <> static void do_copy_indices<false>(destination* , const source* ) {}

            template <> static void do_copy_indices<true>(destination* d, const source* s)
            {
                d->m_indices.m_data.resize(s->m_faces.size() * 3);
                copy_indices(s->m_faces, d->m_indices.m_data);
            }

            public:

            static void do_copy_attributes(destination* d, const source* s)
            {
                do_copy_indices<has_indices_trait<destination>::value>(d, s);
                do_copy_positions<has_positions_trait<destination>::value>(d, s);
                do_copy_uvs<has_uvs_trait<destination>::value>(d, s);
                do_copy_normals<has_normals_trait<destination>::value>(d, s);
                do_copy_tangents<has_tangents_trait<destination>::value>(d, s);
            }
        };

        template <> struct has_positions_trait<lip::model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::model>   { static const bool value = true; };


        template <> struct has_positions_trait<lip::normal_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::normal_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::normal_model> { static const bool value = true; };

        template <> struct has_positions_trait<lip::parametrized_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::parametrized_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::parametrized_model> { static const bool value = true; };


        template <> struct has_positions_trait<lip::normal_parametrized_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::normal_parametrized_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::normal_parametrized_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::normal_parametrized_model> { static const bool value = true; };

        template <> struct has_positions_trait<lip::derivatives_parametrized_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::derivatives_parametrized_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::derivatives_parametrized_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::derivatives_parametrized_model> { static const bool value = true; };
        template <> struct has_tangents_trait<lip::derivatives_parametrized_model> { static const bool value = true; };

        template <> struct has_positions_trait<lip::textured_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::textured_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::textured_model> { static const bool value = true; };

        template <> struct has_positions_trait<lip::normal_textured_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::normal_textured_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::normal_textured_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::normal_textured_model> { static const bool value = true; };

        template <> struct has_positions_trait<lip::derivatives_textured_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::derivatives_textured_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::derivatives_textured_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::derivatives_textured_model> { static const bool value = true; };
        template <> struct has_tangents_trait<lip::derivatives_textured_model> { static const bool value = true; };


        template <> struct has_positions_trait<lip::multi_textured_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::multi_textured_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::multi_textured_model> { static const bool value = true; };

        template <> struct has_positions_trait<lip::normal_multi_textured_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::normal_multi_textured_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::normal_multi_textured_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::normal_multi_textured_model> { static const bool value = true; };


        template <> struct has_positions_trait<lip::derivatives_multi_textured_model> { static const bool value = true; };
        template <> struct has_indices_trait<lip::derivatives_multi_textured_model> { static const bool value = true; };
        template <> struct has_uvs_trait<lip::derivatives_multi_textured_model> { static const bool value = true; };
        template <> struct has_normals_trait<lip::derivatives_multi_textured_model> { static const bool value = true; };
        template <> struct has_tangents_trait<lip::derivatives_multi_textured_model> { static const bool value = true; };


        
        

        template <typename source, typename destination> void copy_attributes( destination* d, const source* s)
        {
            copy_attributes_struct<source, destination>::do_copy_attributes(d, s);
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::model> create_model( const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< lip::model > m = std::make_unique<lip::model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::normal_model> create_normal_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< lip::normal_model > m = std::make_unique<lip::normal_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::parametrized_model> create_parametrized_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< uc::lip::parametrized_model > m = std::make_unique<uc::lip::parametrized_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::normal_parametrized_model> create_normal_parametrized_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< uc::lip::normal_parametrized_model > m = std::make_unique<uc::lip::normal_parametrized_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::derivatives_parametrized_model> create_derivatives_parametrized_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< uc::lip::derivatives_parametrized_model > m = std::make_unique<uc::lip::derivatives_parametrized_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::textured_model> create_textured_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< uc::lip::textured_model > m = std::make_unique<uc::lip::textured_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::normal_textured_model> create_normal_textured_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< uc::lip::normal_textured_model > m = std::make_unique<uc::lip::normal_textured_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        template <typename mesh_create_functor> std::unique_ptr<lip::derivatives_textured_model> create_derivatives_textured_model(const file_name_t& input_file_name, const mesh_create_functor& create_mesh)
        {
            auto mesh = create_mesh(input_file_name);
            std::unique_ptr< uc::lip::derivatives_textured_model > m = std::make_unique<uc::lip::derivatives_textured_model>();

            copy_attributes(m.get(), mesh.get());

            return m;
        }

        static uc::lip::texture2d create_texture_2d(const file_name_t& input_file_name, const std::string& texture_format)
        {
            auto storage = string_to_storage_format(texture_format);
            auto view = string_to_view_format(texture_format);

            if (storage == lip::storage_format::unknown)
            {
                return create_texture_2d(input_file_name);
            }
            else
            {
                return create_texture_2d(input_file_name, storage, view);
            }
        }

        static uc::lip::texture2d_mip_chain create_texture_2d_mip_chain(const file_name_t& input_file_name, const std::string& texture_format)
        {
            auto storage = string_to_storage_format(texture_format);
            auto view = string_to_view_format(texture_format);

            if (storage == lip::storage_format::unknown)
            {
                return create_texture_2d_mip_chain(input_file_name);
            }
            else
            {
                return create_texture_2d_mip_chain(input_file_name, storage, view);
            }
        }

        static std::vector<std::string> materials( const std::vector<std::string>& names )
        {
            std::vector<std::string> s;

            s.resize(names.size());

            std::transform( std::cbegin(names), std::cend(names), std::begin(s), [](const auto& i )
            {
                return gx::import::assimp::material_name(i);
            });

            return s;
        }

        static std::vector<uint32_t> material_indices(const std::vector<std::string>& names, const std::vector<gx::import::geo::multi_material_mesh::material>& m)
        {
            std::vector<uint32_t> r;
            r.resize( names.size() ); //todo: clamp to materials size?

            for (auto i = 0U;i < names.size(); ++i)
            {
                const auto& n = names[i];

                auto id = std::find_if(std::begin(m), std::end(m), [n](const auto& a)
                {
                    return n == a.m_name;
                });

                if (id != std::end(m))
                {
                    r[i] = static_cast<uint32_t> (id - std::begin(m));
                }

            }

            return r;
        }

        template <typename mesh_create_functor, typename model_create_functor, typename copy_attributes> void convert_multi_textured_mesh( const file_name_t& input_file_name, const file_name_t& output_file_name, const mesh_create_functor& create_mesh, const model_create_functor& create_model, const copy_attributes& copy_attributes,  const std::vector<std::string>& texture_file_name, const std::vector<std::string>& texture_format )
        {
            auto m          = create_model();
            auto mesh       = create_mesh(input_file_name);

            concurrency::task_group g;

            size_t s = texture_file_name.size();
            m->m_textures.resize(s);

            for (auto i = 0U; i < s ; ++i)
            {
                g.run([i, &m, &texture_file_name, &texture_format]()
                {
                    m->m_textures[i] = create_texture_2d_mip_chain(texture_file_name[i], texture_format[i]);
                });
            }

            auto mats = materials(texture_file_name);

            g.run( [&m, &mesh, &input_file_name, &mats, &create_mesh, &copy_attributes ]()
            {
                copy_attributes(m.get(), mesh.get(), mats );
            });

            g.wait();

            mesh.reset();

            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        template <typename mesh_create_functor>
        void convert_skinned_mesh(const file_name_t& input_file_name, const file_name_t& output_file_name, const mesh_create_functor& create_mesh, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            std::unique_ptr< uc::lip::normal_skinned_model >    m = std::make_unique<uc::lip::normal_skinned_model>();
            std::shared_ptr<gx::import::geo::skinned_mesh>      mesh;

            concurrency::task_group g;

            size_t s = texture_file_name.size();
            m->m_textures.resize(s);

            for (auto i = 0U; i < s; ++i)
            {
                g.run([i, &m, &texture_file_name, &texture_format]()
                {
                    m->m_textures[i] = create_texture_2d(texture_file_name[i], texture_format[i]);
                });
            }

            auto mats = materials(texture_file_name);
            g.run([&m, &mesh, &input_file_name, &mats, &create_mesh]()
            {
                mesh            = mesh = create_mesh(input_file_name); 
                auto view       = gx::import::geo::skinned_mesh_material_view(mesh.get(), material_indices(mats, mesh->m_materials));
                auto positions  = gx::import::geo::merge_positions(&view);
                
                auto uvs        = gx::import::geo::merge_uvs(&view);
                auto faces      = gx::import::geo::merge_faces(&view);
                auto normals    = gx::import::geo::merge_normals(&view);
                
                auto weights    = gx::import::geo::merge_blend_weights(&view);
                auto indices    = gx::import::geo::merge_blend_indices(&view);
                auto ranges     = model::ranges(&view);

                m->m_indices.m_data.resize(faces.size() * 3);
                m->m_positions.m_data.resize(positions.size());
                m->m_normals.m_data.resize(normals.size());
                m->m_uv.m_data.resize(uvs.size());
                m->m_blend_weights.resize(weights.size());
                m->m_blend_indices.resize(indices.size());

                copy_indices(faces, m->m_indices.m_data);
                copy_positions(positions, m->m_positions.m_data);
                copy_normals(normals, m->m_normals.m_data);
                copy_uv(uvs, m->m_uv.m_data);

                copy_blend_weights(weights, m->m_blend_weights);
                copy_blend_indices(indices, m->m_blend_indices);

                for (auto&& i : ranges)
                {
                    lip::primitive_range r = i;

                    r.m_begin *= 3;
                    r.m_end *= 3;
                    m->m_primitive_ranges.push_back(r);
                }

            });

            g.wait();
            uc::lip::serialize_object(std::move(m), output_file_name);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_model_assimp(const file_name_t& input_file_name, const file_name_t output_file_name, assimp_flags_t a)
        {
            auto m = create_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_model_assimp_bridge(const file_name_t& input_file_name, const file_name_t output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_model_assimp(input_file_name, output_file_name, a);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_model_fbx(const file_name_t& input_file_name, const file_name_t output_file_name, assimp_flags_t a)
        {
            auto m = create_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_model_fbx_bridge(const file_name_t& input_file_name, const file_name_t output_file_name, assimp_flags_t a, const std::vector<file_name_t>& , const std::vector<file_name_t>& )
        {
            return convert_model_fbx(input_file_name, output_file_name, a);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        static void convert_parametrized_model_assimp(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_parametrized_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }
        
        static void convert_parametrized_model_assimp_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_parametrized_model_assimp(input_file_name, output_file_name, a);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_normal_model_assimp(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_normal_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_normal_model_assimp_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_normal_model_assimp(input_file_name, output_file_name, a);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_normal_parametrized_model_assimp(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_normal_parametrized_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_normal_parametrized_model_assimp_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_normal_parametrized_model_assimp(input_file_name, output_file_name, a);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_derivatives_parametrized_model_assimp(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_derivatives_parametrized_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_derivatives_parametrized_model_assimp_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_derivatives_parametrized_model_assimp(input_file_name, output_file_name, a);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_parametrized_model_fbx(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_parametrized_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_parametrized_model_fbx_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_parametrized_model_fbx(input_file_name, output_file_name, a);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_normal_model_fbx(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_normal_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_normal_model_fbx_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_normal_model_fbx(input_file_name, output_file_name, a);
        } 
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_normal_parametrized_model_fbx(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_normal_parametrized_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_normal_parametrized_model_fbx_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_normal_parametrized_model_fbx(input_file_name, output_file_name, a);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_derivatives_parametrized_model_fbx(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a)
        {
            auto m = create_derivatives_parametrized_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_derivatives_parametrized_model_fbx_bridge(const std::string& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>&, const std::vector<file_name_t>&)
        {
            convert_derivatives_parametrized_model_fbx(input_file_name, output_file_name, a);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_textured_model_assimp(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const file_name_t& texture_file_name, const std::string& texture_format)
        {
            auto m = create_textured_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });

            m->m_texture = create_texture_2d_mip_chain(texture_file_name, texture_format);
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_textured_model_assimp_bridge(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>& textures, const std::vector<file_name_t>& texture_formats)
        {
            convert_textured_model_assimp(input_file_name, output_file_name, a, textures[0], texture_formats[0]);
        }

        static void convert_textured_model_fbx(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t, const file_name_t& texture_file_name, const std::string& texture_format)
        {
            auto m = create_textured_model(input_file_name, [](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });

            m->m_texture = create_texture_2d_mip_chain(texture_file_name, texture_format);
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_textured_model_fbx_bridge(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>& textures, const std::vector<file_name_t>& texture_formats)
        {
            convert_textured_model_fbx(input_file_name, output_file_name, a, textures[0], texture_formats[0]);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_normal_textured_model_assimp(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const file_name_t& texture_file_name, const std::string& texture_format)
        {
            auto m = create_normal_textured_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });

            m->m_texture = create_texture_2d_mip_chain(texture_file_name, texture_format);
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_normal_textured_model_assimp_bridge(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>& textures, const std::vector<file_name_t>& texture_formats)
        {
            convert_normal_textured_model_assimp(input_file_name, output_file_name, a, textures[0], texture_formats[0]);
        }

        static void convert_normal_textured_model_fbx(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t, const file_name_t& texture_file_name, const std::string& texture_format)
        {
            auto m = create_normal_textured_model(input_file_name, [](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });

            m->m_texture = create_texture_2d_mip_chain(texture_file_name, texture_format);
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_normal_textured_model_fbx_bridge(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>& textures, const std::vector<file_name_t>& texture_formats)
        {
            convert_normal_textured_model_fbx(input_file_name, output_file_name, a, textures[0], texture_formats[0]);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_derivatives_textured_model_assimp(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const file_name_t& texture_file_name, const std::string& texture_format)
        {
            auto m = create_derivatives_textured_model(input_file_name, [a](const std::string& f)
            {
                return gx::import::assimp::create_mesh(f, a);
            });

            m->m_texture = create_texture_2d_mip_chain(texture_file_name, texture_format);
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_derivatives_textured_model_assimp_bridge(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>& textures, const std::vector<file_name_t>& texture_formats)
        {
            convert_derivatives_textured_model_assimp(input_file_name, output_file_name, a, textures[0], texture_formats[0]);
        }

        static void convert_derivatives_textured_model_fbx(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t, const file_name_t& texture_file_name, const std::string& texture_format)
        {
            auto m = create_derivatives_textured_model(input_file_name, [](const std::string& f)
            {
                return gx::import::fbx::create_mesh(f);
            });

            m->m_texture = create_texture_2d_mip_chain(texture_file_name, texture_format);
            uc::lip::serialize_object(std::move(m), output_file_name, uc::lzham::compress_buffer);
        }

        static void convert_derivatives_textured_model_fbx_bridge(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<file_name_t>& textures, const std::vector<file_name_t>& texture_formats)
        {
            convert_derivatives_textured_model_fbx(input_file_name, output_file_name, a, textures[0], texture_formats[0]);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void convert_multi_textured_model_assimp(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<std::string>& texture_file_name, const std::vector<std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::assimp::create_multi_material_mesh(input_file_name, a);
            };

            auto f1 = []()
            {
                return std::make_unique<lip::multi_textured_model>();
            };

            auto f2 = [](lip::multi_textured_model* d, const gx::import::geo::multi_material_mesh* s, const std::vector<std::string>& mats)
            {
                auto view = gx::import::geo::multi_mesh_material_view(s, material_indices(mats, s->m_materials));
                auto positions = gx::import::geo::merge_positions(&view);
                auto uvs = gx::import::geo::merge_uvs(&view);
                auto faces = gx::import::geo::merge_faces(&view);
                auto ranges = model::ranges(&view);

                d->m_indices.m_data.resize( faces.size() * 3 );
                d->m_positions.m_data.resize( positions.size());
                d->m_uv.m_data.resize(uvs.size());

                copy_indices(faces, d->m_indices.m_data);
                copy_positions(positions, d->m_positions.m_data);
                copy_uv(uvs, d->m_uv.m_data);

                for (auto&& i : ranges)
                {
                    lip::primitive_range r = i;

                    r.m_begin *= 3;
                    r.m_end *= 3;
                    d->m_primitive_ranges.push_back(r);
                }

            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }

        static void convert_multi_textured_model_fbx(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t, const std::vector<std::string>& texture_file_name, const std::vector<std::string>& texture_format)
        {
            auto f0 = [](const file_name_t& input_file_name)
            {
                return gx::import::fbx::create_multi_material_mesh(input_file_name);
            };

            auto f1 = []()
            {
                return std::make_unique<lip::multi_textured_model>();
            };

            auto f2 = [](lip::multi_textured_model* d, const gx::import::geo::multi_material_mesh* s, const std::vector<std::string>& mats)
            {
                auto view = gx::import::geo::multi_mesh_material_view(s, material_indices(mats, s->m_materials));
                auto positions = gx::import::geo::merge_positions(&view);
                auto uvs = gx::import::geo::merge_uvs(&view);
                auto faces = gx::import::geo::merge_faces(&view);
                auto ranges = model::ranges(&view);

                d->m_indices.m_data.resize(faces.size() * 3);
                d->m_positions.m_data.resize(positions.size());
                d->m_uv.m_data.resize(uvs.size());

                copy_indices(faces, d->m_indices.m_data);
                copy_positions(positions, d->m_positions.m_data);
                copy_uv(uvs, d->m_uv.m_data);

                for (auto&& i : ranges)
                {
                    lip::primitive_range r = i;

                    r.m_begin *= 3;
                    r.m_end *= 3;
                    d->m_primitive_ranges.push_back(r);
                }
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void copy_attributes_normal_multi_textured_model( lip::normal_multi_textured_model* d, const gx::import::geo::multi_material_mesh* s, const std::vector<std::string>& mats)
        {
            auto view = gx::import::geo::multi_mesh_material_view(s, material_indices(mats, s->m_materials));
            auto positions = gx::import::geo::merge_positions(&view);
            auto uvs = gx::import::geo::merge_uvs(&view);
            auto faces = gx::import::geo::merge_faces(&view);
            auto normals = gx::import::geo::merge_normals(&view);

            d->m_indices.m_data.resize(faces.size() * 3);
            d->m_positions.m_data.resize(positions.size());
            d->m_uv.m_data.resize(uvs.size());
            d->m_normals.m_data.resize(normals.size());

            copy_indices(faces, d->m_indices.m_data);
            copy_positions(positions, d->m_positions.m_data);
            copy_uv(uvs, d->m_uv.m_data);
            copy_normals(normals, d->m_normals.m_data);

            auto ranges = model::ranges(&view);

            for (auto&& i : ranges)
            {
                lip::primitive_range r = i;

                r.m_begin *= 3;
                r.m_end *= 3;
                d->m_primitive_ranges.push_back(r);
            }
        }

        static void convert_normal_multi_textured_model_assimp(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t a, const std::vector<std::string>& texture_file_name, const std::vector<std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::assimp::create_multi_material_mesh(input_file_name, a);
            };

            auto f1 = []()
            {
                return std::make_unique<lip::normal_multi_textured_model>();
            };

            auto f2 = [](lip::normal_multi_textured_model* d, const gx::import::geo::multi_material_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_normal_multi_textured_model(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }

        static void convert_normal_multi_textured_model_fbx(const file_name_t& input_file_name, const file_name_t& output_file_name, assimp_flags_t, const std::vector<std::string>& texture_file_name, const std::vector<std::string>& texture_format)
        {
            auto f0 = [](const file_name_t& input_file_name)
            {
                return gx::import::fbx::create_multi_material_mesh(input_file_name);
            };

            auto f1 = []()
            {
                return std::make_unique<lip::normal_multi_textured_model>();
            };

            auto f2 = [](lip::normal_multi_textured_model* d, const gx::import::geo::multi_material_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_normal_multi_textured_model(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void copy_attributes_normal_skinned_model_assimp(lip::normal_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
        {
            auto view = gx::import::geo::skinned_mesh_material_view(s, material_indices(mats, s->m_materials));
            auto positions = gx::import::geo::merge_positions(&view);

            auto uvs = gx::import::geo::merge_uvs(&view);
            auto faces = gx::import::geo::merge_faces(&view);
            auto normals = gx::import::geo::merge_normals(&view);

            auto weights = gx::import::geo::merge_blend_weights(&view);
            auto indices = gx::import::geo::merge_blend_indices(&view);
            auto ranges = model::ranges(&view);

            d->m_indices.m_data.resize(faces.size() * 3);
            d->m_positions.m_data.resize(positions.size());
            d->m_normals.m_data.resize(normals.size());
            d->m_uv.m_data.resize(uvs.size());
            d->m_blend_weights.resize(weights.size());
            d->m_blend_indices.resize(indices.size());

            copy_indices(faces, d->m_indices.m_data);
            copy_positions(positions, d->m_positions.m_data);
            copy_normals(normals, d->m_normals.m_data);
            copy_uv(uvs, d->m_uv.m_data);

            copy_blend_weights(weights, d->m_blend_weights);
            copy_blend_indices(indices, d->m_blend_indices);

            for (auto&& i : ranges)
            {
                lip::primitive_range r = i;

                r.m_begin *= 3;
                r.m_end *= 3;
                d->m_primitive_ranges.push_back(r);
            }
        }

        static void convert_normal_skinned_model_assimp(const std::string& input_file_name, const std::string& output_file_name, assimp_flags_t a, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::assimp::create_skinned_mesh(input_file_name, a);
            };

            auto f1 = []()
            {
                return std::make_unique<uc::lip::normal_skinned_model>();
            };

            auto f2 = [](lip::normal_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_normal_skinned_model_assimp(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);

        }

        static void convert_normal_skinned_model_fbx(const std::string& input_file_name, const std::string& output_file_name, assimp_flags_t a, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::fbx::create_skinned_mesh(input_file_name);
            };

            auto f1 = []()
            {
                return std::make_unique<uc::lip::normal_skinned_model>();
            };

            auto f2 = [](lip::normal_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_normal_skinned_model_assimp(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void copy_attributes_skinned_model(lip::normal_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
        {
            auto view = gx::import::geo::skinned_mesh_material_view(s, material_indices(mats, s->m_materials));
            auto positions = gx::import::geo::merge_positions(&view);

            auto uvs = gx::import::geo::merge_uvs(&view);
            auto faces = gx::import::geo::merge_faces(&view);
            auto normals = gx::import::geo::merge_normals(&view);

            auto weights = gx::import::geo::merge_blend_weights(&view);
            auto indices = gx::import::geo::merge_blend_indices(&view);
            auto ranges = model::ranges(&view);

            d->m_indices.m_data.resize(faces.size() * 3);
            d->m_positions.m_data.resize(positions.size());
            d->m_uv.m_data.resize(uvs.size());
            d->m_blend_weights.resize(weights.size());
            d->m_blend_indices.resize(indices.size());

            copy_indices(faces, d->m_indices.m_data);
            copy_positions(positions, d->m_positions.m_data);
            copy_normals(normals, d->m_normals.m_data);
            copy_uv(uvs, d->m_uv.m_data);

            copy_blend_weights(weights, d->m_blend_weights);
            copy_blend_indices(indices, d->m_blend_indices);

            for (auto&& i : ranges)
            {
                lip::primitive_range r = i;

                r.m_begin *= 3;
                r.m_end *= 3;
                d->m_primitive_ranges.push_back(r);
            }
        }

        static void convert_skinned_model_assimp(const std::string& input_file_name, const std::string& output_file_name, assimp_flags_t a, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::assimp::create_skinned_mesh(input_file_name, a);
            };

            auto f1 = []()
            {
                return std::make_unique<uc::lip::normal_skinned_model>();
            };

            auto f2 = [](lip::normal_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_skinned_model(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);

        }

        static void convert_skinned_model_fbx(const std::string& input_file_name, const std::string& output_file_name, assimp_flags_t a, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::fbx::create_skinned_mesh(input_file_name);
            };

            auto f1 = []()
            {
                return std::make_unique<uc::lip::normal_skinned_model>();
            };

            auto f2 = [](lip::normal_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_skinned_model(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void copy_attributes_derivatives_skinned_model(lip::derivatives_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
        {
            auto view = gx::import::geo::skinned_mesh_material_view(s, material_indices(mats, s->m_materials));
            auto positions = gx::import::geo::merge_positions(&view);

            auto uvs = gx::import::geo::merge_uvs(&view);
            auto faces = gx::import::geo::merge_faces(&view);
            auto normals = gx::import::geo::merge_normals(&view);

            auto weights = gx::import::geo::merge_blend_weights(&view);
            auto indices = gx::import::geo::merge_blend_indices(&view);
            auto tangents = gx::import::geo::merge_tangents(&view);
            auto ranges = model::ranges(&view);

            d->m_indices.m_data.resize(faces.size() * 3);
            d->m_positions.m_data.resize(positions.size());
            d->m_normals.m_data.resize(normals.size());
            d->m_uv.m_data.resize(uvs.size());
            d->m_tangents.m_data.resize(tangents.size());

            d->m_blend_weights.resize(weights.size());
            d->m_blend_indices.resize(indices.size());

            copy_indices(faces, d->m_indices.m_data);
            copy_positions(positions, d->m_positions.m_data);
            copy_normals(normals, d->m_normals.m_data);
            copy_uv(uvs, d->m_uv.m_data);
            copy_tangents(tangents, d->m_tangents.m_data);

            copy_blend_weights(weights, d->m_blend_weights);
            copy_blend_indices(indices, d->m_blend_indices);


            for (auto&& i : ranges)
            {
                lip::primitive_range r = i;

                r.m_begin *= 3;
                r.m_end *= 3;
                d->m_primitive_ranges.push_back(r);
            }
        }

        static void convert_derivatives_skinned_model_assimp(const std::string& input_file_name, const std::string& output_file_name, assimp_flags_t a, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::assimp::create_skinned_mesh(input_file_name, a);
            };

            auto f1 = []()
            {
                return std::make_unique<uc::lip::derivatives_skinned_model>();
            };

            auto f2 = [](lip::derivatives_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_derivatives_skinned_model(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);

        }

        static void convert_derivatives_skinned_model_fbx(const std::string& input_file_name, const std::string& output_file_name, assimp_flags_t a, const std::vector< std::string>& texture_file_name, const std::vector< std::string>& texture_format)
        {
            auto f0 = [a](const file_name_t& input_file_name)
            {
                return gx::import::fbx::create_skinned_mesh(input_file_name);
            };

            auto f1 = []()
            {
                return std::make_unique<uc::lip::derivatives_skinned_model>();
            };

            auto f2 = [](lip::derivatives_skinned_model* d, const gx::import::geo::skinned_mesh* s, const std::vector<std::string>& mats)
            {
                copy_attributes_derivatives_skinned_model(d, s, mats);
            };

            convert_multi_textured_mesh(input_file_name, output_file_name, f0, f1, f2, texture_file_name, texture_format);
        }
        static std::tuple< std::vector<std::string>, std::vector<std::string> > clean_duplicate_textures(const std::vector<std::string>& texture_names, const std::vector<std::string>& texture_formats)
        {
            using namespace std;

            set < tuple< string, string> > s;

            for (auto i = 0; i < texture_names.size(); ++i)
            {
                s.insert(make_tuple(texture_names[i], texture_formats[i]));
            }

            vector<string> r0;
            vector<string> r1;

            for (auto&& i : s)
            {
                r0.push_back(std::get<0>(i));
                r1.push_back(std::get<1>(i));
            }

            return make_tuple(r0, r1);
        }
    }
}

namespace uc
{
    namespace model
    {
        using convert_function = std::function<void(const file_name_t&, const file_name_t&, assimp_flags_t, const std::vector<file_name_t>&, const std::vector<file_name_t>& texture_format )>;

        

    }
}

static std::string get_environment()
{
#if defined(_X86)
    return "x86";
#endif

#if defined(_X64)
    return "x64";
#endif
}

namespace uc
{
    namespace lip
    {
        template <typename t>
        std::string model_to_string_type();

        template <>
        std::string model_to_string_type<model>()
        {
            return "model";
        }

        template <>
        std::string model_to_string_type<normal_model>()
        {
            return "normal_model";
        }

        template <>
        std::string model_to_string_type<parametrized_model>()
        {
            return "parametrized_model";
        }

        template <>
        std::string model_to_string_type<normal_parametrized_model>()
        {
            return "normal_parametrized_model";
        }

        template <>
        std::string model_to_string_type<derivatives_parametrized_model>()
        {
            return "derivatives_parametrized_model";
        }

        template <>
        std::string model_to_string_type<textured_model>()
        {
            return "textured_model";
        }

        template <>
        std::string model_to_string_type<normal_textured_model>()
        {
            return "normal_textured_model";
        }

        template <>
        std::string model_to_string_type<derivatives_textured_model>()
        {
            return "derivatives_textured_model";
        }

        template <>
        std::string model_to_string_type<multi_textured_model>()
        {
            return "multi_textured_model";
        }

        template <>
        std::string model_to_string_type<normal_multi_textured_model>()
        {
            return "normal_multi_textured_model";
        }

        template <>
        std::string model_to_string_type<derivatives_multi_textured_model>()
        {
            return "derivatives_multi_textured_model";
        }

        template <>
        std::string model_to_string_type<skinned_model>()
        {
            return "skinned_model";
        }

        template <>
        std::string model_to_string_type<normal_skinned_model>()
        {
            return "normal_skinned_model";
        }

        template <>
        std::string model_to_string_type<derivatives_skinned_model>()
        {
            return "derivatives_skinned_model";
        }
    }
}

namespace uc
{
    namespace model
    {
        struct convertor
        {
            std::string         m_model_type;
            bool                m_needs_textures;
            convert_function    m_convert_function_fbx;
            convert_function    m_convert_function_assimp;
        };

        template <typename > convertor make_convertor();


        template<> convertor make_convertor<lip::model>()
        {
            return
            {
                lip::model_to_string_type<lip::model>(),
                false,
                convert_model_fbx_bridge,
                convert_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::normal_model>()
        {
            return
            {
                lip::model_to_string_type<lip::normal_model>(),
                false,
                convert_normal_model_fbx_bridge,
                convert_normal_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::parametrized_model>()
        {
            return
            {
                lip::model_to_string_type<lip::parametrized_model>(),
                false,
                convert_parametrized_model_fbx_bridge,
                convert_parametrized_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::normal_parametrized_model>()
        {
            return
            {
                lip::model_to_string_type<lip::normal_parametrized_model>(),
                false,
                convert_normal_parametrized_model_fbx_bridge,
                convert_normal_parametrized_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::derivatives_parametrized_model>()
        {
            return
            {
                lip::model_to_string_type<lip::derivatives_parametrized_model>(),
                false,
                convert_derivatives_parametrized_model_fbx_bridge,
                convert_derivatives_parametrized_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::textured_model>()
        {
            return
            {
                lip::model_to_string_type<lip::textured_model>(),
                true,
                convert_textured_model_fbx_bridge,
                convert_textured_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::normal_textured_model>()
        {
            return
            {
                lip::model_to_string_type<lip::normal_textured_model>(),
                true,
                convert_normal_textured_model_fbx_bridge,
                convert_normal_textured_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::derivatives_textured_model>()
        {
            return
            {
                lip::model_to_string_type<lip::derivatives_textured_model>(),
                true,
                convert_derivatives_textured_model_fbx_bridge,
                convert_derivatives_textured_model_assimp_bridge
            };
        }

        template<> convertor make_convertor<lip::multi_textured_model>()
        {
            return
            {
                lip::model_to_string_type<lip::multi_textured_model>(),
                true,
                convert_multi_textured_model_fbx,
                convert_multi_textured_model_assimp
            };
        }

        template<> convertor make_convertor<lip::normal_skinned_model>()
        {
            return
            {
                lip::model_to_string_type<lip::normal_skinned_model>(),
                true,
                convert_normal_skinned_model_fbx,
                convert_normal_skinned_model_assimp
            };
        }

        template<> convertor make_convertor<lip::derivatives_skinned_model>()
        {
            return
            {
                lip::model_to_string_type<lip::derivatives_skinned_model>(),
                true,
                convert_derivatives_skinned_model_fbx,
                convert_derivatives_skinned_model_assimp
            };
        }

        template<> convertor make_convertor<lip::skinned_model>()
        {
            return
            {
                lip::model_to_string_type<lip::skinned_model>(),
                true,
                convert_skinned_model_fbx,
                convert_skinned_model_assimp
            };
        }

        std::vector< convertor > make_convertors()
        {
            //todo: typelists

            std::vector< convertor > r;

            r.push_back(make_convertor<lip::model>());
            r.push_back(make_convertor<lip::parametrized_model>());
            r.push_back(make_convertor<lip::normal_model>());
            r.push_back(make_convertor<lip::normal_parametrized_model>());
            r.push_back(make_convertor<lip::derivatives_parametrized_model>());

            r.push_back(make_convertor<lip::textured_model>());
            r.push_back(make_convertor<lip::normal_textured_model>());
            r.push_back(make_convertor<lip::derivatives_textured_model>());

            r.push_back(make_convertor<lip::multi_textured_model>());
            r.push_back(make_convertor<lip::normal_skinned_model>());
            r.push_back(make_convertor<lip::skinned_model>());
            r.push_back(make_convertor<lip::derivatives_skinned_model>());


            return r;
        }

        const convertor get_convertor(const std::string& v)
        {
            static auto convertors = make_convertors();

            auto r = std::find_if(convertors.cbegin(), convertors.cend(), [&v](const auto& v0)
            {
                return v0.m_model_type == v;
            });

            if (r == convertors.end())
            {
                raise_error<exception>("cannot find convertor for model " + v);
            }

            return *r;
        }
    }
}

int32_t main(int32_t argc, const char* argv[])
{
    using namespace uc::model;
    std::string input_model_error = "uc_dev_model_r.exe";
    try
    {
        uc::os::windows::com_initializer com;
        compressonator::initializer      compressonator;

        std::cout << "Command line:" << std::endl;
        for (auto i = 0; i < argc; ++i)
        {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;

        //
        // ShutdownBCLibrary - Shutdown the BC6H or BC7 library
        //

        auto&& om = build_option_map(argc, argv);
        auto&& vm = std::get<0>(om);
        auto&& desc = std::get<1>(om);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        auto input_model = get_input_model(vm);
        auto output_model = get_output_model(vm);


        uint32_t assimp_options = static_cast<uint32_t>(get_assimp_option(vm));

        assimp_options |= aiProcess_CalcTangentSpace; //always get tangents

        auto model_type = get_model_type(vm);

        std::cout << "building model (" << get_environment() << ") " << std::endl;
        std::cout << "assimp options:" << uc::gx::import::assimp::assimp_postprocess_option_to_string(assimp_options) << std::endl;

        if (model_type == "default")
        {
            model_type = "model";
        }

        if (model_type == "parametrized")
        {
            model_type = "parametrized_model";
        }

        if (model_type == "multi_textured")
        {
            model_type = "multi_textured_model";
        }

        if (model_type == "textured")
        {
            model_type = "textured_model";
        }

        if (model_type == "skinned")
        {
            model_type = "normal_skinned_model";
        }

        auto convertor = get_convertor(model_type);

        std::cout << "building model:"      << input_model << std::endl;
        std::cout << "building model type:" << convertor.m_model_type << std::endl;

        
        {
            std::experimental::filesystem::path path(input_model);
            auto e = path.extension().wstring();

            std::vector<std::string> textures;
            std::vector<std::string> texture_formats;

            if (convertor.m_needs_textures)
            {
                tie(textures, texture_formats) = uc::model::clean_duplicate_textures(get_textures(vm), get_texture_formats(vm));
            }

            if (e == L".fbx")
            {
                std::cout << "building importer: fbx" << std::endl;
                convertor.m_convert_function_fbx(input_model, output_model, assimp_options, textures, texture_formats);
            }
            else
            {
                std::cout << "building importer: assimp" << std::endl;
                convertor.m_convert_function_assimp(input_model, output_model, assimp_options, textures, texture_formats);
            }
        }
       
    }

    catch (const std::exception& e)
    {
        std::cerr << input_model_error << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

