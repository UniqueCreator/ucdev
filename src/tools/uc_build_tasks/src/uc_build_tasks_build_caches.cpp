#include "pch.h"

#include "uc_build_tasks_build_caches.h"
#include "uc_build_tasks_dependencies.h"
#include "uc_build_tasks_include_parser.h"
#include "uc_build_tasks_dependencies_boost.h"
#include "uc_build_tasks_error.h"

#include "uc_build_md5.h"

#include <memory>
#include <fstream>
#include <string>
#include <iterator>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>



namespace uc
{
    namespace build
    {
        namespace tasks
        {
            std::string     create_hash( const std::string& file_name )
            {
                std::ifstream instream(file_name);
                if (instream.good())
                {
                    //blocking call, reads the file
                    std::string input(std::istreambuf_iterator<char>(instream.rdbuf()), std::istreambuf_iterator<char>());
                    return md5(input);
                }
                else
                {
                    raise_error("cannot open file for hash creation");
                    return std::string();
                }
            }

            dependency_cache_item create_dependency_file(const std::string& file_name)
            {
                return  dependency_cache_item(  file_name, create_hash(file_name), dependency_type::file );
            }

            dependencies build_file_cache_item(const std::string& file_name, const std::vector< std::string> include_dirs, const std::vector<std::string> macros)
            {
                auto&& includes = get_includes(file_name, macros, include_dirs);

                dependencies r; 
                r.m_files.insert(create_dependency_file(file_name));

                for (auto&& i : includes)
                {
                    r.m_files.insert(create_dependency_file(i));
                }

                return r;
            }

            dependencies prepare_cache_build_item(dependencies&& d, const std::vector< dependency_cache_item >& sd)
            {
                dependencies result = std::move(d);

                for (auto&& i : sd)
                {
                    result.m_files.insert(i);
                }

                return result;
            }

            dependencies prepare_cache_build_item(dependencies&& d, dependency_cache_item && sd)
            {
                dependencies result = std::move(d);
                result.m_files.insert(std::move(sd));
                return result;
            }

            dependency_cache_item create_item_data_dependency( const item_meta_data& d )
            {
                std::string s = d.m_dxbc + d.m_dxbc_header + d.m_dxbc_variable_name + d.m_pdb_file;

                auto index = s.size();
                
                s.resize(s.size() + 4);

                uint32_t stage = static_cast<uint32_t> (d.m_pipeline_stage);

                s[index]     = stage & 0xff;
                s[index + 1] = (stage >> 8 ) & 0xff;
                s[index + 2] = (stage >> 16) & 0xff;
                s[index + 3] = (stage >> 24) & 0xff;

                return dependency_cache_item ( "uc_build_tasks_shader_input", md5(s) ) ;
            }

            file_cache load_cache( const std::string& cache_file)
            {
                file_cache cache;

                // open the archive
                std::ifstream ifs(cache_file);
                if (ifs.good())
                {
                    boost::archive::text_iarchive ia(ifs);
                    // restore the schedule from the archive
                    ia >> cache;
                }
                return cache;
            }

            bool file_exists(const std::string& file_name)
            {
                std::ifstream instream(file_name);
                return instream.good();
            }

            void save_cache(const std::string& cache_file, const file_cache& cache)
            {
                // open the archive
                std::ofstream ofs(cache_file);
                if (ofs.good())
                {
                    boost::archive::text_oarchive ia(ofs);
                    // restore the schedule from the archive
                    ia << cache;
                }
            }

            std::tuple< std::vector< item_meta_data>, file_cache >   build_caches(const std::vector< item_meta_data>& files_to_parse, const std::string& cache_file, const std::vector< dependency_cache_item >& static_dependencies )
            {
                std::vector< std::unordered_set < std::string > > includes;
                auto&& cache = load_cache( cache_file );

                std::vector< uint32_t     >   files_indices;
                std::vector< item_meta_data>  files_to_build;
                std::vector< dependencies > files_to_build_caches;

                uint32_t index = 0;

                for ( auto&& i : files_to_parse )
                {
                    auto&& cache_item = prepare_cache_build_item(
                                                                    prepare_cache_build_item(build_file_cache_item(i.m_file_name, i.m_additional_include_directories, i.m_preprocessor_directives), static_dependencies),
                                                                    create_item_data_dependency(i));
                    auto&& found_cache_item = cache.m_cache_data.find(i.m_file_name);
                    bool  add_item = false;

                    //if the files is missing in the cache, rebuild
                    if ( found_cache_item == std::end(cache.m_cache_data) )
                    {
                        add_item = true;
                    }
                    else
                    {
                        //if the cache_items do not match ( a file has changed )
                        if (found_cache_item->second != cache_item)
                        {
                            add_item = true;
                        }
                        else
                        {
                            //outputs must exist, or need to exist
                            //( !i.m_pdb_file.empty() && file_exists( i.m_pdb_file ) ) -> removed, root signature has no pdb
                            if (!( file_exists(i.m_dxbc)  &&  file_exists(i.m_dxbc_header )
                                && file_exists(i.m_cpp_generated_file_name)
                                && file_exists(i.m_cpp_generated_file_name_header)
                                ))
                            {
                                add_item = true;
                            }
                        }
                    }

                    if (add_item)
                    {
                        files_to_build.push_back(i);
                        files_to_build_caches.push_back(cache_item);
                        files_indices.push_back(index);
                    }

                    index++;
                }

                //update the cache
                for (auto i = 0U; i < files_to_build.size(); ++i)
                {
                    cache.m_cache_data[ files_to_build[i].m_file_name ] = files_to_build_caches[i];
                }

                return make_tuple( std::move(files_to_build), std::move(cache) );
            }
        }
    }
}